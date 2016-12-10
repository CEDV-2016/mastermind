#pragma once

#define SELECTING 1
#define MOVING    2
#define CHECKING  3
#define GAME_OVER 4

#define NUM_ROWS 6

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "Row.hpp"

struct RowResult
{
  int reds;
  int whites;
  RowResult(int r, int w): reds(r), whites(w) {};
};

class Game
{
private:
  int _state, _points, _currentRow;
  Row *_rows[6];
  Row *_result;
  std::string _all_colors[6] {"RED", "BLUE", "GREEN", "PINK", "YELLOW", "WHITE"};
  Row *_balls_entity[6];
  std::string _player;

public:
  Game();
  ~Game ();
  int getState ();
  void setPlayerName(std::string name);
  std::string getPlayerName();
  void setState (int state);
  int getPoints();
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
  std::string getBallEntityNameAt(int col, int row);
  void addBallEntityNameAt(int col, int row, std::string name);
};
