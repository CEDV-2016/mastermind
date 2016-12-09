#include "NewGameState.hpp"
#include "PlayState.hpp"

template<> NewGameState* Ogre::Singleton<NewGameState>::msSingleton = 0;

NewGameState::NewGameState() {
  _newgame = NULL;
  _nameText = NULL;
}

void
NewGameState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneManager = _root->getSceneManager("SceneManager");
  _camera = _sceneManager->getCamera("MainCamera");
  createGUI();

  _exitGame = false;
}

void
NewGameState::exit ()
{
  _newgame->hide();
}

void
NewGameState::pause ()
{
}

void
NewGameState::resume ()
{
}

bool
NewGameState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
NewGameState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;

  return true;
}

void
NewGameState::keyPressed
(const OIS::KeyEvent &e)
{
}

void
NewGameState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
NewGameState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
NewGameState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
NewGameState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

NewGameState*
NewGameState::getSingletonPtr ()
{
return msSingleton;
}

NewGameState&
NewGameState::getSingleton ()
{
  assert(msSingleton);
  return *msSingleton;
}

void NewGameState::createGUI(){
    if(_newgame == NULL){
    //Config Window
    _newgame = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("newgame.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_newgame);
    CEGUI::Window* newGameButton = _newgame->getChild("NewGameButton");
    newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NewGameState::newGame, this));
    CEGUI::Window* backButton = _newgame->getChild("ExitButton");
    backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NewGameState::back, this));
    _nameText = _newgame->getChild("Editbox");
  } else{
    _nameText->setText("");
    _newgame->show();
  }
}

bool NewGameState::newGame(const CEGUI::EventArgs &e)
{
  PlayState* playState = PlayState::getSingletonPtr();
  playState->setName(_nameText->getText().c_str());
  changeState(playState);
  return true;
}

bool NewGameState::back(const CEGUI::EventArgs &e)
{
  popState();
  return true;
}
