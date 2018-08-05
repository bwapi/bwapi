#pragma once
#include "SetContainer.h"
#include <BWAPI/Position.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Game.h>
#include <BWAPI/Unit.h>
#include <iterator>

namespace BWAPI
{
  // Forward declarations
  class UnitType;
  class Regionset;
  class UnitCommand;
  class TechType;

  /// <summary>The Unitset is a container for a set of pointers to Unit objects. It is typically
  /// used for groups of units instead of having to manage each Unit individually.</summary>
  ///
  /// @see Unit
  using Container_Unitset = SetContainer < Unit, Unit::Hash >;
  class Unitset : public Container_Unitset
  {
  public:
    using Container_Unitset::Container_Unitset;
    /// <summary>A blank Unitset containing no elements.</summary> This is typically used as a
    /// return value for BWAPI interface functions that have encountered an error.
    static const Unitset none;

    template<typename ...Ts>
    Unitset &operator=(Ts &&...vals) {
      Container_Unitset::operator=(std::forward<Ts>(vals)...);
      return *this;
    }

    /// <summary>Calculates the average of all valid Unit positions in this set.</summary>
    ///
    /// @returns Average Position of all units in the set.
    ///
    /// @see Unit::getPosition
    Position getPosition() const;

    /// <summary>Creates a single set containing all units that are loaded into units of this set.</summary>
    ///
    /// @returns The set of all loaded units.
    ///
    /// @see Unit::getLoadedUnits
    Unitset getLoadedUnits() const;

    /// <summary>Creates a single set containing all the @Interceptors of all @Carriers in this set.</summary>
    ///
    /// @returns The set of all @Interceptors .
    /// 
    /// @see Unit::getInterceptors
    Unitset getInterceptors() const;

    /// <summary>Creates a single set containing all the @Larvae of all @Hatcheries, @Lairs, and
    /// @Hives in this set.</summary>
    ///
    /// @returns The set of all @Larvae .
    ///
    /// @see Unit::getLarva
    Unitset getLarva() const;

    /// <summary>Sets the client info for every unit in this set.</summary>
    ///
    /// <param name="clientInfo"> (optional)
    ///   A pointer to client information, managed by the AI module, or nullptr if client
    ///   information is to be cleared.
    /// </param>
    /// <param name="index"> (optional)
    ///   An key value for the client info mapping so that more than one piece of data can be
    ///   mapped to the same unit.
    /// </param>
    ///
    /// @see Unit::setClientInfo
    void setClientInfo(void *clientInfo = nullptr, int index = 0) const;
    /// @overload
    void setClientInfo(int clientInfo = 0, int index = 0) const;

    /// @copydoc Unit::getUnitsInRadius
    Unitset getUnitsInRadius(int radius, const UnitFilter &pred = nullptr) const;

    /// @copydoc Unit::getClosestUnit
    Unit getClosestUnit(const UnitFilter &pred = nullptr, int radius = 999999) const;

    /// @name Unit Commands
    /// @{

    /// @copydoc Unit::issueCommand
    bool issueCommand(UnitCommand command) const;
    
    /// @copydoc Unit::attack
    bool attack(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::attack
    bool attack(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::build
    bool build(UnitType type, TilePosition target = TilePositions::None) const;

    /// @copydoc Unit::buildAddon
    bool buildAddon(UnitType type) const;

    /// @copydoc Unit::train
    bool train(UnitType type) const;

    /// @copydoc Unit::morph
    bool morph(UnitType type) const;

    /// @copydoc Unit::setRallyPoint
    bool setRallyPoint(Unit target) const;

    /// @copydoc Unit::setRallyPoint
    bool setRallyPoint(Position target) const;

    /// @copydoc Unit::move
    bool move(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::patrol
    bool patrol(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::holdPosition
    bool holdPosition(bool shiftQueueCommand = false) const;

    /// @copydoc Unit::stop
    bool stop(bool shiftQueueCommand = false) const;

    /// @copydoc Unit::follow
    bool follow(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::gather
    bool gather(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::returnCargo
    bool returnCargo(bool shiftQueueCommand = false) const;

    /// @copydoc Unit::repair
    bool repair(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::burrow
    bool burrow() const;

    /// @copydoc Unit::unburrow
    bool unburrow() const;

    /// @copydoc Unit::cloak
    bool cloak() const;

    /// @copydoc Unit::decloak
    bool decloak() const;

    /// @copydoc Unit::siege
    bool siege() const;

    /// @copydoc Unit::unsiege
    bool unsiege() const;

    /// @copydoc Unit::lift
    bool lift() const;

    /// @copydoc Unit::load
    bool load(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::unloadAll(bool)
    bool unloadAll(bool shiftQueueCommand = false) const;

    /// @copydoc Unit::unloadAll(Position,bool)
    bool unloadAll(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::rightClick
    bool rightClick(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::rightClick
    bool rightClick(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc Unit::haltConstruction
    bool haltConstruction() const;

    /// @copydoc Unit::cancelConstruction
    bool cancelConstruction() const;

    /// @copydoc Unit::cancelAddon
    bool cancelAddon() const;

    /// @copydoc Unit::cancelTrain
    bool cancelTrain(int slot = -2) const;

    /// @copydoc Unit::cancelMorph
    bool cancelMorph() const;

    /// @copydoc Unit::cancelResearch
    bool cancelResearch() const;

    /// @copydoc Unit::cancelUpgrade
    bool cancelUpgrade() const;

    /// @copydoc Unit::useTech
    bool useTech(TechType tech) const;

    /// @copydoc Unit::useTech
    bool useTech(TechType tech, Unit target) const;

    /// @copydoc Unit::useTech
    bool useTech(TechType tech, Position target) const;

    ///@}
  };
}

