#pragma once
#include <BWAPI/Vectorset.h>
#include <BWAPI/Position.h>
#include <BWAPI/PositionUnit.h>
#include <BWAPI/Filters.h>

namespace BWAPI
{
  // Forward declarations
  class UnitType;
  class Regionset;
  class UnitCommand;
  class TechType;

  /// The Unitset is a container for a set of pointers to Unit objects. It is typically used for
  /// groups of units instead of having to manage each Unit individually.
  ///
  /// @note Because of the efficiency, it is recommended to use the Unitset over any of the STL
  /// containers.
  ///
  /// @see Unit, UnitType, UnitSizeType
  class Unitset : public Vectorset<BWAPI::Unit >
  {
  public:
    /// A blank Unitset containing no elements. This is typically used as a return value for BWAPI
    /// interface functions that have encountered an error.
    static const Unitset none;

    /// The Unitset constructor.
    ///
    /// @param initialSize The initial maximum size of the Unitset before expanding it is
    /// necessary.
    Unitset(size_t initialSize = 16);
    
    /// The copy constructor.
    ///
    /// @param other The Unitset to construct this one from.
    Unitset(const Unitset &other);

    /// The move constructor.
    ///
    /// @param other The Unitset to move its contents from.
    Unitset(Unitset &&other);

    /// Calculates the average of all valid Unit positions in this set.
    ///
    /// @returns Average Position of all units in the set.
    ///
    /// @see UnitInterface::getPosition
    Position getPosition() const;

    /// Creates a single set containing all units that are loaded into units of this set.
    ///
    /// @returns The set of all loaded units.
    ///
    /// @see UnitInterface::getLoadedUnits
    Unitset getLoadedUnits() const;

    /// Creates a single set containing all the @Interceptors of all @Carriers in this set.
    ///
    /// @returns The set of all @Interceptors .
    /// 
    /// @see UnitInterface::getInterceptors
    Unitset getInterceptors() const;

    /// Creates a single set containing all the @Larvae of all @Hatcheries, @Lairs, and @Hives in
    /// this set.
    ///
    /// @returns The set of all @Larvae .
    ///
    /// @see UnitInterface::getLarva
    Unitset getLarva() const;

    /// Sets the client info for every unit in this set.
    ///
    /// @param clientInfo A pointer to client information, managed by the AI module, or nullptr if
    /// client information is to be cleared.
    ///
    /// @see UnitInterface::setClientInfo
    void setClientInfo(void *clientInfo = nullptr, int index = 0) const;

    /// @overload
    void setClientInfo(int clientInfo = 0, int index = 0) const;

    Unitset getUnitsInRadius(int radius, const UnitFilter &pred = nullptr) const;
    Unit getClosestUnit(const UnitFilter &pred = nullptr, int radius = 999999) const;

    /// @copydoc UnitInterface::issueCommand
    bool issueCommand(UnitCommand command) const;
    
    /// @copydoc UnitInterface::attack
    bool attack(PositionOrUnit target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::build
    bool build(UnitType type, TilePosition target = TilePositions::None) const;

    /// @copydoc UnitInterface::buildAddon
    bool buildAddon(UnitType type) const;

    /// @copydoc UnitInterface::train
    bool train(UnitType type) const;

    /// @copydoc UnitInterface::morph
    bool morph(UnitType type) const;

    /// @copydoc UnitInterface::setRallyPoint
    bool setRallyPoint(PositionOrUnit target) const;

    /// @copydoc UnitInterface::move
    bool move(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::patrol
    bool patrol(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::holdPosition
    bool holdPosition(bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::stop
    bool stop(bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::follow
    bool follow(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::gather
    bool gather(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::returnCargo
    bool returnCargo(bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::repair
    bool repair(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::burrow
    bool burrow() const;

    /// @copydoc UnitInterface::unburrow
    bool unburrow() const;

    /// @copydoc UnitInterface::cloak
    bool cloak() const;

    /// @copydoc UnitInterface::decloak
    bool decloak() const;

    /// @copydoc UnitInterface::siege
    bool siege() const;

    /// @copydoc UnitInterface::unsiege
    bool unsiege() const;

    /// @copydoc UnitInterface::lift
    bool lift() const;

    /// @copydoc UnitInterface::load
    bool load(Unit target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::unloadAll(bool)
    bool unloadAll(bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::unloadAll(Position,bool)
    bool unloadAll(Position target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::rightClick
    bool rightClick(PositionOrUnit target, bool shiftQueueCommand = false) const;

    /// @copydoc UnitInterface::haltConstruction
    bool haltConstruction() const;

    /// @copydoc UnitInterface::cancelConstruction
    bool cancelConstruction() const;

    /// @copydoc UnitInterface::cancelAddon
    bool cancelAddon() const;

    /// @copydoc UnitInterface::cancelTrain
    bool cancelTrain(int slot = -2) const;

    /// @copydoc UnitInterface::cancelMorph
    bool cancelMorph() const;

    /// @copydoc UnitInterface::cancelResearch
    bool cancelResearch() const;

    /// @copydoc UnitInterface::cancelUpgrade
    bool cancelUpgrade() const;
    
    /// @copydoc UnitInterface::useTech
    bool useTech(TechType tech, PositionOrUnit target = nullptr) const;
  };
}

