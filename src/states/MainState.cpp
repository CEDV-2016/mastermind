#include "MainState.hpp"
#include "IntroState.hpp"

template<> MainState* Ogre::Singleton<MainState>::msSingleton = 0;

MainState* MainState::getSingletonPtr () {
    return msSingleton;
}

MainState& MainState::getSingleton () {
    assert (msSingleton);
    return *msSingleton;
}

void MainState::enter () {
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

  _exit = false;
}

void MainState::exit() {
    _root->getAutoCreatedWindow()->removeAllViewports();
}

void MainState::pause () {}

void MainState::resume () {}

bool MainState::frameStarted (const Ogre::FrameEvent &evt) {
    changeState(IntroState::getSingletonPtr());
    return true;
}

bool MainState::frameEnded (const Ogre::FrameEvent &evt) {
    return !_exit;
}

void MainState::keyPressed (const OIS::KeyEvent &e) {
    switch (e.key) {
        default:
            break;
    }
}

void MainState::keyReleased (const OIS::KeyEvent &e) {
    switch (e.key) {
        default:
            break;
    }
}

void MainState::mouseMoved (const OIS::MouseEvent &e) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(e.state.X.abs, e.state.Y.abs);
}

void MainState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id) {}

void MainState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id) {}
