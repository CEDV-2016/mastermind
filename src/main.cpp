#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "MainState.h"
#include "IntroState.h"
#include "NewGameState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "CreditsState.h"
#include "RankingState.h"
#include "GameOverState.h"

#include <iostream>

using namespace std;

int main() {
  
  GameManager* game = new GameManager();
  MainState* mainState = new MainState();
  IntroState* introState = new IntroState();
  NewGameState* newGameState = new NewGameState();
  PlayState* playState = new PlayState();
  PauseState* pauseState = new PauseState();
  CreditsState* creditsState = new CreditsState();
  RankingState* rankingState = new RankingState();
  GameOverState* gameOverState = new GameOverState();

  UNUSED_VARIABLE(mainState);
  UNUSED_VARIABLE(introState);
  UNUSED_VARIABLE(newGameState);
  UNUSED_VARIABLE(playState);
  UNUSED_VARIABLE(pauseState);
  UNUSED_VARIABLE(creditsState);
  UNUSED_VARIABLE(rankingState);
  UNUSED_VARIABLE(gameOverState);
    
  try
    {
      // Inicializa el juego y transición al primer estado.
      game->start(MainState::getSingletonPtr());
    }
  catch (Ogre::Exception& e)
    {
      std::cerr << "Excepción detectada: " << e.getFullDescription();
    }
  
  delete game;
  
  return 0;
}
