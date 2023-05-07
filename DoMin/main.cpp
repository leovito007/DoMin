#define SDL_MAIN_HANDLED

#include "game.h"

int main() {
  Renderer renderer(DEFINE::windowWidth, DEFINE::windowHeight);
  
  Game game;
  game.start( renderer);

  return 0;
}