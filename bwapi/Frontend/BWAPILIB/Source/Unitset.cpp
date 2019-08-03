#include <BWAPI/Unitset.h>

#include <BWAPI/UnitCommand.h>
#include <BWAPI/Position.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Game.h>
#include <BWAPI/TechType.h>
#include <BWAPI/IDs.h>

namespace BWAPI
{
  // initialize no-set
  const Unitset Unitset::none;

  ////////////////////////////////////////////////////////// Position
  Position Unitset::getPosition() const
  {
    // Declare the local position
    Position retPosition(0,0);
    int validPosCount = 0;

    // Add up the positions for all units in the set
    for ( auto &u : *this )
    {
      Position pos = u->getPosition();
      if ( u->getGame().isValid(pos) )
      {
        retPosition += pos;
        ++validPosCount;
      }
    }

    if (validPosCount > 0)
      retPosition /= validPosCount;
    return retPosition;
  }
  ////////////////////////////////////////////////////////// sets
  Unitset Unitset::getLoadedUnits() const
  {
    Unitset retSet;
    for (auto &u : *this)
    {
      auto units = u->getLoadedUnits();
      retSet.insert(units.begin(), units.end());
    }
    return retSet;
  }
  Unitset Unitset::getInterceptors() const
  {
    Unitset retSet;
    for (auto &u : *this)
    {
      auto units = u->getInterceptors();
      retSet.insert(units.begin(), units.end());
    }
    return retSet;
  }
  Unitset Unitset::getLarva() const
  {
    Unitset retSet;
    for (auto &u : *this)
    {
      auto units = u->getLarva();
      retSet.insert(units.begin(), units.end());
    }
    return retSet;
  }
  ////////////////////////////////////////////////////////// Misc
  Unitset Unitset::getUnitsInRadius(int radius, const UnitFilter &pred) const
  {
    // Return if this unit does not exist
    if ( this->empty() )
      return Unitset::none;

    // Assume all units are in the same Game
    return begin()->getGame().getUnitsInRadius(this->getPosition(), radius, pred);
  }
  Unit Unitset::getClosestUnit(const UnitFilter &pred, int radius) const
  {
    // Return if this unit does not exist
    if ( this->empty() )
      return nullptr;

    // Assume all units are in the same Game
    return begin()->getGame().getClosestUnit(this->getPosition(), pred, radius);
  }

  bool Unitset::issueCommand(UnitCommand command) const {
    bool success = false;
    for(auto &u : *this) {
      success |= u->issueCommand(command);
    }
    return success;
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool Unitset::attack(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::attack(UnitID::None, target, shiftQueueCommand));
  }
  bool Unitset::attack(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::attack(UnitID::None, target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool Unitset::buildAddon(UnitType type) const
  {
    bool result = false;
    for (auto &it : *this)
    {
      result |= it->buildAddon(type);
    }
    return result;
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool Unitset::train(UnitType type) const
  {
    return this->issueCommand(UnitCommand::train(UnitID::None, type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool Unitset::morph(UnitType type) const
  {
    return this->issueCommand(UnitCommand::morph(UnitID::None, type));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool Unitset::setRallyPoint(Position target) const
  {
    return this->issueCommand(UnitCommand::setRallyPoint(UnitID::None, target));
  }
  bool Unitset::setRallyPoint(Unit target) const
  {
    return this->issueCommand(UnitCommand::setRallyPoint(UnitID::None, target.getID()));
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool Unitset::move(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::move(UnitID::None, target, shiftQueueCommand));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool Unitset::patrol(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::patrol(UnitID::None, target, shiftQueueCommand));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool Unitset::holdPosition(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::holdPosition(UnitID::None, shiftQueueCommand));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool Unitset::stop(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::stop(UnitID::None, shiftQueueCommand));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool Unitset::follow(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::follow(UnitID::None, target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool Unitset::gather(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::gather(UnitID::None, target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool Unitset::returnCargo(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::returnCargo(UnitID::None, shiftQueueCommand));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool Unitset::repair(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::repair(UnitID::None, target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool Unitset::burrow() const
  {
    return this->issueCommand(UnitCommand::burrow(UnitID::None));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool Unitset::unburrow() const
  {
    return this->issueCommand(UnitCommand::unburrow(UnitID::None));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool Unitset::cloak() const
  {
    return this->issueCommand(UnitCommand::cloak(UnitID::None));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool Unitset::decloak() const
  {
    return this->issueCommand(UnitCommand::decloak(UnitID::None));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool Unitset::siege() const
  {
    return this->issueCommand(UnitCommand::siege(UnitID::None));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool Unitset::unsiege() const
  {
    return this->issueCommand(UnitCommand::unsiege(UnitID::None));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool Unitset::lift() const
  {
    return this->issueCommand(UnitCommand::lift(UnitID::None));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool Unitset::load(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::load(UnitID::None, target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::unloadAll(UnitID::None, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::unloadAll(UnitID::None, target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool Unitset::rightClick(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::rightClick(UnitID::None, target.getID(), shiftQueueCommand));
  }
  bool Unitset::rightClick(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::rightClick(UnitID::None, target, shiftQueueCommand));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool Unitset::haltConstruction() const
  {
    return this->issueCommand(UnitCommand::haltConstruction(UnitID::None));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool Unitset::cancelConstruction() const
  {
    return this->issueCommand(UnitCommand::cancelConstruction(UnitID::None));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool Unitset::cancelAddon() const
  {
    return this->issueCommand(UnitCommand::cancelAddon(UnitID::None));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool Unitset::cancelTrain(int slot) const
  {
    return this->issueCommand(UnitCommand::cancelTrain(UnitID::None, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool Unitset::cancelMorph() const
  {
    return this->issueCommand(UnitCommand::cancelMorph(UnitID::None));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool Unitset::cancelResearch() const
  {
    return this->issueCommand(UnitCommand::cancelResearch(UnitID::None));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool Unitset::cancelUpgrade() const
  {
    return this->issueCommand(UnitCommand::cancelUpgrade(UnitID::None));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool Unitset::useTech(TechType tech) const
  {
    return issueCommand(UnitCommand::useTech(UnitID::None, tech));
  }
  bool Unitset::useTech(TechType tech, Unit target) const
  {
    return this->issueCommand(UnitCommand::useTech(UnitID::None, tech, target.getID()));
  }
  bool Unitset::useTech(TechType tech, Position target) const
  {
    return this->issueCommand(UnitCommand::useTech(UnitID::None, tech, target));
  }
}

