#include "CommandGeneral.h"
#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Game.h>
#include <BWAPI/GameImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/PlayerImpl.h>
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
      if (unit->getType() == UnitTypes::Zerg_Drone)
        unit->self->order = Orders::DroneLand.getID();
      else if (unit->getType() == UnitTypes::Protoss_Probe)
        unit->self->order = Orders::BuildProtoss1.getID();
      else if (unit->getType() == UnitTypes::Terran_SCV)
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
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
      unit->self->isIdle = true;
      unit->self->isConstructing = false;
      unit->self->buildType = UnitTypes::None.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Morph)
    {
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        p->spend(-unitType.mineralPrice(), -unitType.gasPrice());
        //-p->planToMake(toMorph);
      }
      if (unitType.isTwoUnitsInOneEgg())
        p->useSupplies(-(unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired()), (u8)unitType.getRace().getID());
      else
        p->useSupplies(-(unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired()), (u8)unitType.getRace().getID());

      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
      unit->self->type = unitType.whatBuilds().first.getID();
      unit->self->buildType = UnitTypes::None.getID();
      unit->self->remainingBuildTime = 0;
      unit->self->isMorphing = false;
      unit->self->isConstructing = false;
      unit->self->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
      TechType tech(unit->self->tech);
      unit->self->tech = TechTypes::None.getID();
      unit->self->isIdle = true;
      unit->self->remainingResearchTime = 0;
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        p->spend(-tech.mineralPrice(),-tech.gasPrice());
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      int slot = command.extra;
      if (!unit->self->exists) return;
      if (slot >= 0)
      {
        for(int i=slot;i<4;i++)
          unit->self->trainingQueue[i]=unit->self->trainingQueue[i+1];
      }
      unit->self->trainingQueueCount--;
      if (unit->self->trainingQueueCount<0)
        unit->self->trainingQueueCount=0;
      if (unit->self->trainingQueueCount==0)
      {
        unit->self->isTraining = false;
        unit->self->order = Orders::Nothing.getID();
        unit->self->isIdle = true;
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
    {
      if (!unit->self->exists) return;
      UpgradeType upgrade = UpgradeType(unit->self->upgrade);
      unit->self->order = Orders::Nothing.getID();
      int level = unit->getPlayer()->getUpgradeLevel(unit->self->upgrade);
      unit->self->upgrade = UpgradeTypes::None.getID();
      unit->self->isIdle = true;
      unit->self->remainingUpgradeTime = 0;
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        p->spend(-upgrade.mineralPriceBase()-upgrade.mineralPriceFactor()*level, -upgrade.gasPriceBase()-upgrade.gasPriceFactor()*level);
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandGeneral::getType()
  {
    return command.type.getID();
  }
  //----------------------------------------------------------------------------------------------------------
};
