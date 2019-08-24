#ifndef _VirtualController_h
#define _VirtualController_h 

#include "observer/Observer.h"
#include <vector>
#include <unordered_set>
#include <SDL2/SDL.h>
#include <boost/circular_buffer.hpp>

class VirtualController : public Observer {
public:

  enum Input { 
    NOINPUT = 0,

    DOWN = 0x1,
    RIGHT = 0x2,
    LEFT = 0x4,
    UP = 0x8,

    DOWNLEFT = (DOWN | LEFT),
    DOWNRIGHT = (DOWN | RIGHT),
    UPLEFT = (UP | LEFT),
    UPRIGHT = (UP | RIGHT),

    LP  = 0x10,
    MP = 0x20,
    HP = 0x40,
    AP = 0x80,
    // TODO: ANYP
    // TODO: MULTIP

    LK  = 0x100,
    MK = 0x200,
    HK = 0x400,
    AK = 0x800,
    // TODO: ANYK
    // TODO: MULTIK

    START = 0x1000,
    SELECT = 0x2000,
    MISC1 = 0x4000,
    MISC2 = 0x8000,
  };
  static Input getInputForString(std::string inputStr, bool faceRight);

  VirtualController();
  ~VirtualController();


  void setBit(uint16_t bit);
  void clearBit(uint16_t bit);

  bool wasPressed(Input input);
  bool isPressed(Input input);

  void update();

  uint16_t getState();
  uint8_t getStickState();
  void printStickState();

  void onNotify(const char* eventName);

private:
  uint16_t currentState = 0;
  boost::circular_buffer<std::vector<uint16_t>> inputHistory;
};

#endif /* ifndef _virtualController_h */
