#include <BWAPI.h>
#include <BWAPI/Client/GameImpl.h>
#include <BWAPI/Client/ForceImpl.h>
#include <BWAPI/Client/PlayerImpl.h>
#include <BWAPI/Client/UnitImpl.h>

#include "Templates.h"
#include "Command.h"

#include <limits>
#include <string>
namespace BWAPI
{
  UnitImpl::UnitImpl(int _id)
    : id(_id)
    , self(&(BWAPI::BWAPIClient.data->units[_id]))
  {
    clear();
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    if ( !canIssueCommand(command) )
      return false;

    command.unit = this;

    // If using train or morph on a hatchery, automatically switch selection to larva
    // (assuming canIssueCommand ensures that there is a larva)
    if ( (command.type == UnitCommandTypes::Train ||
          command.type == UnitCommandTypes::Morph) &&
         getType().producesLarva() && command.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva )
    {
      Unitset larvae( this->getLarva() );
      for (Unit larva : larvae)
      {
        if ( !larva->isConstructing() && larva->isCompleted() && larva->canCommand() )
        {
          command.unit = larva;
          break;
        }
      }
      if ( command.unit == this )
        return false;
    }

    BWAPIC::UnitCommand c;
    c.type      = command.type;
#pragma warning(suppress: 6011)
    c.unitIndex = command.unit->getID();
    if ( command.target )
      c.targetIndex = command.target->getID();
    else
      c.targetIndex = -1;
    c.x     = command.x;
    c.y     = command.y;
    c.extra = command.extra;
    Command(command).execute(0);
    static_cast<GameImpl*>(BroodwarPtr)->addUnitCommand(c);
    lastCommandFrame = Broodwar->getFrameCount();
    lastCommand      = command;
    return true;
  }
}
