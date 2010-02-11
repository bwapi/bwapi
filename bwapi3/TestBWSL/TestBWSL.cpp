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
    Game::enableFlag(Flag::UserInput);
    std::set<Player*> players = Game::getPlayers();
    foreach(Player* player,players)
    {
      printf("%d: %s [%s]\n",player->getID(),player->getName().c_str(),player->getRace().getName().c_str());
    }
  }
  void onEnd(bool isWinner){};
  void onFrame() 
  {
    printf("m=%d, g=%d\n",Game::self()->minerals(),Game::self()->gas());
  }
  bool onSendText(std::string text)
  {
    Game::sendText("You typed: '%s'\n", text.c_str());
    return true;
  }
  void onPlayerLeft(Player* player)
  {
    //printf("onPlayerLeft: %s (%d)",player->getName().c_str(),player->getID());
  }
  void onNukeDetect(Position target)
  {
    //printf("onNukeDetect: (%d,%d)",target.x(),target.y());
  }

  void onUnitCreate(Unit* unit)
  {
    printf("onUnitCreate: %x",unit);
  }
  void onUnitDestroy(Unit* unit)
  {
    printf("onUnitDestroy: %x",unit);
  }
  void onUnitMorph(Unit* unit)
  {
    printf("onUnitMorph: %x",unit);
  }
  void onUnitShow(Unit* unit)
  {
    printf("onUnitShow: %x",unit);
  }
  void onUnitHide(Unit* unit)
  {
    printf("onUnitHide: %x",unit);
  }
  void onUnitRenegade(Unit* unit)
  {
    printf("onUnitRenegade: %x",unit);
  }
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

