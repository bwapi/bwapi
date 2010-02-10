#pragma once
/**
 *  Keep it pure virtual, this way compilers automatically notify the user of changes in the API.
 */

#include "BWSLInterface.h"

IP_STRIP

#include "Position.h"

#include <string>

namespace BWSL
{
  class Unit;
  class Player;
  class BWSL_EXPORT AIModule
  {
    public:
      virtual ~AIModule(){};
      virtual void onStart() = 0;
      virtual void onEnd(bool isWinner) = 0;
      virtual void onFrame() = 0;
      virtual bool onSendText(std::string text) = 0;
      virtual void onPlayerLeft(Player* player) = 0;
      virtual void onNukeDetect(Position target) = 0;

      virtual void onUnitCreate(Unit* unit) = 0;
      virtual void onUnitDestroy(Unit* unit) = 0;
      virtual void onUnitMorph(Unit* unit) = 0;
      virtual void onUnitShow(Unit* unit) = 0;
      virtual void onUnitHide(Unit* unit) = 0;
      virtual void onUnitRenegade(Unit* unit) = 0;
  };
}
