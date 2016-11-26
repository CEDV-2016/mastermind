#include "MyFrameListener.hpp"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om, Ogre::SceneManager* sm) {
  OIS::ParamList param;
  size_t windowHandle;
  std::ostringstream wHandleStr;

  _win = win;
  _camera = cam;
  _overlayManager = om;
  _sceneManager = sm;

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

  if (mbleft) {
    if (_selectedNode != NULL) {
      _selectedNode->showBoundingBox(false);
      _selectedNode = NULL;
    }

    setRayQuery(posx, posy);
    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it;
    it = result.begin();

    if (it != result.end()) {
      _selectedNode = it->movable->getParentSceneNode();
      _selectedNode->showBoundingBox(true);
    }

  }

  // Gestion del overlay ---------------------------------------------
  Ogre::OverlayElement *oe;

  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(Ogre::StringConverter::toString(fps));

  oe = _overlayManager->getOverlayElement("objectInfo");
  if (_selectedNode != NULL)
  {
    oe->setCaption(_selectedNode->getName());
  }
  else
  {
    oe->setCaption("");
  }

  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);
  oe->setTop(posy);

  return true;
}

Ogre::Ray MyFrameListener::setRayQuery(int posx, int posy) { //Sin acabar
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);

  return rayMouse;
}
