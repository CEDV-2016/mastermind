#include "BallsFactory.hpp"

BallsFactory::BallsFactory(Ogre::SceneManager* sm)
{
  _sceneManager = sm;
  _ball_counter = 0;
}

BallsFactory::~BallsFactory()
{
  delete _sceneManager;
}

void BallsFactory::createBoxAndBallSlew(int color, int x, int z, int y)
{
  std::stringstream box_name, slew_name;
  std::string color_str = colorToString(color);;

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
  ent_slew->setQueryFlags(color);
  //Creamos un material para cada entity, ya que si no al cambiar el color de una se cambian todas
  Ogre::MaterialPtr mPtr = createMaterial(color);
  ent_slew->getSubEntity(0)->setMaterial(mPtr);

  slew_name << "SlewNode_" << color_str;
  Ogre::SceneNode* node_slew = _sceneManager->createSceneNode(slew_name.str());
  node_slew->attachObject(ent_slew);
  node_slew->translate(x, z, y);
  node_slew->yaw(Ogre::Degree(color % 2 ? 180 : 0));

  _sceneManager->getRootSceneNode()->addChild(node_slew);
}

Ogre::SceneNode* BallsFactory::createBall(int color) {
  std::stringstream ball_name;
  std::string color_str = colorToString(color);;

  Ogre::Entity* ent_ball = _sceneManager->createEntity("Ball.mesh");
  ent_ball->setQueryFlags(color);
  //Creamos un material para cada entity, ya que si no al cambiar el color de una se cambian todas
  Ogre::MaterialPtr mPtr = createMaterial(color);
  ent_ball->getSubEntity(0)->setMaterial(mPtr);

  ball_name << "BallNode_" << _ball_counter;
  _ball_counter += 1;
  Ogre::SceneNode* node_ball = _sceneManager->createSceneNode(ball_name.str());
  node_ball->attachObject(ent_ball);
  _sceneManager->getRootSceneNode()->addChild(node_ball);

  return node_ball;
}

std::string BallsFactory::colorToString(int color)
{
  switch (color) {
    case BLUE:  return "BLUE";
    case RED:   return "RED";
    case GREEN: return "GREEN";
    case PINK:  return "PINK";
    case WHITE: return "WHITE";
    case BLACK: return "BLACK";
    default:    return "";
  }
}



Ogre::ColourValue BallsFactory::colorToOgreColor(int color)
{
  switch (color) {
    case BLUE:  return Ogre::ColourValue(0, 0, 0.9); //RGB
    case RED:   return Ogre::ColourValue(0.7, 0, 0);
    case GREEN: return Ogre::ColourValue(0, 0.6, 0);
    case PINK:  return Ogre::ColourValue(0.7, 0.1, 0.7);
    case WHITE: return Ogre::ColourValue(0.7, 0.7, 0.7);
    case BLACK: return Ogre::ColourValue(0.1, 0.1, 0.1);
    default:    return Ogre::ColourValue(0.9, 0, 0);
  }
}

Ogre::MaterialPtr BallsFactory::createMaterial(int color)
{
  Ogre::MaterialPtr mPtr = Ogre::MaterialManager::getSingleton().create
  (colorToString(color), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
  mPtr->setAmbient(colorToOgreColor(color));
  mPtr.getPointer()->getTechnique(0)->getPass(0)->setDiffuse(0.4, 0.4, 0.4, 0);

  return mPtr;
}
