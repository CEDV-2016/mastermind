#include "MyApp.hpp"

MyApp::MyApp() {
  _sceneManager = NULL;
  _framelistener = NULL;
}

MyApp::~MyApp() {
  delete _root;
  delete _framelistener;
}

int MyApp::start() {
  _root = new Ogre::Root();

  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }

  Ogre::RenderWindow* window = _root->initialise(true, "Mastermind");
  _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);
  _sceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));

  Ogre::Camera* cam = _sceneManager->createCamera("MainCamera");
  cam->setPosition(Ogre::Vector3(0, 4, 7)); //obtenidas de las coordenadas de la camara en blender
  cam->lookAt(Ogre::Vector3(0, 0, 0)); //obtenido del eje vacío al que sigue la cámara en blender
  cam->setNearClipDistance(5);
  cam->setFarClipDistance(10000);

  Ogre::Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);

  loadResources();
  createScene();

  Ogre::SceneNode *node = _sceneManager->getSceneNode("BoardNode");

  _framelistener = new MyFrameListener(window, cam, node);
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
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation
      (datastr, typestr, sectionstr);
    }
  }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {
  Ogre::Entity* ent1 = _sceneManager->createEntity("Mastermind.mesh");
  Ogre::SceneNode* node1 = _sceneManager->createSceneNode("BoardNode");
  node1->attachObject(ent1);
  node1->translate(0, 0, 0);
  _sceneManager->getRootSceneNode()->addChild(node1);

  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, -5);
  Ogre::MeshManager::getSingleton().createPlane("plane1",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
  200, 200, 1, 1, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* node2 = _sceneManager->createSceneNode("table");
  Ogre::Entity* tableEnt = _sceneManager->createEntity("planeEnt", "plane1");
  tableEnt->setMaterialName("Table");
  node2->attachObject(tableEnt);

  _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  Ogre::Light* light = _sceneManager->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1, -1, 0));
  node2->attachObject(light);

  _sceneManager->getRootSceneNode()->addChild(node2);
}
