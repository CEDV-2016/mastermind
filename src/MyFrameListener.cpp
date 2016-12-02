#include "MyFrameListener.hpp"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam,Ogre::OverlayManager *om, Ogre::SceneManager* sm) {
  OIS::ParamList param;
  size_t windowHandle;
  std::ostringstream wHandleStr;

  _win = win;
  _camera = cam;
  _overlayManager = om;
  _sceneManager = sm;
  _quit = false;
  _sheet = NULL, _init = NULL, _credits = NULL, _ranking = NULL, _player = NULL;

  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));
  param.insert(std::make_pair("x11_mouse_hide", std::string("true"))); // Hide the OS mouse cursor

  // To prevent OIS from kidnapping the mouse cursor. This is very useful while debugging a segmentation fault with GDB
#if defined OIS_WIN32_PLATFORM
   param.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
   param.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
   param.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
   param.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
   param.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
   param.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
   param.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*> (_inputManager->createInputObject(OIS::OISKeyboard, true));
  _mouse = static_cast<OIS::Mouse*> (_inputManager->createInputObject(OIS::OISMouse, true));
  _mouse->getMouseState().width = _win->getWidth();
  _mouse->getMouseState().height = _win->getHeight();

  _raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());
  _selectedNode = NULL;

  _keyboard->setEventCallback(this);
  _mouse->setEventCallback(this);
}

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  _sceneManager->destroyQuery(_raySceneQuery);
  OIS::InputManager::destroyInputSystem(_inputManager);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  _timeSinceLastFrame = evt.timeSinceLastFrame;
  CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(_timeSinceLastFrame);
  _mouse->capture();
  _keyboard->capture();
  if(_quit) return false;

  Ogre::Vector3 vt(0, 0, 0);
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;
  bool mbleft = false;

  //Teclado
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
  if(_keyboard->isKeyDown(OIS::KC_Q)) vt += Ogre::Vector3(0, 0, -1);
  if(_keyboard->isKeyDown(OIS::KC_E)) vt += Ogre::Vector3(0, 0, 1);
  if(_keyboard->isKeyDown(OIS::KC_A)) vt += Ogre::Vector3(-1, 0, 0);
  if(_keyboard->isKeyDown(OIS::KC_W)) vt += Ogre::Vector3(0, 1, 0);
  if(_keyboard->isKeyDown(OIS::KC_S)) vt += Ogre::Vector3(0, -1, 0);
  if(_keyboard->isKeyDown(OIS::KC_D)) vt += Ogre::Vector3(1, 0, 0);
  if(_keyboard->isKeyDown(OIS::KC_P)) _init->show();

  _camera->moveRelative(vt * deltaT * tSpeed);

  //Mover ratón
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

  return true;
}

bool MyFrameListener::keyPressed(const OIS::KeyEvent& evt)
{
  if(evt.key==OIS::KC_ESCAPE) return _quit=true;
 
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(evt.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(evt.text);

  return true;
}

bool MyFrameListener::keyReleased(const OIS::KeyEvent& evt)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(evt.key));
  return true;
}

bool MyFrameListener::mouseMoved(const OIS::MouseEvent& evt)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
  return true;
}

bool MyFrameListener::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertMouseButton(id));
  return true;
}

bool MyFrameListener::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertMouseButton(id));
  return true;
}

CEGUI::MouseButton MyFrameListener::convertMouseButton(OIS::MouseButtonID id)
{
  CEGUI::MouseButton ceguiId;
  switch(id)
    {
    case OIS::MB_Left:
      ceguiId = CEGUI::LeftButton;
      break;
    case OIS::MB_Right:
      ceguiId = CEGUI::RightButton;
      break;
    case OIS::MB_Middle:
      ceguiId = CEGUI::MiddleButton;
      break;
    default:
      ceguiId = CEGUI::LeftButton;
    }
  return ceguiId;
}

void MyFrameListener::setSheet(CEGUI::Window* sheet)
{
  _sheet = sheet;
}

void MyFrameListener::setMenuInit(CEGUI::Window* init)
{
  _init = init;
}

void MyFrameListener::moveCamera(int posX, int posY, int posZ)
{
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = 0.1;
  Ogre::Vector3 vt(0, 0, 0);
  vt += Ogre::Vector3(posX, posY, posZ);
  _camera->moveRelative(vt * deltaT * tSpeed);
}

bool MyFrameListener::quit(const CEGUI::EventArgs &e)
{
  _quit = true;
  return true;
}

bool MyFrameListener::newGame(const CEGUI::EventArgs &e)
{
  CEGUI::Window* playerWin;
  if(_player == NULL){
    playerWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("game.layout");
    _player = playerWin;
    _init->hide();
    _sheet->addChild(playerWin);

  }else{
    _init->hide();
    _player->show();
  }
  return true;
}

bool MyFrameListener::showCredits(const CEGUI::EventArgs &e)
{
  CEGUI::Window* creditsWin;
  if(_credits == NULL){
    creditsWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("credits.layout");
    _credits = creditsWin;
    _init->hide();
    moveCamera(0, 2, 0);
    _sheet->addChild(creditsWin);
    CEGUI::Window* backButton = creditsWin->getChild("BackButton");
    backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::hideCredits, this));

  }else{
    _init->hide();
    moveCamera(0, 2, 0);
    _credits->show();
  }
  return true;
}

bool MyFrameListener::hideCredits(const CEGUI::EventArgs &e)
{
  _credits->hide();
    moveCamera(0, -2, 0);
  _init->show();
  return true;
}

bool MyFrameListener::showRanking(const CEGUI::EventArgs &e)
{
  CEGUI::Window* rankingWin;
  if(_ranking == NULL){
    rankingWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("ranking.layout");
    _ranking = rankingWin;
    _init->hide();
    moveCamera(0, -2, 0);
    _sheet->addChild(rankingWin);
    CEGUI::Window* backButton = rankingWin->getChild("BackButton");
    backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::hideRanking, this));

  }else{
    _init->hide();
    moveCamera(0, -2, 0);
    _ranking->show();
  }
  return true;
}

bool MyFrameListener::hideRanking(const CEGUI::EventArgs &e)
{
  _ranking->hide();
    moveCamera(0, 2, 0);
  _init->show();
  return true;
}

Ogre::Ray MyFrameListener::setRayQuery(int posx, int posy) {
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  // _raySceneQuery->setQueryMask(RED);
  return rayMouse;
}
