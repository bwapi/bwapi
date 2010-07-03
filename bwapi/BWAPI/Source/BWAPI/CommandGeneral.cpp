#include "CommandGeneral.h"
#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Game.h>
#include <BWAPI/GameImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/TechType.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandGeneral::CommandGeneral(UnitCommand command) : command(command)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandGeneral::execute()
  {
    UnitImpl* unit = (UnitImpl*)command.unit;
    UnitImpl* target = (UnitImpl*)command.target;
    Position position(command.x,command.y);
    TilePosition tileposition(command.x,command.y);
    UnitType unitType(command.extra);
    UpgradeType upgradeType(command.extra);
    TechType techType(command.extra);
    if (command.type == UnitCommandTypes::Attack_Position)
    {
      if (!unit->self->exists) return;
      if (!unit->getType().canMove()) return;
      unit->self->order  = Orders::AttackMove.getID();
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      if (target==NULL || !target->self->exists) return;
      if (!unit->_exists) return;
      if (!unit->getType().canAttack()) return;
      unit->self->order = Orders::AttackUnit.getID();
      unit->self->target = BroodwarImpl.server.getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      if (!unit->self->exists) return;
      if (unit->getType()==UnitTypes::Zerg_Drone)
        unit->self->order = Orders::DroneLand.getID();
      else if (unit->getType()==UnitTypes::Protoss_Probe)
        unit->self->order = Orders::BuildProtoss1.getID();
      else if (unit->getType()==UnitTypes::Terran_SCV)
        unit->self->order = Orders::BuildTerran.getID();
      else if (unit->getType() == UnitTypes::Terran_Command_Center ||
              unit->getType() == UnitTypes::Terran_Factory ||
              unit->getType() == UnitTypes::Terran_Starport ||
              unit->getType() == UnitTypes::Terran_Science_Facility)
        unit->self->secondaryOrder = Orders::BuildAddon.getID();
      unit->self->isConstructing = true;
      unit->self->isIdle         = false;
      unit->self->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Burrow.getID();
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandGeneral::getType()
  {
    return command.type.getID();
  }
  //----------------------------------------------------------------------------------------------------------
};
