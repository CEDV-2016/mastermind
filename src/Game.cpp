#include "Game.hpp"

Game::Game() {
  for (int i = 0; i < NUM_ROWS; i++) {
    _rows[i] = new Row();
    _balls_entity[i] = new Row();
  }
  generateRandomResult();
  _points = 6000;
  _currentRow = 0;
  _state = SELECTING;
}

Game::~Game () {
  for (int i = 0; i < NUM_ROWS; i++){
    delete _rows[i];
    delete _balls_entity[i];
  }
  delete _result;
}

void Game::setPlayerName(std::string) {
  _player = name;
}

int Game::getState() {
  return _state;
}

void Game::setState (int state) {
  _state = state;

  if (state == GAME_OVER) saveRankings();
}

void Game::addBall (int row, int column, std::string color) {
  _rows[row]->addBall(column, color);
}

void Game::generateRandomResult() {
  _result = new Row();

  unsigned seed = std::time(0);
  std::srand(seed);
  std::random_shuffle(&_all_colors[0], &_all_colors[sizeof(_all_colors)/sizeof(*_all_colors)]);

  for (int i = 0; i < ROW_SIZE; i++){
    _result->addBall(i, _all_colors[i]);
  }
}

RowResult Game::checkCurrentRow() {
  int reds = 0, whites = 0;

  for (int i=0; i < ROW_SIZE; i++) {
    if (_result->getBallAt(i) == _rows[_currentRow]->getBallAt(i)) reds++;
    else
      for (int j=0; j < ROW_SIZE; j++) {
        if (_result->getBallAt(i) == _rows[_currentRow]->getBallAt(j)) whites++;
      }
  }
  RowResult r(reds, whites);
  return r;
}

bool Game::currentRowFull() {
  return _rows[_currentRow]->isFull();
}

bool Game::rowsLeft() {
  for (int i = 0; i < ROW_SIZE; i++){
    if (!_rows[i]->isFull()) return false;
  }
  return true;
}

int Game::getCurrentRow() {
  return _currentRow;
}

void Game::addCurrentRow() {
  _currentRow++;

  _points -= 1000;
}

std::string Game::toString(){
  std::stringstream stream;
  std::string msg;
  stream << "----GAME TO STRING----" << "\n";
  stream << "Res = " << _result->toString() << "\n";

  for (int i = NUM_ROWS-1; i >= 0; i--) {
    msg = _currentRow == i? "<" : "";
    stream << i << ". " <<  _rows[i]->toString() << msg <<  "\n";
  }
  return stream.str();
}

Row* Game::getResult() {
  return _result;
}

std::string Game::getBallEntityNameAt(int col, int row) {
  return _balls_entity[row]->getBallAt(col);
}

void Game::addBallEntityNameAt(int col, int row, std::string name) {
  _balls_entity[row]->addBall(col, name);
}

void Game::saveRankings() {
  std::ofstream outfile;

  outfile.open("media/rankings.csv", std::ios::app);
  outfile << _player << ";" << _points << "\n";
}

void Game::readRankings() {}
