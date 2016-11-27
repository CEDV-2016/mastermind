#pragma once

#include <Ogre.h>
#include "MyFrameListener.hpp"
#include "Colors.hpp"

class MyApp {

private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  MyFrameListener* _framelistener;
  Ogre::OverlayManager* _overlayManager;

public:
  MyApp();
  ~MyApp();
  int start();
  void loadResources();
  void createScene();
  void createOverlay();
};
