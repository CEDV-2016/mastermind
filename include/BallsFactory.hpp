#pragma once

#include <Ogre.h>
#include "Colors.hpp"

class BallsFactory
{
private:
  Ogre::SceneManager* _sceneManager;
  std::string colorToString(int color);
  Ogre::ColourValue colorToOgreColor(int color);
  Ogre::MaterialPtr createMaterial(int color);

public:
  BallsFactory(Ogre::SceneManager* sm);
  ~BallsFactory();
  void createBoxAndBallSlew(int color, int x, int z, int y);
};
