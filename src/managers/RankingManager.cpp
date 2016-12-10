#include "RankingManager.hpp"

std::vector<std::string> RankingManager::getRanking () {
  if (_rankings.size() == 0) {
    this->loadRankings();
  }
  return _rankings;
}

bool RankingManager::checkRanking (int turns) {
  if (turns < std::stoi(_rankings[9])) {
    return true;
  }
  return false;
}

void RankingManager::setRanking (std::string name, int turns) {
  int position = 5;
  int i;
  for (i = 5; i > 0; i--) {
    if (turns > stoi(_rankings[i * 2 - 1])) {
      break;
    }
    position--;
  }
  for (i = 5; i > position; i--) {
    _rankings[i * 2 - 1] = _rankings[i * 2 - 3];
    _rankings[i * 2 - 2] = _rankings[i * 2 - 4];
  }
  _rankings[position * 2 - 2] = name;
  _rankings[position * 2 - 1] = std::to_string(turns);

  saveRankings();
}

void RankingManager::saveRankings () {
  std::ofstream file ("media/rankings.txt");
  if (file.is_open()) {
    for (int i = 0; i<10; i++) {
      file << _rankings[i] << std::endl;
    }
    file.close();
  }
}

void RankingManager::loadRankings () {
  std::string line;
  std::ifstream file ("media/rankings.txt");
  if (file.is_open()) {
    while (getline(file, line)) {
      _rankings.push_back(line);
    }
    file.close();
  }
}
