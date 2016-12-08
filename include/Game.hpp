#pragma once

#define SELECTING 1
#define MOVING    2
#define CHECKING  3
#define GAME_OVER 4

#include <iostream>
#include <fstream>
#include <vector>
#include "Row.hpp"

#define NUM_ROWS 6

struct Result {
  int reds;
  int whites;
  Result(int r, int w): reds(r), whites(w) {};
};

class Game {
private:
  int _state, _points, _currentRow;
  Row *_rows[6];
  Row *_result;
  const std::string _all_colors[6] {"RED", "BLUE", "GREEN", "PINK", "YELLOW", "WHITE"};
  std::string _player;

public:
  Game(std::string name);
  ~Game ();
  int getState ();
  void setState (int state);
  void addBall(int row, int column, std::string color);
  void generateRandomResult();
  bool currentRowFull();
  Result checkCurrentRow();
  bool rowsLeft();
  int getCurrentRow();
  void addCurrentRow();
  std::string toString();
  void saveRankings();
  void readRankings();
};
