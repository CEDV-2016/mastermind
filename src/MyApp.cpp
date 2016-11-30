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

  _framelistener = new MyFrameListener(window, cam, _overlayManager, _sceneManager);
  _root->addFrameListener(_framelistener);

  createGUI();

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

  //Mastermind
  Ogre::Entity* ent_mastermind = _sceneManager->createEntity("Mastermind.mesh");
  Ogre::SceneNode* node_mastermind = _sceneManager->createSceneNode("Mastermind");
  node_mastermind->attachObject(ent_mastermind);
  node_mastermind->translate(-1, 0, 0);
  _sceneManager->getRootSceneNode()->addChild(node_mastermind);

  _ballsFactory->createBoxAndBallSlew(RED,   1, 0, 1); //X Z -Y
  _ballsFactory->createBoxAndBallSlew(BLUE,  1, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew(GREEN, 1, 0, -1);
  _ballsFactory->createBoxAndBallSlew(PINK,  2, 0, 1);
  _ballsFactory->createBoxAndBallSlew(WHITE, 2, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew(BLACK, 2, 0, -1);

  //Suelo
  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
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

void MyApp::createOverlay() {
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  Ogre::Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}

void MyApp::createGUI()
{
  //CEGUI
  renderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

  CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

  // Let's make the OS and the CEGUI cursor be in the same place
  CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();  
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(-mousePos.d_x,-mousePos.d_y);

  CEGUI::FontManager::getSingleton().createAll("*.font", "Fonts");

  //Sheet
  CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

  //Config Window
  CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("init.layout");

  //Attaching buttons
  sheet->addChild(formatWin);
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}
