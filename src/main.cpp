#include <iostream>

#include "GameManager.hpp"
#include "MainState.hpp"
#include "IntroState.hpp"
#include "NewGameState.hpp"
#include "PlayState.hpp"
#include "PauseState.hpp"
#include "CreditsState.hpp"
#include "RankingState.hpp"
#include "GameOverState.hpp"

using namespace std;

int main() {

  new GameManager();
  new MainState();
  new IntroState();
  new NewGameState();
  new PlayState();
  new PauseState();
  new CreditsState();
  new RankingState();
  new GameOverState();

  try
  {
    game->start(MainState::getSingletonPtr());
  }
  catch (Ogre::Exception& e)
  {
    std::cerr << "Excepción detectada: " << e.getFullDescription();
  }

  delete game;

  return 0;
}
