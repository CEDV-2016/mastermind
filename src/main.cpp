#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "IntroState.h"

#include <iostream>

using namespace std;

int main() {
  
  GameManager* game = new GameManager();
  IntroState* introState = new IntroState();

  UNUSED_VARIABLE(introState);
    
  try
    {
      // Inicializa el juego y transición al primer estado.
      game->start(IntroState::getSingletonPtr());
    }
  catch (Ogre::Exception& e)
    {
      std::cerr << "Excepción detectada: " << e.getFullDescription();
    }
  
  delete game;
  
  return 0;
}
