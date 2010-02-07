#include <stdio.h>
#include <tchar.h>

#include <BWSL\all.h>

#include <windows.h>

#include <string>

using namespace BWSL;

class ConsoleAIModule : public AIModule
{
public:
  ~ConsoleAIModule(){};
  void onStart()
  {
    printf("match started\n");
  }
  void onEnd(bool isWinner){};
  void onFrame(){};
  bool onSendText(std::string text)
  {
    printf("text intercepted: %s\n", text.c_str());
    return true;
  }
  void onPlayerLeft(Player* player){};
  void onNukeDetect(Position target){};

  void onUnitCreate(Unit* unit){};
  void onUnitDestroy(Unit* unit){};
  void onUnitMorph(Unit* unit){};
  void onUnitShow(Unit* unit){};
  void onUnitHide(Unit* unit){};
  void onUnitRenegade(Unit* unit){};
};

int _tmain(int argc, _TCHAR* argv[])
{
  if(!connect())
  {
    printf("trying to connect...\n");
    while(!connect())
    {
      Sleep(1000);
    }
  }
  printf("connected\n");

  ConsoleAIModule myAI;
  if(!takeover(myAI))
  {
    printf("takeover failed: %s\n", getLastError().toString().c_str());
  }

  system("pause");
  return 0;
}

