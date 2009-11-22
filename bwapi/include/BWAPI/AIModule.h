#pragma once
#include <string>
#include <BWAPI/Position.h>
namespace BWAPI
{
  class Unit;
  class Player;
  class AIModule
  {
    public:
      AIModule();
      virtual ~AIModule();
      virtual void onStart();
      virtual void onEnd(bool isWinner);
      virtual void onFrame();
      virtual bool onSendText(std::string text);
      virtual void onPlayerLeft(Player* player);
      virtual void onNukeDetect(Position target);

      virtual void onUnitCreate(Unit* unit);
      virtual void onUnitDestroy(Unit* unit);
      virtual void onUnitMorph(Unit* unit);
      virtual void onUnitShow(Unit* unit);
      virtual void onUnitHide(Unit* unit);
      virtual void onUnitRenegade(Unit* unit);
  };
}
