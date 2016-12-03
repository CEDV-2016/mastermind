#pragma once

#define MENU_MAIN     1
#define MENU_CREDITS  2
#define MENU_RANKINGS 3
#define SELECTING     4
#define MOVING        5

#include <iostream>
#include <vector>
#include "Row.hpp"

#define NUM_ROWS 6

class Game {
private:
  int _state;
  Row *_rows[6];
  Row *_result;

public:
  Game();
  ~Game ();
  int getState ();
  void setState (int state);
  void addBall(int row, int column, int color);
  void generateRandomResult();
  bool checkRow(int row);
  bool rowsLeft();
};
