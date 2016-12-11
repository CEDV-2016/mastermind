#include "CreditsState.hpp"
#include "SoundFXManager.hpp"

template<> CreditsState* Ogre::Singleton<CreditsState>::msSingleton = 0;

CreditsState::CreditsState() {
  _credits = NULL;
}


void
CreditsState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("MainCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = 0.1;
  Ogre::Vector3 vt(0, 0, 0);
  vt += Ogre::Vector3(2, 0, 0);
  _camera->moveRelative(vt * deltaT * tSpeed);
  createGUI();
  SoundFXManager::getSingletonPtr()->load("paper.wav")->play();

  _exitGame = false;
}

void
CreditsState::exit ()
{
  _credits->hide();
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = 0.1;
  Ogre::Vector3 vt(0, 0, 0);
  vt += Ogre::Vector3(-2, 0, 0);
  _camera->moveRelative(vt * deltaT * tSpeed);
}

void
CreditsState::pause ()
{
}

void
CreditsState::resume ()
{
}

bool
CreditsState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
CreditsState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
  return false;

  return true;
}

void
CreditsState::keyPressed
(const OIS::KeyEvent &e)
{
}

void
CreditsState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
CreditsState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
CreditsState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
CreditsState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

CreditsState*
CreditsState::getSingletonPtr ()
{
  return msSingleton;
}

CreditsState&
CreditsState::getSingleton ()
{
  assert(msSingleton);
  return *msSingleton;
}

void CreditsState::createGUI(){
  if(_credits == NULL){
    //Config Window
    _credits = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("credits.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_credits);
    CEGUI::Window* backButton = _credits->getChild("BackButton");
    backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CreditsState::back, this));
  } else{
    _credits->show();
  }
}

bool CreditsState::back(const CEGUI::EventArgs &e)
{
  popState();
  return true;
}
