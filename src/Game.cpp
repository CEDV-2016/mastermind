#include "Game.hpp"

Game::Game() {
  for (int i = 0; i < NUM_ROWS; i++) {
    _rows[i] = new Row();
  }
  generateRandomResult();
  _state = SELECTING; //MAIN_MENU
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

bool Game::checkRow(int row) {
  return _result == _rows[row];
}

bool Game::rowsLeft() {
  for (int i = 0; i < ROW_SIZE; i++){
    if (!_rows[i]->isFull()) return false;
  }
  return true;
}

std::string Game::toString(){
  std::stringstream stream;

  stream << "----GAME TO STRING----" << "\n";
  stream << "Res = " << _result->toString() << "\n";

  for (int i = NUM_ROWS-1; i >= 0; i--) {
    stream << i << ". " <<  _rows[i]->toString() << "\n";
  }
  return stream.str();
}
