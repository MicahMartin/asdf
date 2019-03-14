#include "screens/FightScreen.h"

FightScreen::FightScreen(){
  printf("the address of the screen %p\n", this);
};
FightScreen::~FightScreen(){};

void FightScreen::init(){};
void FightScreen::update(){};

void FightScreen::draw(){
  for (auto gameTexture : textureList) {
    gameTexture->render();
  }
};

void FightScreen::addTexture(GameTexture* gText){
  Screen::addTexture(gText);
};
void FightScreen::removeTexture(int index){
  Screen::removeTexture(index);
};
