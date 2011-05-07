#include "UnitImpl.h"

#include <math.h>
#include <limits>
#include <sstream>

#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include <BWAPI/WeaponType.h>
#include "BWAPI/TemplatesImpl.h"
#include "Command.h"
#include "DLLMain.h"
#include "Detours.h"

#include <BW/Unit.h>
#include <BW/Offsets.h>

#include "../../Debug.h"

namespace BWAPI
{
  //--------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
  bool UnitImpl::canIssueCommand(UnitCommand command) const
  {
    return Templates::canIssueCommand<class GameImpl, class PlayerImpl, class UnitImpl>(this,command);
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    if (!canIssueCommand(command))
      return false;

    command.unit = this;

    if (command.type == UnitCommandTypes::Train ||
        command.type == UnitCommandTypes::Morph)
      if (getType().producesLarva() && command.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva )
        command.unit = *getLarva().begin();

    if (command.type == UnitCommandTypes::Use_Tech_Unit &&
       (command.extra == TechTypes::Archon_Warp || command.extra == TechTypes::Dark_Archon_Meld))
    {
      //select both units for archon warp or dark archon meld
      BW::Orders::Select sel = BW::Orders::Select(2, (UnitImpl*)command.unit, (UnitImpl*)command.target);
      botAPM_select++;
      QueueGameCommand((PBYTE)&sel, sel.size);
    }
    else if ( command.type != UnitCommandTypes::Unload )
      ((UnitImpl*)command.unit)->orderSelect();

    BroodwarImpl.executeCommand( command, true);
    ((UnitImpl*)command.unit)->lastCommandFrame = BroodwarImpl.frameCount;
    ((UnitImpl*)command.unit)->lastCommand      = command;
    return true;
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool UnitImpl::attack(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::attack(this, target, shiftQueueCommand));
  }
  //--------------------------------------------- ATTACK UNIT ------------------------------------------------
  bool UnitImpl::attack(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::attack(this, target, shiftQueueCommand));
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool UnitImpl::build(TilePosition target, UnitType type)
  {
    return issueCommand(UnitCommand::build(this, target, type));
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type)
  {
    return issueCommand(UnitCommand::buildAddon(this,type));
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type)
  {
    return issueCommand(UnitCommand::train(this,type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    return issueCommand(UnitCommand::morph(this,type));
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    return issueCommand(UnitCommand::research(this,tech));
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    return issueCommand(UnitCommand::upgrade(this,upgrade));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPoint(Position target)
  {
    return issueCommand(UnitCommand::setRallyPoint(this,target));
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyPoint(Unit* target)
  {
    return issueCommand(UnitCommand::setRallyPoint(this,target));
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::move(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::patrol(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::holdPosition(this, shiftQueueCommand));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::stop(this, shiftQueueCommand));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool UnitImpl::follow(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::follow(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::gather(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::returnCargo(this, shiftQueueCommand));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::repair(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    return issueCommand(UnitCommand::burrow(this));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    return issueCommand(UnitCommand::unburrow(this));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    return issueCommand(UnitCommand::cloak(this));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    return issueCommand(UnitCommand::decloak(this));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    return issueCommand(UnitCommand::siege(this));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    return issueCommand(UnitCommand::unsiege(this));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    return issueCommand(UnitCommand::lift(this));
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    return issueCommand(UnitCommand::land(this,target));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::load(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    return issueCommand(UnitCommand::unload(this,target));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::unloadAll(this, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::unloadAll(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::rightClick(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::rightClick(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    return issueCommand(UnitCommand::haltConstruction(this));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    return issueCommand(UnitCommand::cancelConstruction(this));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    return issueCommand(UnitCommand::cancelAddon(this));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    return issueCommand(UnitCommand::cancelTrain(this, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    return issueCommand(UnitCommand::cancelMorph(this));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    return issueCommand(UnitCommand::cancelResearch(this));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    return issueCommand(UnitCommand::cancelUpgrade(this));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    return issueCommand(UnitCommand::useTech(this,tech));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position target)
  {
    return issueCommand(UnitCommand::useTech(this,tech,target));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    return issueCommand(UnitCommand::useTech(this,tech,target));
  }
  //--------------------------------------------- PLACE COP --------------------------------------------------
  bool UnitImpl::placeCOP(TilePosition target)
  {
    return issueCommand(UnitCommand::placeCOP(this, target));
  }
};
