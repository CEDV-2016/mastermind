#pragma once

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>
#include <iostream>

#include "Flags.hpp"
#include "GameState.hpp"
#include "BallsFactory.hpp"

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
public:
  PlayState ();

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

  void createScene();
  void createGUI();

  void setName(std::string name);
  void showResultingBalls();
  void highlightCurrentRow();
  Ogre::Ray setRayQuery(int posx, int posy, Ogre::uint32 mask);

protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  CEGUI::OgreRenderer* renderer;
  CEGUI::Window* _gameGUI, *_nameView, *_pointsView;
  BallsFactory *_ballsFactory;
  Ogre::SceneNode* _current_ball;
  Game * _game;

  bool _exitGame, _left_click;
};
