#pragma once

#include <Ogre.h>
#include "MyFrameListener.hpp"
#include "BallsFactory.hpp"
#include "Colors.hpp"
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>

class MyApp {

private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  CEGUI::OgreRenderer* renderer; 
  MyFrameListener* _framelistener;
  Ogre::OverlayManager* _overlayManager;
  BallsFactory *_ballsFactory;

public:
  MyApp();
  ~MyApp();
  int start();
  void loadResources();
  void createScene();
  void createOverlay();
  void createGUI();
};
