#include "Game.hpp"

Game::Game(std::string name) {
  for (int i = 0; i < NUM_ROWS; i++) {
    _rows[i] = new Row();
  }
  generateRandomResult();
  _points = 1000;
  _player = name;
  _currentRow = 0;
  _state = SELECTING;
}

Game::~Game () {
  for (int i = 0; i < NUM_ROWS; i++){
    delete _rows[i];
  }
  delete _result;
}

int Game::getState() {
  return _state;
}

void Game::setState (int state) {
  _state = state;
}

void Game::addBall (int row, int column, std::string color) {
  _rows[row]->addBall(column, color);
}

void Game::generateRandomResult() {
  _result = new Row();

  for (int i = 0; i < ROW_SIZE; i++){
    _result->addBall(i, _all_colors[i]);
  }
}

bool Game::checkCurrentRow() {
  return _rows[_currentRow]->equals(_result);
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

void Game::saveRankings() {
  std::ofstream outfile;

  outfile.open("rankings.csv", std::ios::app);
  outfile << _player << ";" << _points << "\n";
}
