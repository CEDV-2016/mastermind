#pragma once

#include <Ogre.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS/OIS.h>
#include "Flags.hpp"
#include "BallsFactory.hpp"
#include "Game.hpp"

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
  Ogre::SceneNode* _current_ball;
  Game * _game;


  Ogre::Ray setRayQuery(int posx, int posy, int mask);

public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om, Ogre::SceneManager* sm, BallsFactory* bf);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);
};
