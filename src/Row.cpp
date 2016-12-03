#include "Row.hpp"

Row::Row() {
  for (int i = 0; i < ROW_SIZE; i++){
    _balls[i] = -1;
  }
}

Row::~Row(){}

void Row::addBall(int column, int color) {
  _balls[column] = color;
}

bool Row::operator==(Row* r)
{
  for (int i = 0; i < ROW_SIZE; i++) {
    if (this->_balls[i] != r->getBallAt(i)) {
      return false;
    }
  }
  return true;
}

int Row::getBallAt(int column) {
  return _balls[column];
}
