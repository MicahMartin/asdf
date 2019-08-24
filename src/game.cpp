#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "Game.h"

Game::Game(){}
Game::~Game(){}

void Game::init() {
  // init Graphics
  graphics.init(640,480);
  // register with input manager so we can catch quit messages
  inputManager->init();
  inputManager->addObserver("game", this);
  // add the initial virtual controller
  inputManager->addVirtualController(&virtualControllers[0]);
  inputManager->addVirtualController(&virtualControllers[1]);
  // set the state to the title screen
  stateManager->pushState(new OpeningState());
}

void Game::update() {
  ++gameTime;
  // read input event stack for this frame and send to virtual controller(s)
  inputManager->update();

  // pass input to currentState. side effects inbound
  GameState* currentState = stateManager->getState();
  // this method modifies state stack
  currentState->handleInput();
  currentState->update();

  // the current state holds a pointer to the currrent screen
  // screen has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  graphics.clear();
  currentState->draw();
  graphics.present();
}

void Game::onNotify(const char* eventName) {


  printf("Game: handling event - %s\n", eventName);

  // handle quit request
  // def need to make eventName a  bit flag enum so I can switch on it
  // strcmp returns 0 on true, dumb
  if(std::strcmp(eventName, "QUIT_REQUEST") == 0){
    printf("Shutting down\n");
    running = false;
    // printf("Here is the input history size %d\n", inputManager.getInputHistorySize());

  }else{
    printf("Not quitting\n");
  }
}

bool Game::stillRunning() {
  return running;
}
