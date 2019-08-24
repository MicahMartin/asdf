#ifndef _InputManager_h
#define _InputManager_h 

#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include "observer/Observer.h"
#include "observer/Subject.h"
#include "input/VirtualController.h"

class InputManager : public Subject {

public:
  static InputManager* getInstance(){
    static InputManager instance;
    return &instance;
  };
  // runnable?
  void init();
  void update();

  void addVirtualController(VirtualController* controller);
  VirtualController* getVirtualController(int index);

  // Subject
  void addObserver(const char* observerName, Observer* observer);
  void removeObserver(const char* observerName);
  void notifyAll(const char* eventName);
  void notifyOne(const char* observerName, const char* eventName);

private:
  InputManager() = default;
  ~InputManager() = default;
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;
  InputManager(InputManager&&) = delete;
  InputManager& operator=(InputManager&&) = delete;
  std::vector<VirtualController*> controllers;
  
  // button config
  nlohmann::json configJson;

  struct ConfItem {
    /* data */
    int user;
    uint16_t inputBit;
    std::string inputString;
  };
  std::unordered_map<uint32_t, ConfItem> bConf;

  // would need to be careful about observing deleted objects since using pointers
  std::unordered_map<const char*, Observer*> observerList;
  int numOfObservers;
};


#endif /* ifndef _inputManager_h */
