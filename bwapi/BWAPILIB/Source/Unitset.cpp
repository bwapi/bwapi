#include <BWAPI/Unitset.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/Position.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Game.h>
#include <BWAPI/TechType.h>

#include <utility>

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
      if ( pos.isValid() )
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
  void Unitset::setClientInfo(void *clientInfo, int index) const
  {
    if ( index < 0 || index > 255 )
      return;

    // Assign the client info to all units in the set
    for (auto &u : *this)
    {
      u->setClientInfo(clientInfo, index);
    }
  }
  void Unitset::setClientInfo(int clientInfo, int index) const
  {
    this->setClientInfo((void*)clientInfo, index);
  }

  Unitset Unitset::getUnitsInRadius(int radius, const UnitFilter &pred) const
  {
    // Return if this unit does not exist
    if ( this->empty() )
      return Unitset::none;

    return Broodwar->getUnitsInRadius(this->getPosition(), radius, pred);
  }
  Unit Unitset::getClosestUnit(const UnitFilter &pred, int radius) const
  {
    // Return if this unit does not exist
    if ( this->empty() )
      return nullptr;

    return Broodwar->getClosestUnit(this->getPosition(), pred, radius);
  }

  bool Unitset::issueCommand(UnitCommand command) const
  {
    return Broodwar->issueCommand(*this, command);
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool Unitset::attack(PositionOrUnit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::attack(nullptr, target, shiftQueueCommand));
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool Unitset::build(UnitType type, TilePosition target) const
  {
    if ( target == TilePositions::None )
      return this->train(type);
    return this->issueCommand(UnitCommand::build(nullptr, target, type));
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
    return this->issueCommand(UnitCommand::train(nullptr,type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool Unitset::morph(UnitType type) const
  {
    return this->issueCommand(UnitCommand::morph(nullptr,type));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool Unitset::setRallyPoint(PositionOrUnit target) const
  {
    return this->issueCommand( UnitCommand::setRallyPoint(nullptr, target) );
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool Unitset::move(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::move(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool Unitset::patrol(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::patrol(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool Unitset::holdPosition(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::holdPosition(nullptr, shiftQueueCommand));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool Unitset::stop(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::stop(nullptr, shiftQueueCommand));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool Unitset::follow(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::follow(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool Unitset::gather(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::gather(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool Unitset::returnCargo(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::returnCargo(nullptr, shiftQueueCommand));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool Unitset::repair(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::repair(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool Unitset::burrow() const
  {
    return this->issueCommand(UnitCommand::burrow(nullptr));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool Unitset::unburrow() const
  {
    return this->issueCommand(UnitCommand::unburrow(nullptr));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool Unitset::cloak() const
  {
    return this->issueCommand(UnitCommand::cloak(nullptr));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool Unitset::decloak() const
  {
    return this->issueCommand(UnitCommand::decloak(nullptr));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool Unitset::siege() const
  {
    return this->issueCommand(UnitCommand::siege(nullptr));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool Unitset::unsiege() const
  {
    return this->issueCommand(UnitCommand::unsiege(nullptr));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool Unitset::lift() const
  {
    return this->issueCommand(UnitCommand::lift(nullptr));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool Unitset::load(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::load(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::unloadAll(nullptr, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::unloadAll(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool Unitset::rightClick(PositionOrUnit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::rightClick(nullptr,target, shiftQueueCommand));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool Unitset::haltConstruction() const
  {
    return this->issueCommand(UnitCommand::haltConstruction(nullptr));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool Unitset::cancelConstruction() const
  {
    return this->issueCommand(UnitCommand::cancelConstruction(nullptr));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool Unitset::cancelAddon() const
  {
    return this->issueCommand(UnitCommand::cancelAddon(nullptr));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool Unitset::cancelTrain(int slot) const
  {
    return this->issueCommand(UnitCommand::cancelTrain(nullptr, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool Unitset::cancelMorph() const
  {
    return this->issueCommand(UnitCommand::cancelMorph(nullptr));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool Unitset::cancelResearch() const
  {
    return this->issueCommand(UnitCommand::cancelResearch(nullptr));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool Unitset::cancelUpgrade() const
  {
    return this->issueCommand(UnitCommand::cancelUpgrade(nullptr));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool Unitset::useTech(TechType tech, PositionOrUnit target) const
  {
    if ( target.isUnit() && target.getUnit() == nullptr )
      return this->issueCommand(UnitCommand::useTech(nullptr,tech));
    return this->issueCommand(UnitCommand::useTech(nullptr,tech,target));
  }
}

