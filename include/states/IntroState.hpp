#pragma once

#include <Ogre.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "GameState.hpp"
#include "Track.hpp"
#include "TrackManager.hpp"


class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
public:
  IntroState() {}

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
  static IntroState& getSingleton ();
  static IntroState* getSingletonPtr ();

  void createScene();
  void createGUI();

  bool newGame(const CEGUI::EventArgs &e);
  bool navigateToCredits(const CEGUI::EventArgs &e);
  bool navigateToRanking(const CEGUI::EventArgs &e);
  bool quit(const CEGUI::EventArgs &e);
  bool initSDL();

protected:
  Ogre::Root* _root;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::SceneManager* _sceneManager;
  Ogre::RaySceneQuery *_raySceneQuery;
  Ogre::SceneNode *_selectedNode;
  CEGUI::OgreRenderer* renderer;
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  CEGUI::Window* _intro;
  TrackManager* _trackManager;
  TrackPtr _mainTrack;

  bool _exitGame;
};
