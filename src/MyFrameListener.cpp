#include "MyFrameListener.hpp"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om) {
  OIS::ParamList param;
  size_t windowHandle;
  std::ostringstream wHandleStr;

  _win = win;
  _camera = cam;
  _overlayManager = om;

  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));

  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*> (_inputManager->createInputObject(OIS::OISKeyboard, false));
  _mouse = static_cast<OIS::Mouse*> (_inputManager->createInputObject(OIS::OISMouse, false));
  _mouse->getMouseState().width = _win->getWidth();
  _mouse->getMouseState().height = _win->getHeight();
}

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  OIS::InputManager::destroyInputSystem(_inputManager);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  Ogre::Vector3 vt(0, 0, 0);
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;

  _keyboard->capture();
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
  if(_keyboard->isKeyDown(OIS::KC_UP))    vt += Ogre::Vector3(0, 0, -1);
  if(_keyboard->isKeyDown(OIS::KC_DOWN))  vt += Ogre::Vector3(0, 0, 1);
  if(_keyboard->isKeyDown(OIS::KC_LEFT))  vt += Ogre::Vector3(-1, 0, 0);
  if(_keyboard->isKeyDown(OIS::KC_RIGHT)) vt += Ogre::Vector3(1, 0, 0);

  _camera->moveRelative(vt * deltaT * tSpeed);

  //Mover ratón
  _mouse->capture();

  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  // Gestion del overlay ---------------------------------------------
  Ogre::OverlayElement *oe;

  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(Ogre::StringConverter::toString(fps));

  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);
  oe->setTop(posy);

  return true;
}
