#include "IntroState.h"
#include "PlayState.h"
#include "BallsFactory.hpp"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneManager = _root->createSceneManager(Ogre::ST_INTERIOR, "SceneManager");
  _sceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));
  _sceneManager->addRenderQueueListener(new Ogre::OverlaySystem());

  _camera = _sceneManager->createCamera("MainCamera");
  _camera->setPosition(Ogre::Vector3(0, 6, 2)); 
  _camera->lookAt(Ogre::Vector3(0, 0, 0));      
  _camera->setNearClipDistance(0.1);
  _camera->setFarClipDistance(1000);
  _camera->setFOVy(Ogre::Degree(40));

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);

  createScene();

  OIS::ParamList param;
  size_t windowHandle;
  std::ostringstream wHandleStr;

  //window->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));
  param.insert(std::make_pair("x11_mouse_hide", std::string("true"))); // Hide the OS mouse cursor

  createGUI();

  _exitGame = false;
}

void
IntroState::exit()
{
  _sceneManager->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  if(_exitGame) return false;
  return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    //changeState(PlayState::getSingletonPtr());
  }
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void IntroState::createScene() {
  _ballsFactory = new BallsFactory(_sceneManager);

  //Mastermind
  Ogre::Entity* ent_mastermind = _sceneManager->createEntity("Mastermind.mesh");
  Ogre::SceneNode* node_mastermind = _sceneManager->createSceneNode("Mastermind");
  node_mastermind->attachObject(ent_mastermind);
  node_mastermind->translate(-1, 0, 0);
  _sceneManager->getRootSceneNode()->addChild(node_mastermind);

  _ballsFactory->createBoxAndBallSlew(RED,   1, 0, 1); //X Z -Y
  _ballsFactory->createBoxAndBallSlew(BLUE,  1, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew(GREEN, 1, 0, -1);
  _ballsFactory->createBoxAndBallSlew(PINK,  2, 0, 1);
  _ballsFactory->createBoxAndBallSlew(WHITE, 2, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew(BLACK, 2, 0, -1);

  //Suelo
  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("plane1",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
  200, 200, 1, 1, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* node2 = _sceneManager->createSceneNode("table");
  Ogre::Entity* tableEnt = _sceneManager->createEntity("planeEnt", "plane1");
  tableEnt->setMaterialName("Table");
  node2->attachObject(tableEnt);

  _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  Ogre::Light* light = _sceneManager->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1, -1, 0));
  node2->attachObject(light);

  _sceneManager->getRootSceneNode()->addChild(node2);
}

void IntroState::createGUI()
{
  //CEGUI
  renderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

  CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

  // Let's make the OS and the CEGUI cursor be in the same place
  CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();  
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(-mousePos.d_x,-mousePos.d_y);

  CEGUI::FontManager::getSingleton().createAll("*.font", "Fonts");

  //Sheet
  CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

  //Config Window
  CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("init.layout");

  //Config Button
  CEGUI::Window* exitButton = formatWin->getChild("ExitButton");
  exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::quit, 
						      this));
  /*CEGUI::Window* newGameButton = formatWin->getChild("NewGameButton");
  newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&MyFrameListener::newGame, 
						      _framelistener));
  CEGUI::Window* creditsButton = formatWin->getChild("CreditsButton");
  creditsButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&MyFrameListener::showCredits, 
						      _framelistener));
  CEGUI::Window* rankingButton = formatWin->getChild("RankingButton");
  rankingButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&MyFrameListener::showRanking, 
						      _framelistener));*/

  //Attaching buttons
  sheet->addChild(formatWin);
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

bool IntroState::quit(const CEGUI::EventArgs &e)
{
  _exitGame = true;
  return true;
}
