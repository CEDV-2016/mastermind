#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#define ROW_SIZE 4

class Row
{
private:
  std::string _balls[ROW_SIZE];

public:
  Row();
  ~Row();
  void addBall(int column, std::string color);
  std::string getBallAt(int column);
  bool equals(Row* r);
  bool isFull();
  std::string toString();
};
