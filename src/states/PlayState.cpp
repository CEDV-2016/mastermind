#include "PlayState.hpp"
#include "PauseState.hpp"
#include "GameManager.hpp"
#include "BallsFactory.hpp"
#include "GameOverState.hpp"
#include "SoundFX.hpp"
#include "SoundFXManager.hpp"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

PlayState::PlayState() {
  _game = new Game();
  _gameGUI = NULL;
  _selectedNode = NULL;
  _current_ball = NULL;
  _ballsFactory = NULL;
}

void
PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _rankingManager = new RankingManager();
  if (_game != NULL) _game->restart();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("MainCamera");
  _ballsFactory = new BallsFactory(_sceneMgr);
  _raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
  createScene();
  createGUI();

  _exitGame = false;
  _left_click = false;
}

void
PlayState::exit ()
{
  _gameGUI->hide();
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
  _gameGUI->hide();
}

void
PlayState::resume()
{
  _gameGUI->show();
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{

  switch(_game->getState())
  {

    /*
    * Este estado se ejecuta cuando aun no hemos seleccionado ningun color de las
    * cajas de bolas. Al hacer click en una caja creamos una bola de ese color.
    */
    case SELECTING:
    {
      if (_left_click) {
        _selectedNode = NULL;

        setRayQuery(_mouse_x, _mouse_y, SLEW | BUTTON);
        Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator it;
        it = result.begin();

        if (it != result.end()) {
          _selectedNode = it->movable->getParentSceneNode();
        }

        if (_selectedNode != NULL) {
          switch (_selectedNode->getAttachedObject(0)->getQueryFlags()) {

            case SLEW:
            {
              std::string color;
              std::istringstream full_name(_selectedNode->getName());
              while (getline(full_name, color, '_')); //Obtenemos el último split

              _current_ball = _ballsFactory->createBall(color);

              SoundFXManager::getSingletonPtr()->load("bubble.wav")->play();
              _game->setState(MOVING);
            } break;

            case BUTTON: {
              _game->setState(CHECKING);
            } break;
          }
        }
      }
    } break;

    /*
    * Este estado se ejecuta cuando hemos seleccionado una bola. Ahora tenemos
    * que moverla a las mismas coordenadas del ratón hasta que haga click sobre
    * una cuadrícula encima del tablero.
    */
    case MOVING:
    {
      Ogre::Ray r = setRayQuery(_mouse_x, _mouse_y, -1);
      Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
      Ogre::RaySceneQueryResult::iterator it;
      Ogre::Vector3 position;
      it = result.begin();

      if (it != result.end()) {
        position = r.getPoint(it->distance);
        int y = position.y < 0.1 ? 0 : 1;
        _current_ball->setPosition(position.x, y, position.z);
      }

      /*
      * Como la bola está debajo del ratón no se puede pinchar en el tablero
      * (solo detecta bola). Mediante queries le decimos que solo mire los tiles.
      */
      if (_left_click) {
        _selectedNode = NULL;

        r = setRayQuery(_mouse_x, _mouse_y, TILE);
        result = _raySceneQuery->execute();
        it = result.begin();

        if (it != result.end()) {
          _selectedNode = it->movable->getParentSceneNode();
        }

        if (_selectedNode != NULL) {
          std::string coordinates, col, row, color;
          int int_col, int_row;
          std::istringstream tile_name(_selectedNode->getName());
          std::istringstream ball_name(_current_ball->getName());
          while (getline(tile_name, coordinates, '_'));
          while (getline(ball_name, color, '_'));

          row = coordinates.substr(0,1);
          col = coordinates.substr(1,2);
          std::istringstream(row) >> int_row;
          std::istringstream(col) >> int_col;

          if (_game->getCurrentRow() == int_row) {
            // std::cout << "Bola " << color << " en la fila = " << int_row << " columna = " << int_col << std::endl;

            /* Si ya había una bola en esa posición la borramos */
            std::string ball_entity = _game->getBallEntityNameAt(int_col, int_row);
            if (ball_entity != "-1") _sceneMgr->destroyEntity(ball_entity);
            _game->addBallEntityNameAt(int_col, int_row, _current_ball->getName());

            _game->addBall(int_row, int_col, color);
            _current_ball->setPosition(_selectedNode->getPosition());
            _game->setState(SELECTING);

            SoundFXManager::getSingletonPtr()->load("drop.wav")->play();

          }
        }
      }
    } break;

    case CHECKING:
    {
      if (!_game->currentRowFull()) /* ROW UNCOMPLETED */
      {
        std::cout << "Aún no has completado la fila!\n";
        _game->setState(SELECTING);
      }
      else
      {
        struct RowResult result = _game->checkCurrentRow();
        _ballsFactory->createCheckingBalls(_game->getCurrentRow(), result.reds, result.whites);

        if (result.reds == 4) /* WIN */
        {
          showResultingBalls();
          _game->setState(GAME_OVER);
          SoundFXManager::getSingletonPtr()->load("right.wav")->play();
        }
        else
        {

          if (_game->getCurrentRow() >= NUM_ROWS-1) /* LOOSE */
          {
            std::cout << "Has perdido!\n";
            showResultingBalls();
            _game->setState(GAME_OVER);
          }
          else /* KEEP PLAYING */
          {
            _game->addCurrentRow();
            highlightCurrentRow();
            _pointsView->setText(std::to_string(_game->getPoints()));

            _game->setState(SELECTING);
            SoundFXManager::getSingletonPtr()->load("wrong.wav")->play();
          }
        }
      }
    } break;

    case GAME_OVER:
    {
      _rankingManager->setRanking(_game->getPlayerName(), _game->getPoints());
      GameOverState* gameOverState = GameOverState::getSingletonPtr();
      gameOverState->setPoints(_game->getPlayerName(), std::to_string(_game->getPoints()));
      pushState(gameOverState);
    }
    break;
  } //switch

  _left_click = false;

  return !_exitGame;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) _exitGame = true;
  if (e.key == OIS::KC_P) pushState(PauseState::getSingletonPtr());
  if (e.key == OIS::KC_R) std::cout << _game->getResult()->toString() << std::endl;
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
  _mouse_x = e.state.X.abs;
  _mouse_y = e.state.Y.abs;
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  _left_click = true;
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PlayState*
PlayState::getSingletonPtr ()
{
  return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{
  assert(msSingleton);
  return *msSingleton;
}

void PlayState::createScene() {
  /* Boxes and ball slews */
  _ballsFactory->createBoxAndBallSlew("RED",   1, 0, 1); //X Z -Y
  _ballsFactory->createBoxAndBallSlew("BLUE",  1, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew("GREEN", 1, 0, -1);
  _ballsFactory->createBoxAndBallSlew("WHITE", 2, 0, 1);
  _ballsFactory->createBoxAndBallSlew("PINK",  2, 0, -0.5);
  _ballsFactory->createBoxAndBallSlew("YELLOW", 2, 0, -1);

  /* Tiles */
  Ogre::Entity* ent_tile;
  Ogre::SceneNode* node_tile;
  for (int row=0; row < NUM_ROWS; row++) {
    for (int col=0; col < ROW_SIZE; col++) {
      std::stringstream tile_name;
      ent_tile = _sceneMgr->createEntity("Tile.mesh");
      ent_tile->setQueryFlags(TILE);
      tile_name << "Tile_" << row << col;
      node_tile = _sceneMgr->createSceneNode(tile_name.str());
      node_tile->attachObject(ent_tile);
      node_tile->translate(-1.62 + (double)col/3.3, 1.05, 1.19 - (double)row/2.6);
      _sceneMgr->getRootSceneNode()->addChild(node_tile);
    }
  }

  /* Black balls */
  Ogre::SceneNode* black_ball_node;
  for (int col=0; col < ROW_SIZE; col++) {
    black_ball_node = _ballsFactory->createBall("BLACK");
    black_ball_node->setPosition(-1.62 + (double)col/3.3, 1.1, -1.15);
  }

  /* Highlight first row */
  Ogre::Entity* ent_mastermind = _sceneMgr->getEntity("Mastermind");
  for (unsigned int i=0; i<ent_mastermind->getNumSubEntities(); i++) {
    Ogre::SubEntity *aux = ent_mastermind->getSubEntity(i);
    if (aux->getMaterialName() == "Row_0")
    aux->setMaterialName("Material.semitransparent");
  }
}

void PlayState::createGUI()
{
  if(_gameGUI == NULL){
    //Config Window
    _gameGUI = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("game.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_gameGUI);
    _nameView = _gameGUI->getChild("NameText");
    _pointsView = _gameGUI->getChild("PointsText");
    _nameView->setText(_game->getPlayerName());
    _pointsView->setText(std::to_string(_game->getPoints()));
  } else{
    _nameView->setText(_game->getPlayerName());
    _pointsView->setText(std::to_string(_game->getPoints()));
    _gameGUI->show();
  }
}

Ogre::Ray PlayState::setRayQuery(int posx, int posy, Ogre::uint32 mask) {
  Ogre::RenderWindow* win =  GameManager::getSingletonPtr()->getRenderWindow();
  Ogre::uint32 all_masks = BOARD | GROUND | TILE | BOX | SLEW | BALL | BUTTON;

  Ogre::Ray rayMouse = _camera->getCameraToViewportRay (posx/float(win->getWidth()), posy/float(win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  _raySceneQuery->setQueryMask(mask == (Ogre::uint32)-1 ? all_masks : mask);
  return rayMouse;
}

void PlayState::showResultingBalls() {
  Ogre::Entity* ent_ball;
  for (int i=0; i<ROW_SIZE; i++) {
    std::stringstream name;
    name << "BallNode_" << i << "_BLACK";
    ent_ball = _sceneMgr->getEntity(name.str());
    if (ent_ball != NULL) ent_ball->getSubEntity(0)->setMaterialName(_game->getResult()->getBallAt(i));
  }
}

void PlayState::highlightCurrentRow(){
  Ogre::Entity* ent_mastermind = _sceneMgr->getEntity("Mastermind");
  std::stringstream row_name, row_prev_name;
  row_name << "Row_" << _game->getCurrentRow();
  row_prev_name << "Row_" << _game->getCurrentRow()-1;

  for (unsigned int i=0; i<ent_mastermind->getNumSubEntities(); i++) {
    Ogre::SubEntity *aux = ent_mastermind->getSubEntity(i);
    if (aux->getMaterialName() == "Material.semitransparent") aux->setMaterialName(row_prev_name.str());
    if (aux->getMaterialName() == row_name.str()) aux->setMaterialName("Material.semitransparent");
  }
}


void PlayState::setName(std::string name)
{
  _game->setPlayerName(name);
}
