#include "screens/OpeningScreen.h"
#include "graphics/GameTexture.h"
#include <SDL2_image/SDL_image.h>

OpeningScreen::OpeningScreen(){
}

OpeningScreen::~OpeningScreen(){
}

void OpeningScreen::init(){ 
  printf("init opening screen \n");

  GameTexture* background = new GameTexture();
  background->loadTexture("../data/background.jpg");
  background->setDimensions(0,0,400,400);

  GameTexture* sprite = new GameTexture();
  sprite->loadTexture("../data/megaMan.jpg");
  sprite->setDimensions(0,0,50,50);

  addTexture(background);
  addTexture(sprite);
}

void OpeningScreen::update(){
 // Fill screen with purple 
 //printf("updating the screen \n");
 // foreach in texture vector, call render
}

void OpeningScreen::draw() {
  for (auto gameTexture : textureList) {
    gameTexture->render();
  }
}



void OpeningScreen::addTexture(GameTexture* gText) {
  textureList.push_back(gText);
}
void OpeningScreen::removeTexture(int index) {
  textureList.erase(textureList.begin() + index);
}
