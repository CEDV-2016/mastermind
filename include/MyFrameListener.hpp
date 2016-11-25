#include <Ogre.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS/OIS.h>

class MyFrameListener : public Ogre::FrameListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  Ogre::Camera* _camera;
  Ogre::OverlayManager* _overlayManager;
  Ogre::RenderWindow* _win;

public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);
};
