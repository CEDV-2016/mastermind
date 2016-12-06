#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS/OIS.h>
#include "BallsFactory.hpp"
#include "Colors.hpp"
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>

#include "GameState.h"

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

  void loadResources();
  void createScene();
  void createOverlay();
  void createGUI();

  bool quit(const CEGUI::EventArgs &e);

 protected:
  Ogre::Root* _root;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::SceneManager* _sceneManager;
  Ogre::RaySceneQuery *_raySceneQuery;
  Ogre::SceneNode *_selectedNode;
  CEGUI::OgreRenderer* renderer; 
  BallsFactory *_ballsFactory;
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;

  bool _exitGame;
};

#endif
