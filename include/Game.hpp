#pragma once

#define SELECTING 1
#define MOVING    2
#define CHECKING  3
#define GAME_OVER 4

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "Row.hpp"

#define NUM_ROWS 6

struct RowResult {
  int reds;
  int whites;
  RowResult(int r, int w): reds(r), whites(w) {};
};

class Game {
private:
  int _state, _points, _currentRow;
  Row *_rows[6];
  Row *_result;
  std::string _all_colors[6] {"RED", "BLUE", "GREEN", "PINK", "YELLOW", "WHITE"};
  std::string _player;

public:
  Game(std::string name);
  ~Game ();
  int getState ();
  void setState (int state);
  void addBall(int row, int column, std::string color);
  void generateRandomResult();
  bool currentRowFull();
  RowResult checkCurrentRow();
  bool rowsLeft();
  int getCurrentRow();
  void addCurrentRow();
  std::string toString();
  void saveRankings();
  void readRankings();
  Row* getResult();
};
