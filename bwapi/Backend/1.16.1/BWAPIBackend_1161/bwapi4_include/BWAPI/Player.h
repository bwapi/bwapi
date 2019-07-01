#pragma once
#include <string>

#include <BWAPI/Position.h>
#include <BWAPI/Race.h>
#include <BWAPI/Filters.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  // Forwards
  class Color;
  class ForceInterface;
  typedef ForceInterface *Force;
  class PlayerType;
  class TechType;
  class Unitset;
  class UpgradeType;
  class WeaponType;

  struct PlayerData;

  class PlayerInterface;
  typedef PlayerInterface *Player;

  /// <summary>The Player represents a unique controller in the game.</summary> Each player in
  /// a match will have his or her own player instance. There is also a neutral player which owns
  /// all the neutral units (such as mineral patches and vespene geysers).
  ///
  /// @see Playerset, PlayerType, Race
  /// @ingroup Interface
  class PlayerInterface
  {
  protected:
    virtual ~PlayerInterface() {};
  public :
    virtual int getID() const = 0;

    virtual std::string getName() const = 0;

    virtual const Unitset &getUnits() const = 0;

    virtual Race getRace() const = 0;

    virtual PlayerType getType() const = 0;

    virtual Force getForce() const = 0;

    virtual bool isAlly(const Player player) const = 0;

    virtual bool isEnemy(const Player player) const = 0;

    virtual bool isNeutral() const = 0;

    virtual TilePosition getStartLocation() const = 0;

    virtual bool isVictorious() const = 0;

    virtual bool isDefeated() const = 0;

    virtual bool leftGame() const = 0;

    virtual int minerals() const = 0;

    virtual int gas() const = 0;

    virtual int gatheredMinerals() const = 0;

    virtual int gatheredGas() const = 0;

    virtual int repairedMinerals() const = 0;

    virtual int repairedGas() const = 0;

    virtual int refundedMinerals() const = 0;

    virtual int refundedGas() const = 0;

    virtual int spentMinerals() const = 0;

    virtual int spentGas() const = 0;

    virtual int supplyTotal(Race race = Races::None) const = 0;

    virtual int supplyUsed(Race race = Races::None) const = 0;

    virtual int allUnitCount(UnitType unit = UnitTypes::AllUnits) const = 0;

    virtual int visibleUnitCount(UnitType unit = UnitTypes::AllUnits) const = 0;

    virtual int completedUnitCount(UnitType unit = UnitTypes::AllUnits) const = 0;

    int incompleteUnitCount(UnitType unit = UnitTypes::AllUnits) const;

    virtual int deadUnitCount(UnitType unit = UnitTypes::AllUnits) const = 0;

    virtual int killedUnitCount(UnitType unit = UnitTypes::AllUnits) const = 0;

    virtual int getUpgradeLevel(UpgradeType upgrade) const = 0;

    virtual bool hasResearched(TechType tech) const = 0;

    virtual bool isResearching(TechType tech) const = 0;

    virtual bool isUpgrading(UpgradeType upgrade) const = 0;

    virtual BWAPI::Color getColor() const = 0;

    char getTextColor() const;

    int maxEnergy(UnitType unit) const;

    double topSpeed(UnitType unit) const;

    int weaponMaxRange(WeaponType weapon) const;

    int sightRange(UnitType unit) const;

    int weaponDamageCooldown(UnitType unit) const;

    int armor(UnitType unit) const;

    int damage(WeaponType wpn) const;

    virtual int getUnitScore() const = 0;

    virtual int getKillScore() const = 0;

    virtual int getBuildingScore() const = 0;

    virtual int getRazingScore() const = 0;

    virtual int getCustomScore() const = 0;

    virtual bool isObserver() const = 0;

    virtual int getMaxUpgradeLevel(UpgradeType upgrade) const = 0;

    virtual bool isResearchAvailable(TechType tech) const = 0;

    virtual bool isUnitAvailable(UnitType unit) const = 0;

    bool hasUnitTypeRequirement(UnitType unit, int amount = 1) const;
  };
};
