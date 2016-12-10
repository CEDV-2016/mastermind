#pragma once

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.hpp"

class PauseState : public Ogre::Singleton<PauseState>, public GameState
{
public:
  PauseState() {}

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
  static PauseState& getSingleton ();
  static PauseState* getSingletonPtr ();

  void createGUI();

  bool back(const CEGUI::EventArgs &e);
  bool exitPause(const CEGUI::EventArgs &e);

protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  CEGUI::Window* _pause;

  bool _exitGame;
};
