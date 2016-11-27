#include "BallsFactory.hpp"

BallsFactory::BallsFactory(Ogre::SceneManager* sm)
{
  _sceneManager = sm;
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
  box_name << "BoxNode_" << color_str;
  Ogre::SceneNode* node_box = _sceneManager->createSceneNode(box_name.str());
  node_box->attachObject(ent_box);
  node_box->translate(x, z, y);
  _sceneManager->getRootSceneNode()->addChild(node_box);

  //Montón de bolas
  Ogre::Entity* ent_slew = _sceneManager->createEntity("Ball_slew.mesh");
  ent_slew->setQueryFlags(color);

  //Creamos un material para cada mesh, ya que si no al cambiar el color de una se cambian todas
  Ogre::MaterialPtr mPtr = createMaterial(color);
  ent_slew->getSubEntity(0)->setMaterial(mPtr);

  slew_name << "SlewNode_" << color_str;
  Ogre::SceneNode* node_slew = _sceneManager->createSceneNode(slew_name.str());
  node_slew->attachObject(ent_slew);
  node_slew->translate(x, z, y);
  _sceneManager->getRootSceneNode()->addChild(node_slew);
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
    default:    return NULL;
  }
}

Ogre::ColourValue BallsFactory::colorToOgreColor(int color)
{
  //http://www.rapidtables.com/web/color/RGB_Color.htm
  switch (color) {
    case BLUE:  return Ogre::ColourValue(0, 0, 0.9); //RGB
    case RED:   return Ogre::ColourValue(0.9, 0, 0);
    case GREEN: return Ogre::ColourValue(0, 0.9, 0);
    case PINK:  return Ogre::ColourValue(0.9, 0, 0.9);
    case WHITE: return Ogre::ColourValue(0.9, 0.9, 0.9);
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
