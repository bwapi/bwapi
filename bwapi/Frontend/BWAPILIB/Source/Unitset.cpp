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
      success = success || u->issueCommand(command);
    }
    return success;
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool Unitset::attack(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::attack({-1}, target, shiftQueueCommand));
  }
  bool Unitset::attack(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::attack({-1}, target, shiftQueueCommand));
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool Unitset::build(UnitType type, TilePosition target) const
  {
    if ( target == TilePositions::None )
      return this->train(type);
    return this->issueCommand(UnitCommand::build({-1}, target, type));
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
    return this->issueCommand(UnitCommand::train({-1},type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool Unitset::morph(UnitType type) const
  {
    return this->issueCommand(UnitCommand::morph({-1},type));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool Unitset::setRallyPoint(Position target) const
  {
    return this->issueCommand(UnitCommand::setRallyPoint({-1}, target));
  }
  bool Unitset::setRallyPoint(Unit target) const
  {
    return this->issueCommand(UnitCommand::setRallyPoint({-1}, target.getID()));
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool Unitset::move(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::move({-1},target, shiftQueueCommand));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool Unitset::patrol(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::patrol({-1},target, shiftQueueCommand));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool Unitset::holdPosition(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::holdPosition({-1}, shiftQueueCommand));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool Unitset::stop(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::stop({-1}, shiftQueueCommand));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool Unitset::follow(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::follow({-1},target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool Unitset::gather(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::gather({-1},target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool Unitset::returnCargo(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::returnCargo({-1}, shiftQueueCommand));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool Unitset::repair(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::repair({-1},target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool Unitset::burrow() const
  {
    return this->issueCommand(UnitCommand::burrow({-1}));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool Unitset::unburrow() const
  {
    return this->issueCommand(UnitCommand::unburrow({-1}));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool Unitset::cloak() const
  {
    return this->issueCommand(UnitCommand::cloak({-1}));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool Unitset::decloak() const
  {
    return this->issueCommand(UnitCommand::decloak({-1}));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool Unitset::siege() const
  {
    return this->issueCommand(UnitCommand::siege({-1}));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool Unitset::unsiege() const
  {
    return this->issueCommand(UnitCommand::unsiege({-1}));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool Unitset::lift() const
  {
    return this->issueCommand(UnitCommand::lift({-1}));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool Unitset::load(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::load({-1},target.getID(), shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::unloadAll({-1}, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::unloadAll({-1},target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool Unitset::rightClick(Unit target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::rightClick({-1}, target.getID(), shiftQueueCommand));
  }
  bool Unitset::rightClick(Position target, bool shiftQueueCommand) const
  {
    return this->issueCommand(UnitCommand::rightClick({-1}, target, shiftQueueCommand));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool Unitset::haltConstruction() const
  {
    return this->issueCommand(UnitCommand::haltConstruction({-1}));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool Unitset::cancelConstruction() const
  {
    return this->issueCommand(UnitCommand::cancelConstruction({-1}));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool Unitset::cancelAddon() const
  {
    return this->issueCommand(UnitCommand::cancelAddon({-1}));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool Unitset::cancelTrain(int slot) const
  {
    return this->issueCommand(UnitCommand::cancelTrain({-1}, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool Unitset::cancelMorph() const
  {
    return this->issueCommand(UnitCommand::cancelMorph({-1}));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool Unitset::cancelResearch() const
  {
    return this->issueCommand(UnitCommand::cancelResearch({-1}));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool Unitset::cancelUpgrade() const
  {
    return this->issueCommand(UnitCommand::cancelUpgrade({-1}));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool Unitset::useTech(TechType tech) const
  {
    return issueCommand(UnitCommand::useTech({-1}, tech));
  }
  bool Unitset::useTech(TechType tech, Unit target) const
  {
    return this->issueCommand(UnitCommand::useTech({-1}, tech, target));
  }
  bool Unitset::useTech(TechType tech, Position target) const
  {
    return this->issueCommand(UnitCommand::useTech({-1}, tech, target));
  }
}

