#pragma once

#include <Ogre.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS/OIS.h>
#include "Colors.hpp"
#include "BallsFactory.hpp"

#define SELECTING_BALL 1 << 0
#define MOVING_BALL    1 << 1

class MyFrameListener : public Ogre::FrameListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  Ogre::Camera* _camera;
  Ogre::OverlayManager* _overlayManager;
  Ogre::RenderWindow* _win;
  Ogre::SceneManager* _sceneManager;
  Ogre::RaySceneQuery *_raySceneQuery;
  Ogre::SceneNode *_selectedNode;
  BallsFactory *_ballsFactory;
  int _state;
  Ogre::SceneNode* _current_ball;

  Ogre::Ray setRayQuery(int posx, int posy, int mask);

public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om, Ogre::SceneManager* sm, BallsFactory* bf);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);
};
