#include "RankingState.h"

template<> RankingState* Ogre::Singleton<RankingState>::msSingleton = 0;

RankingState::RankingState() {
  _ranking = NULL;
}


void
RankingState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("MainCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = 0.1;
  Ogre::Vector3 vt(0, 0, 0);
  vt += Ogre::Vector3(-4, 0, 0);
  _camera->moveRelative(vt * deltaT * tSpeed);
  createGUI();

  _exitGame = false;
}

void
RankingState::exit ()
{
  _ranking->hide();
  Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = 0.1;
  Ogre::Vector3 vt(0, 0, 0);
  vt += Ogre::Vector3(4, 0, 0);
  _camera->moveRelative(vt * deltaT * tSpeed);
}

void
RankingState::pause ()
{
}

void
RankingState::resume ()
{
}

bool
RankingState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
RankingState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
RankingState::keyPressed
(const OIS::KeyEvent &e) 
{
}

void
RankingState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
RankingState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
RankingState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
RankingState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

RankingState*
RankingState::getSingletonPtr ()
{
return msSingleton;
}

RankingState&
RankingState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void RankingState::createGUI(){
    if(_ranking == NULL){
    //Config Window
    _ranking = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("ranking.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_ranking);
    CEGUI::Window* backButton = _ranking->getChild("BackButton");
    backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&RankingState::back, this));
  } else{
    _ranking->show();
  }
}

bool RankingState::back(const CEGUI::EventArgs &e)
{
  popState();
}
