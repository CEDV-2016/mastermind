#include "MyFrameListener.hpp"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om, Ogre::SceneManager* sm, BallsFactory* bf) {
  OIS::ParamList param;
  size_t windowHandle;
  std::ostringstream wHandleStr;

  _win = win;
  _camera = cam;
  _overlayManager = om;
  _sceneManager = sm;
  _state = SELECTING_BALL;
  _ballsFactory = bf;
  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));

  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*> (_inputManager->createInputObject(OIS::OISKeyboard, false));
  _mouse = static_cast<OIS::Mouse*> (_inputManager->createInputObject(OIS::OISMouse, false));
  _mouse->getMouseState().width = _win->getWidth();
  _mouse->getMouseState().height = _win->getHeight();

  _raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());
  _selectedNode = NULL;
  _current_ball = NULL;
}

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  _sceneManager->destroyQuery(_raySceneQuery);
  OIS::InputManager::destroyInputSystem(_inputManager);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  Ogre::Vector3 vt(0, 0, 0);
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;
  bool mbleft = false;

  //Teclado
  _keyboard->capture();
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
  if(_keyboard->isKeyDown(OIS::KC_Q)) vt += Ogre::Vector3(0, 0, -1);
  if(_keyboard->isKeyDown(OIS::KC_E)) vt += Ogre::Vector3(0, 0, 1);
  if(_keyboard->isKeyDown(OIS::KC_A)) vt += Ogre::Vector3(-1, 0, 0);
  if(_keyboard->isKeyDown(OIS::KC_W)) vt += Ogre::Vector3(0, 1, 0);
  if(_keyboard->isKeyDown(OIS::KC_S)) vt += Ogre::Vector3(0, -1, 0);
  if(_keyboard->isKeyDown(OIS::KC_D)) vt += Ogre::Vector3(1, 0, 0);

  _camera->moveRelative(vt * deltaT * tSpeed);

  //Mover ratón
  _mouse->capture();
  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.
  mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left); //Click izquierdo


  int color;

  switch(_state){

    /**
    * Este estado se ejecuta cuando aun no hemos seleccionado ningun color de las
    * cajas de bolas. Al hacer click en una caja creamos una bola de ese color.
    */
    case SELECTING_BALL:

    if (mbleft) {
      if (_selectedNode != NULL) {
        _selectedNode->showBoundingBox(false);
        _selectedNode = NULL;
      }

      setRayQuery(posx, posy, -1);
      Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
      Ogre::RaySceneQueryResult::iterator it;
      it = result.begin();

      if (it != result.end()) {
        _selectedNode = it->movable->getParentSceneNode();
        _selectedNode->showBoundingBox(true);
      }

      color = _selectedNode->getAttachedObject(0)->getQueryFlags();
      if (color != BOARD && color != GROUND && color != BOX) {
        _current_ball = _ballsFactory->createBall(color);

        _state = MOVING_BALL;
      }

    }
    break;

    /**
    * Este estado se ejecuta cuando hemos seleccionado una bola. Ahora tenemos
    * que moverla a las mismas coordenadas del ratón hasta que haga click y la
    * coloque encima del tablero.
    */
    case MOVING_BALL:

    Ogre::Ray r = setRayQuery(posx, posy, -1);
    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it;
    Ogre::Vector3 position;
    it = result.begin();

    if (it != result.end()) {
      position = r.getPoint(it->distance);
      int y = position.y < 0.1 ? 0 : 1;
      _current_ball->setPosition(position.x, y, position.z);
      color = it->movable->getParentSceneNode()->getAttachedObject(0)->getQueryFlags();
    }

    /**
    * Como la bola está debajo del ratón no se puede pinchar en el tablero
    * (solo detecta bola). Mediante queries le decimos que solo mire el tablero.
    */

    r = setRayQuery(posx, posy, BOARD);
    result = _raySceneQuery->execute();
    it = result.begin();

    if (it != result.end()) {
      color = it->movable->getParentSceneNode()->getAttachedObject(0)->getQueryFlags();
    }


    if (mbleft) {
      if (color == BOARD) {
        _current_ball->setPosition(position.x, position.y, position.z);
        _state = SELECTING_BALL;
      }
    }
    break;
  }

  // Overlay management
  Ogre::OverlayElement *oe;
  std::string flags, msg;
  std::stringstream stream;

  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(Ogre::StringConverter::toString(fps));

  oe = _overlayManager->getOverlayElement("objectInfo");
  if (_selectedNode != NULL) {
    flags = Ogre::StringConverter::toString(_selectedNode->getAttachedObject(0)->getQueryFlags());
    switch (atoi(flags.c_str())) {
      case RED:    msg = "RED";   break;
      case BLUE:   msg = "BLUE";  break;
      case GREEN:  msg = "GREEN"; break;
      case PINK:   msg = "PINK";  break;
      case WHITE:  msg = "WHITE"; break;
      case BLACK:  msg = "BLACK"; break;
      case BOARD:  msg = "BOARD"; break;
      case GROUND: msg = "GROUND"; break;
      default:     msg = "NO COLOR"; break;
    }
    // stream << _selectedNode->getName() << " (Flags: " << msg << ")";
    stream << "Color: " << msg << " State: " << _state;
    oe->setCaption(stream.str());
  }
  else {
    stream << "Nothing selected. State: " << _state;
    oe->setCaption(stream.str());
  }

  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);
  oe->setTop(posy);

  return true;
}

Ogre::Ray MyFrameListener::setRayQuery(int posx, int posy, int mask) {
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  if (mask != -1){
    _raySceneQuery->setQueryMask(mask);
  }  else {
    _raySceneQuery->setQueryMask(RED | BLUE | GREEN | PINK | WHITE | BLACK | BOARD | GROUND | BOX);
  }
  return rayMouse;
}
