#ifndef GameOverState_H
#define GameOverState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <iostream>

#include "GameState.h"

class GameOverState : public Ogre::Singleton<GameOverState>, public GameState
{
 public:
  GameOverState() {}

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
  static GameOverState& getSingleton ();
  static GameOverState* getSingletonPtr ();

  void createGUI();

  bool finish(const CEGUI::EventArgs &e);

  void setPoints(std::string name, std::string points);

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  CEGUI::Window* _gameover, *_nameView, *_pointsView;
  std::string _name, _points;

  bool _exitGame;
};

#endif
