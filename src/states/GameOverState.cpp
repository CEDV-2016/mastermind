#include "IntroState.hpp"
#include "PlayState.hpp"
#include "GameOverState.hpp"

template<> GameOverState* Ogre::Singleton<GameOverState>::msSingleton = 0;

GameOverState::GameOverState() {
  _gameover = NULL;
}

void
GameOverState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("MainCamera");
  createGUI();

  _exitGame = false;
}

void
GameOverState::exit ()
{
  _gameover->hide();
}

void
GameOverState::pause ()
{
  _gameover->hide();
}

void
GameOverState::resume ()
{
}

bool
GameOverState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
GameOverState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
  return false;

  return true;
}

void
GameOverState::keyPressed
(const OIS::KeyEvent &e) {
  // Tecla p --> Estado anterior.
  if (e.key == OIS::KC_P) {
    popState();
  }
}

void
GameOverState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
GameOverState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
GameOverState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
GameOverState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

GameOverState*
GameOverState::getSingletonPtr ()
{
  return msSingleton;
}

GameOverState&
GameOverState::getSingleton ()
{
  assert(msSingleton);
  return *msSingleton;
}

void GameOverState::createGUI()
{
  if(_gameover == NULL){
    //Config Window
    _gameover = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("gameover.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_gameover);
    CEGUI::Window* restartButton = _gameover->getChild("RestartButton");
    restartButton->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&GameOverState::restartGame,
        this));
        CEGUI::Window* resumeButton = _gameover->getChild("FinishButton");
        resumeButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&GameOverState::finish,
            this));
            _nameView = _gameover->getChild("NameFinal");
            _pointsView = _gameover->getChild("PointsFinal");
            _nameView->setText(_name);
            _pointsView->setText(_points);
          } else{
            _nameView->setText(_name);
            _pointsView->setText(_points);
            _gameover->show();
          }
        }

        bool GameOverState::restartGame(const CEGUI::EventArgs &e)
        {
          popState();
          restart(IntroState::getSingletonPtr());
          return true;
        }

        bool GameOverState::finish(const CEGUI::EventArgs &e)
        {
          popState();
          restart(IntroState::getSingletonPtr());
          return true;
        }

        void GameOverState::setPoints(std::string name, std::string points)
        {
          _name = name;
          _points = points;
        }
