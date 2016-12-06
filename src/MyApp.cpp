#include "MyApp.hpp"
#include "BallsFactory.hpp"

MyApp::MyApp() {
  _sceneManager = NULL;
  _framelistener = NULL;
  _overlayManager = NULL;
  _sceneManager = NULL;
}

MyApp::~MyApp() {
  delete _root;
  delete _framelistener;
  delete _overlayManager;
  delete _sceneManager;
  delete _ballsFactory;
}

int MyApp::start() {
  _root = new Ogre::Root();

  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }

  Ogre::RenderWindow* window = _root->initialise(true, "Mastermind");
  _sceneManager = _root->createSceneManager(Ogre::ST_INTERIOR);
  _sceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));
  _sceneManager->addRenderQueueListener(new Ogre::OverlaySystem());

  Ogre::Camera* cam = _sceneManager->createCamera("MainCamera");
  cam->setPosition(Ogre::Vector3(0, 6, 2)); //obtenidas de las coordenadas de la camara en blender
  cam->lookAt(Ogre::Vector3(0, 0, 0));      //obtenido del eje vacío al que sigue la cámara en blender
  cam->setNearClipDistance(0.1);
  cam->setFarClipDistance(1000);
  cam->setFOVy(Ogre::Degree(40));

  Ogre::Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);

  loadResources();
  createScene();
  createOverlay();

  _framelistener = new MyFrameListener(window, cam, _overlayManager, _sceneManager, _ballsFactory);
  _root->addFrameListener(_framelistener);


  _root->startRendering();
  return 0;
}

void MyApp::loadResources() {
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");

  Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
  Ogre::String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;
      datastr = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(datastr, typestr, sectionstr);
    }
  }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {
  _ballsFactory = new BallsFactory(_sceneManager);

  /*
  * Mastermind
  */
  Ogre::Entity* ent_mastermind = _sceneManager->createEntity("Mastermind", "Mastermind.mesh");
  ent_mastermind->setQueryFlags(BOARD);
  Ogre::SceneNode* node_mastermind = _sceneManager->createSceneNode("Mastermind");
  node_mastermind->attachObject(ent_mastermind);
  node_mastermind->translate(-1, 0, 0);
  _sceneManager->getRootSceneNode()->addChild(node_mastermind);

  for (unsigned int i=0; i<ent_mastermind->getNumSubEntities(); i++) {
    Ogre::SubEntity *aux = ent_mastermind->getSubEntity(i);
    if (aux->getMaterialName() == "Row_0")
      aux->setMaterialName("Material.alfa");
  }

  /*
  * Button
  */
  Ogre::Entity* ent_button = _sceneManager->createEntity("Button.mesh");
  ent_button->setQueryFlags(BUTTON);
  Ogre::SceneNode* node_button = _sceneManager->createSceneNode("Button");
  node_button->attachObject(ent_button);
  node_button->translate(-0.292, 1.086, -1.179);
  _sceneManager->getRootSceneNode()->addChild(node_button);


  /*
  * Boxes and ball slews
  */
  _ballsFactory->createBoxAndBallSlew("RED",   1, 0, 1); //X Z -Y
  _ballsFactory->createBoxAndBallSlew("BLUE",  1, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew("GREEN", 1, 0, -1);
  _ballsFactory->createBoxAndBallSlew("WHITE", 2, 0, 1);
  _ballsFactory->createBoxAndBallSlew("PINK",  2, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew("YELLOW", 2, 0, -1);

  /*
  * Ground
  */
  Ogre::Entity* ent_ground = _sceneManager->createEntity("Ground.mesh");
  ent_ground->setQueryFlags(GROUND);
  Ogre::SceneNode* node_ground = _sceneManager->createSceneNode("Ground");
  node_ground->attachObject(ent_ground);
  _sceneManager->getRootSceneNode()->addChild(node_ground);

  /*
  * Tiles
  */
  Ogre::Entity* ent_tile;
  Ogre::SceneNode* node_tile;

  for (int row=0; row < NUM_ROWS; row++) {
    for (int col=0; col < ROW_SIZE; col++) {
      std::stringstream tile_name;
      ent_tile = _sceneManager->createEntity("Tile.mesh");
      ent_tile->setQueryFlags(TILE);
      tile_name << "Tile_" << row << col;
      node_tile = _sceneManager->createSceneNode(tile_name.str());
      node_tile->attachObject(ent_tile);
      node_tile->translate(-1.62 + (double)col/3.3, 1.05, 1.19 - (double)row/2.6);
      _sceneManager->getRootSceneNode()->addChild(node_tile);
    }
  }

  /*
  * Black balls
  */
  Ogre::SceneNode* black_ball_node;
  for (int col=0; col < ROW_SIZE; col++) {
    black_ball_node = _ballsFactory->createBall("BLACK");
    black_ball_node->setPosition(-1.62 + (double)col/3.3, 1.1, -1.15);
  }

  /*
  * Light
  */
  _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  Ogre::Light* light = _sceneManager->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1, -1, 0));
  Ogre::SceneNode* node_light = _sceneManager->createSceneNode("LightNode");
  node_light->attachObject(light);
  _sceneManager->getRootSceneNode()->addChild(node_light);
}

void MyApp::createOverlay() {
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  Ogre::Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}
