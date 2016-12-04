#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#define RED    0
#define BLUE   1
#define GREEN  2
#define PINK   3
#define YELLOW 4
#define WHITE  5
#define BLACK  6

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
  bool operator==(Row* r);
  bool isFull();
  std::string toString();
};
