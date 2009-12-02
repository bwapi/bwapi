#include <stdio.h>
#include <tchar.h>

#include <BWAgent\all.h>

#include "windows.h"

using namespace BWAgent;

class ConsoleModule : public AIModule
{
public:
  ~ConsoleModule(){};
  void onStart() {};
  void onEnd(bool isWinner) {};
  void onFrame() {};
  bool onSendText(std::string text) {return false;};
//  void onPlayerLeft(Player* player) {};
//  void onNukeDetect(Position target) {};

  void onUnitCreate(Unit* unit) {};
  void onUnitDestroy(Unit* unit) {};
  void onUnitMorph(Unit* unit) {};
  void onUnitShow(Unit* unit) {};
  void onUnitHide(Unit* unit) {};
  void onUnitRenegade(Unit* unit) {};
};

int _tmain(int argc, _TCHAR* argv[])
{
  ConsoleModule consoleModule;
  if(!BWAgent::connect(&consoleModule))
  {
    printf("could not connect: %s\n", BWAgent::getLastError());
    system("pause");
    return 1;
  }
  printf("connected\n");
  system("pause");
  return 0;
}

