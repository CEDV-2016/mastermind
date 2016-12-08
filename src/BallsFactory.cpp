#include "BallsFactory.hpp"

BallsFactory::BallsFactory(Ogre::SceneManager* sm)
{
  _sceneManager = sm;
  _ball_counter = 0;
  _black_ball_counter = 0;

  for (int i = 0; i < 7; i++) createMaterial(_all_colors[i]);
}

BallsFactory::~BallsFactory()
{
  delete _sceneManager;
}

void BallsFactory::createBoxAndBallSlew(std::string color_str, int x, int z, int y)
{
  std::stringstream box_name, slew_name;

  //Caja de bolas
  Ogre::Entity* ent_box = _sceneManager->createEntity("Box.mesh");
  ent_box->setQueryFlags(BOX);
  box_name << "BoxNode_" << color_str;
  Ogre::SceneNode* node_box = _sceneManager->createSceneNode(box_name.str());
  node_box->attachObject(ent_box);
  node_box->translate(x, z, y);
  _sceneManager->getRootSceneNode()->addChild(node_box);

  //Montón de bolas
  Ogre::Entity* ent_slew = _sceneManager->createEntity("Ball_slew.mesh");
  ent_slew->setQueryFlags(SLEW);

  /*
  *Creamos un material para cada entity, ya que si no al cambiar el color de una se cambian todas.
  */
  ent_slew->getSubEntity(0)->setMaterialName(color_str);

  slew_name << "SlewNode_" << color_str;
  Ogre::SceneNode* node_slew = _sceneManager->createSceneNode(slew_name.str());
  node_slew->translate(x, z, y);
  node_slew->attachObject(ent_slew);
  node_slew->yaw(Ogre::Degree(color_str.length() % 2 ? 180 : 0));

  _sceneManager->getRootSceneNode()->addChild(node_slew);
}

Ogre::SceneNode* BallsFactory::createBall(std::string color) {
  std::stringstream ball_name;
  int counter;

  if (color == "BLACK") {
    counter = _black_ball_counter;
    _black_ball_counter++;
  } else {
    counter = _ball_counter;
    _ball_counter++;
  }

  ball_name << "BallNode_" << counter << "_" << color;
  Ogre::Entity* ent_ball = _sceneManager->createEntity(ball_name.str(), "Ball.mesh");

  ent_ball->setQueryFlags(BALL);
  ent_ball->getSubEntity(0)->setMaterialName(color);

  Ogre::SceneNode* node_ball = _sceneManager->createSceneNode(ball_name.str());
  node_ball->attachObject(ent_ball);
  _sceneManager->getRootSceneNode()->addChild(node_ball);

  return node_ball;
}

void BallsFactory::createCheckingBalls(int row, int reds, int whites) {
  double next_x = -0.34;
  double next_y = 1.03;
  double next_z = 1.13 - row*0.385;
  int placed_balls = 0;

  Ogre::SceneNode* ball;
  for (int i=0; i<reds; i++) {
    ball = createBall("RED");
    ball->scale(Ogre::Vector3(0.5, 0.5, 0.5));
    ball->setPosition(next_x, next_y, next_z);
    placed_balls++;

    switch(placed_balls){
      case 1:
      case 3: next_x = -0.24; break;

      case 0:
      case 2: next_x = -0.34; next_z += 0.1; break;
    }
  }

  for (int i=0; i<whites; i++) {
    ball = createBall("WHITE");
    ball->scale(Ogre::Vector3(0.5, 0.5, 0.5));
    ball->setPosition(next_x, next_y, next_z);
    placed_balls++;

    switch(placed_balls){
      case 1:
      case 3: next_x = -0.24; break;

      case 0:
      case 2: next_x = -0.34; next_z += 0.1; break;
    }
  }

}

Ogre::MaterialPtr BallsFactory::createMaterial(std::string color)
{
  Ogre::MaterialPtr mPtr = Ogre::MaterialManager::getSingleton().create
  (color, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
  mPtr->setAmbient(colorToOgreColor(color));
  mPtr.getPointer()->getTechnique(0)->getPass(0)->setDiffuse(0.4, 0.4, 0.4, 0);
  mPtr.getPointer()->getTechnique(0)->getPass(0)->setSpecular(0.4, 0.4, 0.4, 0);

  return mPtr;
}

Ogre::ColourValue BallsFactory::colorToOgreColor(std::string color)
{
  if (color == "BLUE")  return Ogre::ColourValue(0, 0, 0.9);
  if (color == "RED")   return Ogre::ColourValue(0.7, 0, 0);
  if (color == "GREEN") return Ogre::ColourValue(0, 0.6, 0);
  if (color == "PINK")  return Ogre::ColourValue(0.7, 0.1, 0.7);
  if (color == "WHITE") return Ogre::ColourValue(0.7, 0.7, 0.7);
  if (color == "YELLOW") return Ogre::ColourValue(0.7, 0.7, 0.1);
  if (color == "BLACK") return Ogre::ColourValue(0.1, 0.1, 0.1);

  return Ogre::ColourValue(1, 1, 1);
}
