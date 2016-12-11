#include "IntroState.hpp"
#include "NewGameState.hpp"
#include "PlayState.hpp"
#include "CreditsState.hpp"
#include "RankingState.hpp"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneManager = _root->getSceneManager("SceneManager");
  _camera = _sceneManager->getCamera("MainCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  createScene();
  createGUI();

  _exitGame = false;
}

void
IntroState::exit()
{
  _intro->hide();
  _sceneManager->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
  _intro->hide();
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
  /* Mastermind */
  Ogre::Entity* ent_mastermind = _sceneManager->createEntity("Mastermind", "Mastermind.mesh");
  ent_mastermind->setQueryFlags(BOARD);
  Ogre::SceneNode* node_mastermind = _sceneManager->createSceneNode("Mastermind");
  node_mastermind->attachObject(ent_mastermind);
  node_mastermind->translate(-1, 0, 0);
  _sceneManager->getRootSceneNode()->addChild(node_mastermind);

  /* Button */
  Ogre::Entity* ent_button = _sceneManager->createEntity("Button.mesh");
  ent_button->setQueryFlags(BUTTON);
  Ogre::SceneNode* node_button = _sceneManager->createSceneNode("Button");
  node_button->attachObject(ent_button);
  node_button->translate(-0.292, 1.086, -1.179);
  _sceneManager->getRootSceneNode()->addChild(node_button);

  /* Ground */
  Ogre::Entity* ent_ground = _sceneManager->createEntity("Ground.mesh");
  ent_ground->setQueryFlags(GROUND);
  Ogre::SceneNode* node_ground = _sceneManager->createSceneNode("Ground");
  node_ground->attachObject(ent_ground);
  _sceneManager->getRootSceneNode()->addChild(node_ground);


  /* Light */
  _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  Ogre::Light* light = _sceneManager->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1, -1, 0));
  Ogre::SceneNode* node_light = _sceneManager->createSceneNode("LightNode");
  node_light->attachObject(light);
  _sceneManager->getRootSceneNode()->addChild(node_light);
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
  pushState(NewGameState::getSingletonPtr());
  return true;
}

bool IntroState::navigateToCredits(const CEGUI::EventArgs &e)
{
  pushState(CreditsState::getSingletonPtr());
  return true;
}

bool IntroState::navigateToRanking(const CEGUI::EventArgs &e)
{
  pushState(RankingState::getSingletonPtr());
  return true;
}

bool IntroState::quit(const CEGUI::EventArgs &e)
{
  _exitGame = true;
  return true;
}
