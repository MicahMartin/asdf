#include "screens/MenuScreen.h"
#include "graphics/GameTexture.h"
#include <SDL2_image/SDL_image.h>

MenuScreen::MenuScreen(){
  printf("menu screen constructor\n");

  GameTexture* menuBackground = new GameTexture();
  menuBackground->loadTexture("../data/images/menuBackground.png");
  menuBackground->setDimensions(0,0, graphics->getWindowWidth(), graphics->getWindowHeight());

  GameTexture* versus = new GameTexture();
  versus->loadTexture("../data/images/versus.png");
  versus->setDimensions(200,100,240,50);

  GameTexture* config = new GameTexture();
  config->loadTexture("../data/images/config.png");
  config->setDimensions(200,240,240,50);

  addTexture(menuBackground);
  addTexture(versus);
  addTexture(config);
  screenTime = 0;
}

MenuScreen::~MenuScreen(){
}

void MenuScreen::init(){ 
}

void MenuScreen::update(){
  screenTime++;
}

void MenuScreen::draw() {
  for (auto gameTexture : textureList) {
    gameTexture->render();
  }
}



void MenuScreen::addTexture(GameTexture* gText) {
  textureList.push_back(gText);
}
void MenuScreen::removeTexture(int index) {
  textureList.erase(textureList.begin() + index);
}
