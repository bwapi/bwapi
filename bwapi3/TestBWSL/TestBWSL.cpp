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
    Game::enableFlag(Flag::CompleteMapInformation);
    std::set<Player*> players = Game::getPlayers();
    foreach(Player* player,players)
    {
      printf("%d: %s [%s]\n",player->getID(),player->getName().c_str(),player->getRace().getName().c_str());
    }
  }
  void onEnd(bool isWinner){};
  void onFrame() 
  {
    std::set<Unit*> units = Game::getSelectedUnits();
    foreach(Unit* u, units)
    {
      int x=u->getPosition().x();
      int y=u->getPosition().y();
      Game::drawCircleMap(x,y,20,Colors::Green,false);
    }
    //printf("%d,%d\n",Game::enemy()->getStartLocation().x(),Game::enemy()->getStartLocation().y());
    //Game::drawBoxMap(Game::enemy()->getStartLocation().x()*32,Game::enemy()->getStartLocation().y()*32,Game::enemy()->getStartLocation().x()*32+4*32,Game::enemy()->getStartLocation().y()*32+3*32,Colors::Red,false);
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

