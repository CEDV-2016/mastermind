#include "Row.hpp"

Row::Row() {
  for (int i = 0; i < ROW_SIZE; i++){
    _balls[i] = "-1";
  }
}

Row::~Row(){}

void Row::addBall(int column, std::string color) {
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

std::string Row::getBallAt(int column) {
  return _balls[column];
}

bool Row::isFull() {
  for (int i = 0; i < ROW_SIZE; i++) {
    if (_balls[i] == "-1") return false;
  }
  return true;
}

std::string Row::toString(){
  std::stringstream stream;

  for (int i = 0; i < ROW_SIZE; i++){
    stream << _balls[i] << "  ";
  }
  return stream.str();
}
