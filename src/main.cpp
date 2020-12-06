#include <SDL2/SDL.h>
#include <thread>
#include <chrono>
#include "game.h"
#include "ggponet.h"

void ggpoUpdate(Game* game){
  GameState* currentState = game->stateManager->getState();
  if(std::strcmp(currentState->stateName, "FIGHT_STATE") == 0){
    FightState* fightState = (FightState*)currentState;
    if (fightState->netPlayState) {
      // ggpo_idle(fightState->ggpo, foobar);
    }
  }
}

int main(int argc, char* args[]) {


  // Game.init
  Game game;
  std::string p1Char = "samurai";
  std::string p2Char = "alucard";

  if (argc >= 1) {
    game.stateManager->getInstance()->setPnum(std::stoi(args[1]));
    std::string realWindowName = game.graphics->windowName + std::to_string(std::stoi(args[1]));
    SDL_SetWindowTitle(game.graphics->getWindow(), realWindowName.c_str());
    if (argc >= 3) {
      if (std::stoi(args[2]) + std::stoi(args[3]) == 0) {
        game.graphics->resizeWindow(true);
      } else {
        game.graphics->resizeWindow(std::stoi(args[2]), std::stoi(args[3]));
      }
    }
    if (argc >= 4) {
      const char* p1CharArg = args[4];
      const char* p2CharArg = args[5];
      printf("chars %s %s\n", p1CharArg, p2CharArg);

      p1Char = p1CharArg;
      p2Char = p2CharArg;

    }
  }
  game.stateManager->getInstance()->setCharName(1, p1Char);
  game.stateManager->getInstance()->setCharName(2, p2Char);

  //mainloop
  {
    using namespace std::chrono;
    using FPS = duration<int, std::ratio<1, 60>>;
    auto nextFrame = system_clock::now() + FPS{1};
    auto prevSec = time_point_cast<seconds>(system_clock::now());
    int fpsCounter = 0;

    while(game.running) {
      game.update();
      ggpoUpdate(&game);
      game.draw();

      ++fpsCounter;
      auto currentSec = time_point_cast<seconds>(system_clock::now());
      if (currentSec > prevSec) {
        game.graphics->setFPS(fpsCounter);
        fpsCounter = 0;
        prevSec = currentSec;
      }

      while(nextFrame >= system_clock::now()){
        //busyloop
      }
      nextFrame = system_clock::now() + FPS{1};
    }
  }

  printf("exiting game\n");
  SDL_Quit();
  return 0;
}
