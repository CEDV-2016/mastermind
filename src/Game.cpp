#include "Game.hpp"

Game::Game() {
  for (int i = 0; i < NUM_ROWS; i++) {
    _rows[i] = new Row();
  }
  _result = new Row();

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

void Game::addBall (int row, int column, int color) {
  _rows[row]->addBall(column, color);
}

void Game::generateRandomResult() {
  for (int i = 0; i < ROW_SIZE; i++){
    _result->addBall(i, i);
  }
}

bool Game::checkRow(int row) {
  return _result == _rows[row];
}
