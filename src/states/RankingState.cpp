#include <sstream>

#include "RankingState.hpp"

template<> RankingState* Ogre::Singleton<RankingState>::msSingleton = 0;

RankingState::RankingState() {
  _ranking = NULL;
  _rankingManager = new RankingManager;
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

    std::vector<std::string> ranking = _rankingManager->getRanking();
    std::string name, score;
    for (unsigned int i=0; i<ranking.size(); i+=2){
       name = std::string("Name") + std::to_string(i/2+1) + std::string("Text");
       score = std::string("Score") + std::to_string(i/2+1) + std::string("Text");
       CEGUI::Window* name_record = _ranking->getChild(name);
       name_record->setText(ranking[i]);
       CEGUI::Window* score_record = _ranking->getChild(score);
       score_record ->setText(ranking[i+1]);
    }
  } else{
    std::vector<std::string> ranking = _rankingManager->getRanking();
    std::string name, score;
    for (unsigned int i=0; i<ranking.size(); i+=2){
       name = std::string("Name") + std::to_string(i/2+1) + std::string("Text");
       score = std::string("Score") + std::to_string(i/2+1) + std::string("Text");
       CEGUI::Window* name_record = _ranking->getChild(name);
       name_record->setText(ranking[i]);
       CEGUI::Window* score_record = _ranking->getChild(score);
       score_record ->setText(ranking[i+1]);
    }
    _ranking->show();
  }
}

bool RankingState::back(const CEGUI::EventArgs &e)
{
  popState();
  return true;
}
