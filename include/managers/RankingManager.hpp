#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class RankingManager
{
public:
  RankingManager () {};
  ~RankingManager () {};
  std::vector<std::string> getRanking ();
  bool checkRanking (int turns);
  void setRanking (std::string, int turns);

private:
  std::vector<std::string> _rankings;
  void saveRankings ();
  void loadRankings ();
};
