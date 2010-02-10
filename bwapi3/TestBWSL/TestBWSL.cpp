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
  }
  void onEnd(bool isWinner){};
  void onFrame() 
  {
    Game::printf("frame: %d\n",Game::getFrameCount());
    Game::drawCircleScreen(Game::getMousePosition().x(),Game::getMousePosition().y(),20,Colors::Blue,true);
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

