#pragma once
#include "../Types.h"
#include <string>

#include <BWAPI4/Client/PlayerData.h>
#include <BWAPI4/Unitset.h>
#include <BWAPI/Color.h>
#include <BWAPI/Race.h>

#include "UnitImpl.h"
#include "ForceImpl.h"

namespace BWAPI
{
  class UnitType;
  class UpgradeType;
  class TechType;
  class PlayerType;
  class Race;
}

namespace BWAPI4
{
  // Forwards
  class ForceImpl;
  typedef ForceImpl* Force;

  /** Represents one player in game. Note that there is always player 12 who owns resources. */
  class PlayerImpl
  {
    public:
      int         getID() const;
      std::string getName() const;
      Race        getRace() const;
      PlayerType  getType() const;
      Force       getForce() const;

      bool isAlly(const Player player) const;
      bool isEnemy(const Player player) const;
      bool isNeutral() const;

      bool isVictorious() const;
      bool isDefeated() const;
      bool leftGame() const;

      const Unitset &getUnits() const;
      TilePosition  getStartLocation() const;

      int  minerals() const;
      int  gas() const;

      int  gatheredMinerals() const;
      int  gatheredGas() const;
      int  repairedMinerals() const;
      int  repairedGas() const;
      int  refundedMinerals() const;
      int  refundedGas() const;
      int  spentMinerals() const;
      int  spentGas() const;

      int  supplyTotal(Race race = Races::None) const;
      int  supplyUsed(Race race = Races::None) const;

      int  allUnitCount(UnitType unit) const;
      int  visibleUnitCount(UnitType unit) const;
      int  completedUnitCount(UnitType unit) const;
      int  deadUnitCount(UnitType unit) const;
      int  killedUnitCount(UnitType unit) const;

      int  getUpgradeLevel(UpgradeType upgrade) const;
      bool hasResearched(TechType tech) const;
      bool isResearching(TechType tech) const;
      bool isUpgrading(UpgradeType upgrade) const;

      BWAPI4::Color getColor() const;

      int getUnitScore() const;
      int getKillScore() const;
      int getBuildingScore() const;
      int getRazingScore() const;
      int getCustomScore() const;

      bool isObserver() const;

      int  getMaxUpgradeLevel(UpgradeType upgrade) const;
      bool isResearchAvailable(TechType tech) const;
      bool isUnitAvailable(UnitType unit) const;

      char getTextColor() const;
      bool hasUnitTypeRequirement(UnitType unit, int amount = 1) const;
      int weaponMaxRange(WeaponType weapon) const;

      //Internal BWAPI commands:
      /**
       * Constructs new player
       * @param id 0-based Index of the player (11 for resources as it is player 12)
       */
      PlayerImpl(u8 index);
      int getIndex() const;      // Gets 0-based index of the player. (11 for neutral)
      int getNationId() const;   // Nation id in team melee

      void setID(int newID);

      /**
       * Updates localData according to bw values. (Should be called every frame before local data updates are
       * performed
       */
      void updateData();
      void onGameEnd();

      void setParticipating(bool isParticipating = true);
      void resetResources();
      
    // data members
      ForceImpl*  force = nullptr;
      PlayerData  data = PlayerData();
      PlayerData* self = &data;
      Unitset     units;

      s32 _repairedMinerals;
      s32 _repairedGas;
      s32 _refundedMinerals;
      s32 _refundedGas;

      bool wasSeenByBWAPIPlayer = false;
    private :
      int id = -1;
      u8 index;  /**< Order of the player, is used to load player's information from the memory */
  };
};
