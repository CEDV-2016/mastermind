#pragma once

#include <vector>
#include <iostream>

#define ROW_SIZE 4
class Row
{
private:
  int _balls[ROW_SIZE];

public:
  Row();
  ~Row();
  void addBall(int column, int color);
  int getBallAt(int column);
  bool operator==(Row* r);
};
