#ifndef _StateDef_h
#define _StateDef_h

#include <vector>
#include <nlohmann/json.hpp>
#include "game_objects/GameObject.h"
#include "input/VirtualController.h"
#include "graphics/Animation.h"
#include "physics/CollisionBox.h"
#include "domain_language/Script.h"
#include "domain_language/VirtualMachine.h"


typedef std::vector<CollisionBox*> CollisionBoxList;

enum FlagBit {
  NO_TURN = 0x01,
  NO_TURN_ON_ENTER  = 0x02,
  TECHABLE = 0x04,
  SUPER_ATTACK = 0x08,
};


struct SoundObj {
  Mix_Chunk* sound;
  bool active = false;
  int soundID = 0;
  int channel = -1;
};

const int HITBOX_GROUP_MAX = 8;
const int COLLISION_BOX_MAX = 16;

struct StateDefObj {
  int stateTime;
  int animTime;
  int freezeFrame;
  int freezeLength;
  int animFrame;
  int animationTimePassed;
  int currentAnimElemIndex;
  int currentAnimElemTimePassed;
  bool hitboxesDisabled;
  bool canWhiffCancel;
  bool canHitCancel;
  bool counterHitFlag;
  bool hitboxGroupDisabled[HITBOX_GROUP_MAX];

  AnimationStateObj animObj;
  CollisionBoxState collisionBoxStates[COLLISION_BOX_MAX];

  // std::unordered_map<int, bool> hitboxGroupDisabled;
  // std::unordered_map<CollisionBox*, CollisionBoxState> collisionBoxStates;
};
class GameObject;
class StateDef {
public:
  StateDef();
  ~StateDef();

  // load shit
  void init(nlohmann::json::value_type json, VirtualMachine* charVm, float _animScale);
  void loadFlags(nlohmann::json::value_type json);
  void loadAnimation(nlohmann::json json);
  void loadCollisionBoxes(nlohmann::json json);
  void loadVisualEffects(nlohmann::json json);

  bool checkFlag(FlagBit bit);

  void enter();
  void handleCancels();
  void update();
  void draw(std::pair<int,int> position, bool faceRight, bool inHitStop);
  void drawCollisionBoxes();

  StateDefObj* saveState();
  void loadState(StateDefObj stateObj);

  void resetAnim();


  GameObject* owner;
  VirtualMachine* charVm;
  Script updateScript;
  Script cancelScript;
  Animation anim;
  StateDefObj stateObj;

  // TODO: Polymorph or atleast use a union
  CollisionBoxList pushBoxes;
  CollisionBoxList hurtBoxes;
  CollisionBoxList hitBoxes;
  CollisionBoxList throwHitBoxes;
  CollisionBoxList throwHurtBoxes;
  CollisionBoxList proximityBoxes;
  CollisionBoxList projectileBoxes;
  std::vector<CollisionBox*> collisionBoxes;

  std::unordered_map<int, std::vector<int>> soundIndexMap;
  std::unordered_map<int, int> visualEffectMap; // <startFrame, visualID>
  // TODO: Methods to talk to anim so this stuff can stay private
  int stateNum;
  int techState;

  int stateTime = 0;
  int animTime = 0;
  int freezeFrame = 0;
  int freezeLength = 0;
  float animScale = 1;
  bool hitboxesDisabled = false;

  bool canWhiffCancel = false;
  bool canHitCancel = false;
  bool counterHitFlag = false;
  std::unordered_map<int, bool> hitboxGroupDisabled;
  std::string charName = "";

private:
  static std::map<std::string, FlagBit> flagMap;
  uint8_t flagByte = 0;
};
#endif
