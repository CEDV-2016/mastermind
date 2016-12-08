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
  _ballsFactory->createBoxAndBallSlew(RED,   1, 0, 1); //X Z -Y
  _ballsFactory->createBoxAndBallSlew(BLUE,  1, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew(GREEN, 1, 0, -1);
  _ballsFactory->createBoxAndBallSlew(PINK,  2, 0, 1);
  _ballsFactory->createBoxAndBallSlew(WHITE, 2, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew(BLACK, 2, 0, -1);
}

void PlayState::createGUI()
{
    if(_game == NULL){
    //Config Window
    _game = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("game.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_game);
    _nameView = _game->getChild("NameText");
    _pointsView = _game->getChild("PointsText");
    _nameView->setText(_name);
    _pointsView->setText("6000");
  } else{
    _nameView->setText(_name);
    _pointsView->setText("6000");
    _game->show();
  }
}

void PlayState::setName(std::string name)
{
  _name = name;
}
