#pragma once

#include <Ogre.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include "Colors.hpp"

class MyFrameListener : public Ogre::FrameListener, OIS::KeyListener, OIS::MouseListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  CEGUI::Window* _sheet, *_init, *_credits, *_ranking, *_player;
  Ogre::Camera* _camera;
  Ogre::OverlayManager* _overlayManager;
  Ogre::RenderWindow* _win;
  Ogre::SceneManager* _sceneManager;
  Ogre::RaySceneQuery *_raySceneQuery;
  Ogre::SceneNode *_selectedNode;
  Ogre::Ray setRayQuery(int posx, int posy);
  
  bool keyPressed(const OIS::KeyEvent& evt);
  bool keyReleased(const OIS::KeyEvent& evt);
  bool mouseMoved(const OIS::MouseEvent& evt);
  bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

  bool _quit;
  float _timeSinceLastFrame;

  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

  void moveCamera(int posX, int posY, int posZ);

public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::OverlayManager *om, Ogre::SceneManager* sm);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);
  void setSheet(CEGUI::Window* sheet);
  void setMenuInit(CEGUI::Window* init);
  bool quit(const CEGUI::EventArgs &e);
  bool newGame(const CEGUI::EventArgs &e);
  bool showCredits(const CEGUI::EventArgs &e);
  bool hideCredits(const CEGUI::EventArgs &e);
  bool showRanking(const CEGUI::EventArgs &e);
  bool hideRanking(const CEGUI::EventArgs &e);
};
