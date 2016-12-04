#pragma once

#define MENU_MAIN     1
#define MENU_CREDITS  2
#define MENU_RANKINGS 3
#define SELECTING     4
#define MOVING        5
#define GAME_OVER     6

#include <iostream>
#include <vector>
#include "Row.hpp"

#define NUM_ROWS 6

class Game {
private:
  int _state;
  Row *_rows[6];
  Row *_result;
  const std::string _all_colors[6] {"RED", "BLUE", "GREEN", "PINK", "YELLOW", "WHITE"};

public:
  Game();
  ~Game ();
  int getState ();
  void setState (int state);
  void addBall(int row, int column, std::string color);
  void generateRandomResult();
  bool checkRow(int row); //lanzar una exception si no está llena?
  bool rowsLeft();
  std::string toString();
};
