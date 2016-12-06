#include "MyFrameListener.hpp"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om, Ogre::SceneManager* sm, BallsFactory* bf) {
  OIS::ParamList param;
  size_t windowHandle;
  std::ostringstream wHandleStr;

  _win = win;
  _camera = cam;
  _overlayManager = om;
  _sceneManager = sm;
  _ballsFactory = bf;
  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));

  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*> (_inputManager->createInputObject(OIS::OISKeyboard, false));
  _mouse = static_cast<OIS::Mouse*> (_inputManager->createInputObject(OIS::OISMouse, false));
  _mouse->getMouseState().width = _win->getWidth();
  _mouse->getMouseState().height = _win->getHeight();
  _game = new Game("Player");

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
  // Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;
  bool mbleft = false;

  _keyboard->capture();
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
  // if(_keyboard->isKeyDown(OIS::KC_Q)) vt += Ogre::Vector3(0, 0, -1);
  // if(_keyboard->isKeyDown(OIS::KC_E)) vt += Ogre::Vector3(0, 0, 1);
  // if(_keyboard->isKeyDown(OIS::KC_A)) vt += Ogre::Vector3(-1, 0, 0);
  // if(_keyboard->isKeyDown(OIS::KC_W)) vt += Ogre::Vector3(0, 1, 0);
  // if(_keyboard->isKeyDown(OIS::KC_S)) vt += Ogre::Vector3(0, -1, 0);
  // if(_keyboard->isKeyDown(OIS::KC_D)) vt += Ogre::Vector3(1, 0, 0);
  // _camera->moveRelative(vt * deltaT * tSpeed);
  if(_keyboard->isKeyDown(OIS::KC_P)) std::cout << _game->toString() << std::endl;


  //Mover ratón
  _mouse->capture();
  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.
  mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left); //Click izquierdo

  switch(_game->getState())
  {

    /**
    * Este estado se ejecuta cuando aun no hemos seleccionado ningun color de las
    * cajas de bolas. Al hacer click en una caja creamos una bola de ese color.
    */
    case SELECTING:
    {
      /*
      * Molaría hacer que al pasar el ratón por encima de un slew el color se
      * vuleva blanquecino.
      */
      if (mbleft) {

        if (_selectedNode != NULL) _selectedNode->showBoundingBox(false);
        _selectedNode = NULL;

        setRayQuery(posx, posy, SLEW | BUTTON);
        Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator it;
        it = result.begin();

        if (it != result.end()) {
          _selectedNode = it->movable->getParentSceneNode();
          _selectedNode->showBoundingBox(true);
        }

        if (_selectedNode != NULL) {
          switch (_selectedNode->getAttachedObject(0)->getQueryFlags()){

            case SLEW:
            {
              std::string color;
              std::istringstream full_name(_selectedNode->getName());
              while (getline(full_name, color, '_')); //Obtenemos el último split

              _current_ball = _ballsFactory->createBall(color);

              _game->setState(MOVING);
            } break;

            case BUTTON: {
              _game->setState(CHECKING);
            } break;
          }
        }
      }
    } break;

    /*
    * Este estado se ejecuta cuando hemos seleccionado una bola. Ahora tenemos
    * que moverla a las mismas coordenadas del ratón hasta que haga click sobre
    * una cuadrícula encima del tablero.
    */
    case MOVING:
    {
      Ogre::Ray r = setRayQuery(posx, posy, -1);
      Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
      Ogre::RaySceneQueryResult::iterator it;
      Ogre::Vector3 position;
      it = result.begin();

      if (it != result.end()) {
        position = r.getPoint(it->distance);
        int y = position.y < 0.1 ? 0 : 1;
        _current_ball->setPosition(position.x, y, position.z);
        // flags = it->movable->getParentSceneNode()->getAttachedObject(0)->getQueryFlags();
      }

      /*
      * Como la bola está debajo del ratón no se puede pinchar en el tablero
      * (solo detecta bola). Mediante queries le decimos que solo mire los tiles.
      */
      if (mbleft) {
        if (_selectedNode != NULL){
          _selectedNode->showBoundingBox(false);
          _selectedNode = NULL;
        }

        r = setRayQuery(posx, posy, TILE);
        result = _raySceneQuery->execute();
        it = result.begin();

        if (it != result.end()) {
          _selectedNode = it->movable->getParentSceneNode();
          _selectedNode->showBoundingBox(true);
        }
        if (_selectedNode != NULL) {
          std::string coordinates, col, row, color;
          int int_col, int_row;
          std::istringstream tile_name(_selectedNode->getName());
          std::istringstream ball_name(_current_ball->getName());
          while (getline(tile_name, coordinates, '_'));
          while (getline(ball_name, color, '_'));

          row = coordinates.substr(0,1);
          col = coordinates.substr(1,2);
          std::istringstream(row) >> int_row;
          std::istringstream(col) >> int_col;

          if (_game->getCurrentRow() == int_row) {
            std::cout << "Bola " << color << " en la fila = " << int_row << " columna = " << int_col << std::endl;
            _game->addBall(int_row, int_col, color);
            _current_ball->setPosition(_selectedNode->getPosition());
            _game->setState(SELECTING);
          }
        }
      }
    } break;

    case CHECKING:
    {
      if (_game->currentRowFull())
      {
        if (_game->checkCurrentRow())
        {
          std::cout << "Has ganado!\n";
          _game->setState(GAME_OVER);
        }
        else
        {
          _game->addCurrentRow();

          if (_game->getCurrentRow() > NUM_ROWS-1)
          {
            std::cout << "Has perdido!\n";
            _game->setState(GAME_OVER);
          }
          else
          {
            std::cout << "Mala linea, pero te quedan filas!\n";
            _game->setState(SELECTING);
          }
        }
      }
      else
      {
        std::cout << "Aún no has completado la fila!\n";
        _game->setState(SELECTING);
      }

    } break;

    case GAME_OVER:
    {
    }
    break;
  } //switch

  // Overlay management
  Ogre::OverlayElement *oe;
  std::string msg, color;
  std::stringstream stream;

  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(Ogre::StringConverter::toString(fps));

  oe = _overlayManager->getOverlayElement("objectInfo");
  if (_selectedNode != NULL) {

    stream << "Flags: " << _selectedNode->getName() << " State: " << _game->getState();
    oe->setCaption(stream.str());
  }
  else {
    stream << "Nothing selected. State: " << _game->getState();
    oe->setCaption(stream.str());
  }

  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);
  oe->setTop(posy);

  return true;
}

Ogre::Ray MyFrameListener::setRayQuery(int posx, int posy, Ogre::uint32 mask) {
  Ogre::uint32 all_masks = BOARD | GROUND | TILE | BOX | SLEW | BALL | BUTTON;

  Ogre::Ray rayMouse = _camera->getCameraToViewportRay (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);

  _raySceneQuery->setQueryMask(mask == (Ogre::uint32)-1 ? all_masks : mask);

  return rayMouse;
}
