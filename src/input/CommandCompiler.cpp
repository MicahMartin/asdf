#include "input/CommandCompiler.h"

// each 'commandString' is a descriptor for a stack of boolean function calls
// P | ~P = ((wasPressed(LP)) || (wasReleased(LP)))
// @F & !D = ((wasPressed(F, strict = false)) && !(wasPressed(D)))
// MP & *D = ((wasPressed(MP)) && (isPressed(F)))
// DF = (wasPressed(DF))
// ~D = (wasReleased(D))

//  Forward, neutral, forward
//  "F, N, F",
//  back , neutral, back 
//  "B, N, B",
//  any down release, neutral, any down press, LP
//  "@~D, N, @D, LP",
//  any forward that doesnt include down, neutral, forward (lienent dash)
//  "@F & !D, N, F"
//  any back that doesnt include down, neutral, forward (lienent backdash)
//  "@B & !D, N, B",
//  release of down, downforward, anyforward that doesnt include down, lk or release of lk (236K)
//  "~D, DF, @F & !D, LK | ~LK",
//  release of down, downback, any back that doesnt include down, lp or release of lp (214P)
//  "~D, DB, @B & !D, LP | ~LP",
//  MP + forward IS pressed
//  "MP & *F",
//  MP + back IS pressed 
//  "MP & *B",
//
// input = N, F, B, U, D, UF, UB, DF, DB, LP, LK, MP, MK
// funcMods = ~, *, @
// unary = !
// binary = &, |
// TODO: unary and binary
//  "MP & *F",
//  "MP & *B",
//  "@F & !D, N, F"
//  "@B & !D, N, B",
//  "~D, DF, @F & !D, LK | ~LK",
//  "~D, DB, @B & !D, LP | ~LP",
//  TODO: load from file
std::vector<std::string> CommandCompiler::commandStrings = {
  "@F, N, F", // dash
  "B, N, B", // backdash
  "~D, DF, F, LK | ~LK", // 236K
  "~D, DB, B, LP | ~LP", // 214P
  "@~D, N, @D, LP | ~LP", // 22P
  "*F & *MP", // lp and mk
  "~LP", // release of lp
  "~MP", // release of mp
};

CommandCompiler::CommandCompiler() { }

CommandCompiler::~CommandCompiler(){ }

void CommandCompiler::init() {
  commandFunctionList.clear();
  for (int i = 0; i < commandStrings.size(); ++i) {
    compile(commandStrings[i].c_str());
  }
  printf("done compiling commands\n");
}

void CommandCompiler::compile(const char* inputString) {
  // consume all tokens up until a delim
  // create annonymous boolean func commandFunc
  // get function pointers to wasPressed, wasReleased, and isPressed, 
  // set function pointer to wasPressed
  // set strict param to true
  // if we match the function mod any, set the strict param to false
  // if we match the function mod *, set the function pointer to isPressed
  // if we match the function mod ~, set the function pointer to wasReleased
  // bind strict param to function pointer
  // set command func's return to the evaluation of function @ function pointer
  // push command func onto commandFuncStack
  std::vector<CommandToken> tokens = commandScanner.scan(inputString);
  std::vector<CommandFunction> commandFuncs;
  currentToken = &tokens[0];
  //for (auto i : tokens) {
  //}
  while(currentToken->type != CTOKEN_END){
    printf("TOKEN:%s\n", std::string(currentToken->start, currentToken->length).c_str());
    commandFuncs.push_back(compileNode());
    printf("node compiled\n");
  }
  commandFunctionList.push_back(commandFuncs);
}

CommandFunction CommandCompiler::compileNode(){
  // function pointer is &VirtualController::wasPressed by default
  // bool strictness is true by default
  using namespace std::placeholders;
  std::function<bool(Input, bool, int, bool)> funcPointer = std::bind(&VirtualController::wasPressedWrapper, controllerPointer, _1, _2, _3, _4);
  CommandFunction finalFunc;
  bool strictness = true;

  while(currentToken->type != CTOKEN_DELIM && currentToken->type != CTOKEN_END){
    switch (currentToken->type) {
      case CTOKEN_RELEASED: {
        funcPointer = std::bind(&VirtualController::wasReleasedWrapper, controllerPointer, _1, _2, _3, _4);
        printf("setting func pointer to wasReleasedWrapper\n");
      }
      break;
      case CTOKEN_HELD: {
        funcPointer = std::bind(&VirtualController::isPressedWrapper, controllerPointer, _1, _2, _3, _4);
        printf("setting func pointer to isPressedWrapper\n");
      }
      break;
      case CTOKEN_ANY: {
        strictness = false;
        printf("setting strictness to false\n");
      }
      break;
      case CTOKEN_NEUTRAL: {
       finalFunc = std::bind(funcPointer, NOINPUT, strictness, _1, _2);
       printf("building neutral\n");
      }
      break;
      case CTOKEN_FORWARD: {
       finalFunc = std::bind(funcPointer, RIGHT, strictness, _1, _2);
       printf("building forward\n");
      }
      break;
      case CTOKEN_BACK: {
        finalFunc = std::bind(funcPointer, LEFT, strictness, _1, _2);
        printf("building back\n");
      }
      break;
      case CTOKEN_UP: {
        finalFunc = std::bind(funcPointer, UP, strictness, _1, _2);
        printf("building up\n");
      }
      break;
      case CTOKEN_DOWN: {
        finalFunc = std::bind(funcPointer, DOWN, strictness, _1, _2);
        printf("building down\n");
      }
      break;
      case CTOKEN_UPFORWARD: {
        finalFunc = std::bind(funcPointer, UPRIGHT, strictness, _1, _2);
       printf("building upforward\n");
      }
      break;
      case CTOKEN_UPBACK: {
        finalFunc = std::bind(funcPointer, UPLEFT, strictness, _1, _2);
        printf("building upback\n");
      }
      break;
      case CTOKEN_DOWNFORWARD: {
        finalFunc = std::bind(funcPointer, DOWNRIGHT, strictness, _1, _2);
        printf("building upforward\n");
      }
      break;
      case CTOKEN_DOWNBACK: {
        finalFunc = std::bind(funcPointer, DOWNLEFT, strictness, _1, _2);
        printf("building downback\n");
      }
      break;
      case CTOKEN_LP: {
        finalFunc = std::bind(funcPointer, LP, strictness, _1, _2);
        printf("building lightpunch\n");
      }
      break;
      case CTOKEN_LK: {
        finalFunc = std::bind(funcPointer, LK, strictness, _1, _2);
        printf("building lightk\n");
      }
      break;
      case CTOKEN_MP: {
        finalFunc = std::bind(funcPointer, MP, strictness, _1, _2);
        printf("building mediumP\n");
      }
      break;
      case CTOKEN_MK: {
        finalFunc = std::bind(funcPointer, MK, strictness, _1, _2);
        printf("building mediumKick\n");
      }
      break;
      case CTOKEN_OR: {
        // finalFunc = std::bind(funcPointer, MK, strictness, _1, _2);
        currentToken++;
        finalFunc = binaryCommand(finalFunc, CTOKEN_OR);
        printf("building or\n");
      }
      break;
      case CTOKEN_AND: {
        // finalFunc = std::bind(funcPointer, MK, strictness, _1, _2);
        currentToken++;
        finalFunc = binaryCommand(finalFunc, CTOKEN_AND);
        printf("building and\n");
      }
      break;
      default:
        break;
    }
    if(currentToken->type != CTOKEN_END){
      currentToken++;
    }
  }
  if(currentToken->type != CTOKEN_END){
    currentToken++;
  }
  return finalFunc;
}

CommandFunction CommandCompiler::binaryCommand(CommandFunction currentFunc, CommandTokenType type){
  CommandFunction nextStatement = compileNode();
  CommandFunction returnFunction = [currentFunc, nextStatement, type](int index, bool faceRight) -> bool {
    if(type == CTOKEN_OR){
      return (currentFunc(index, faceRight) || nextStatement(index, faceRight));
    } else if(type == CTOKEN_AND){
      return (currentFunc(index, faceRight) && nextStatement(index, faceRight));
    } else {
      return false;
    };
  };

  return returnFunction;
}