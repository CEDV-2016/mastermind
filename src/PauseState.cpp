#include "IntroState.h"
#include "PauseState.h"

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;

void
PauseState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("MainCamera");
  createGUI();

  _exitGame = false;
}

void
PauseState::exit ()
{
}

void
PauseState::pause ()
{
}

void
PauseState::resume ()
{
}

bool
PauseState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
PauseState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
PauseState::keyPressed
(const OIS::KeyEvent &e) {
  // Tecla p --> Estado anterior.
  if (e.key == OIS::KC_P) {
    popState();
  }
}

void
PauseState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
PauseState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PauseState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
PauseState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PauseState*
PauseState::getSingletonPtr ()
{
return msSingleton;
}

PauseState&
PauseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void PauseState::createGUI()
{
  if(_pause == NULL){
    //Config Window
    _pause = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("pause.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_pause);
    CEGUI::Window* resumeButton = _pause->getChild("ResumeButton");
    resumeButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&PauseState::back, 
						      this));
    CEGUI::Window* exitButton = _pause->getChild("ExitPauseButton");
    exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&PauseState::exitPause, 
						      this));
  } else{
    _pause->show();
  }
}

bool PauseState::back(const CEGUI::EventArgs &e)
{
  _pause->hide();
  popState();
  return true;
}

bool PauseState::exitPause(const CEGUI::EventArgs &e)
{
  _exitGame = true;
  return true;
}
