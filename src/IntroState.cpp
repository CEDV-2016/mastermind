#include "IntroState.h"
#include "NewGameState.h"
#include "PlayState.h"
#include "CreditsState.h"
#include "RankingState.h"

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
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);

  createScene();
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
  _intro->show();
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
  //Mastermind
  Ogre::Entity* ent_mastermind = _sceneManager->createEntity("Mastermind.mesh");
  Ogre::SceneNode* node_mastermind = _sceneManager->createSceneNode("Mastermind");
  node_mastermind->attachObject(ent_mastermind);
  node_mastermind->translate(-1, 0, 0);
  _sceneManager->getRootSceneNode()->addChild(node_mastermind);

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
  if(_intro == NULL){
    //Config Window
    _intro = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("init.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_intro);

    //Config Button
    CEGUI::Window* newGameButton = _intro->getChild("NewGameButton");
    newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::newGame, 
						      this));
    CEGUI::Window* creditsButton = _intro->getChild("CreditsButton");
    creditsButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::navigateToCredits, 
						      this));
    CEGUI::Window* rankingButton = _intro->getChild("RankingButton");
    rankingButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::navigateToRanking, 
						      this));
    CEGUI::Window* exitButton = _intro->getChild("ExitButton");
    exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::quit, 
						      this));
  } else{
    _intro->show();
  }
}

bool IntroState::newGame(const CEGUI::EventArgs &e)
{
  _intro->hide();
  pushState(NewGameState::getSingletonPtr());
  return true;
}

bool IntroState::navigateToCredits(const CEGUI::EventArgs &e)
{
  _intro->hide();
  pushState(CreditsState::getSingletonPtr());
  return true;
}

bool IntroState::navigateToRanking(const CEGUI::EventArgs &e)
{
  _intro->hide();
  pushState(RankingState::getSingletonPtr());
  return true;
}

bool IntroState::quit(const CEGUI::EventArgs &e)
{
  _exitGame = true;
  return true;
}
