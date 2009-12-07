#include <stdio.h>
#include <tchar.h>

#include <BWAPI2\all.h>

#include "windows.h"

using namespace BWAPI2;

class ConsoleModule : public AIModule
{
public:
  ~ConsoleModule(){};
  void onEnd(bool isWinner) {};
  void onStartMatch()
  {
    printf("on start match [%s]\n",Game::mapName().c_str());
  };
  void onFrame()
  {
    printf("on frame %d, (%d,%d)\n", BWAPI2::Game::getFrameCount(),BWAPI2::Game::getMouseX(),BWAPI2::Game::getMouseY());
    
    printf("on frame %d\n", BWAPI2::Game::isVisible((BWAPI2::Game::getMouseX()+BWAPI2::Game::getScreenX())/32,(BWAPI2::Game::getMouseY()+BWAPI2::Game::getScreenY())/32));
    printf("all units (size): %d",Game::getAllUnits().size());
/*    
    for(std::set<Unit*>::iterator i=Game::getAllUnits().begin();i!=Game::getAllUnits().end();i++)
    {
      Unit *unit = (*i);
      if (unit->getType()==64)
      {
        unit->holdPosition();
      }
    }
   */ 
  };
  void onSendText(std::string text)
  {
    printf("onSendText: '%s'\n",text.c_str());
    Game::sendText("typed: %s",text.c_str());
  };
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

  if (!BWAPI2::connect())
  {
    printf("waiting for server to start up\n");
    while(!BWAPI2::connect())
    {
      Sleep(200);
    }
  }
  printf("connected\n");
  if(!takeover(consoleModule))
  {
    printf(BWAPI2::getLastError().c_str());
  }
  system("pause");
  return 0;
}

