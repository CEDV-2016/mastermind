#include "PlayState.h"
#include "PauseState.h"
#include "BallsFactory.hpp"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

PlayState::PlayState() {
  _game = NULL;
}

void
PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("MainCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  createScene();
  createGUI();

  _exitGame = false;
}

void
PlayState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void PlayState::createScene() {
  _ballsFactory = new BallsFactory(_sceneMgr);

  //Mastermind
  Ogre::Entity* ent_mastermind = _sceneMgr->createEntity("Mastermind.mesh");
  Ogre::SceneNode* node_mastermind = _sceneMgr->createSceneNode("Mastermind");
  node_mastermind->attachObject(ent_mastermind);
  node_mastermind->translate(-1, 0, 0);
  _sceneMgr->getRootSceneNode()->addChild(node_mastermind);

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

  Ogre::SceneNode* node2 = _sceneMgr->createSceneNode("table");
  Ogre::Entity* tableEnt = _sceneMgr->createEntity("planeEnt", "plane1");
  tableEnt->setMaterialName("Table");
  node2->attachObject(tableEnt);

  _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  Ogre::Light* light = _sceneMgr->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1, -1, 0));
  node2->attachObject(light);

  _sceneMgr->getRootSceneNode()->addChild(node2);
}

void PlayState::createGUI()
{
    if(_game == NULL){
    //Config Window
    _game = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("game.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_game);
  } else{
    _game->show();
  }
}
