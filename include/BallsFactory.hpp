#pragma once

#include <Ogre.h>

#include "Flags.hpp"

class BallsFactory
{
private:
  Ogre::SceneManager* _sceneManager;
  int _ball_counter, _black_ball_counter;
  Ogre::MaterialPtr createMaterial(std::string color);
  Ogre::ColourValue colorToOgreColor(std::string color);
  const std::string _all_colors[7] {"RED", "BLUE", "GREEN", "PINK", "YELLOW", "WHITE", "BLACK"};

public:
  BallsFactory(Ogre::SceneManager* sm);
  ~BallsFactory();
  void createBoxAndBallSlew(std::string color, int x, int z, int y);
  Ogre::SceneNode* createBall(std::string color);
  void createCheckingBalls(int row, int reds, int whites);
};
