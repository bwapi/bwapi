#pragma once
#include <Util/Types.h>
#include <string>

#include <BWAPI/Player.h>
#include <BWAPI/Client/PlayerData.h>
#include <BWAPI/Unitset.h>

#include "UnitImpl.h"
#include "ForceImpl.h"

namespace BWAPI
{
  // Forwards
  class ForceInterface;
  typedef ForceInterface* Force;
  class UnitType;
  class UpgradeType;
  class TechType;
  class PlayerType;
  class Race;

  /** Represents one player in game. Note that there is always player 12 who owns resources. */
  class PlayerImpl : public PlayerInterface
  {
    public:
      virtual int         getID() const override;
      virtual std::string getName() const override;
      virtual Race        getRace() const override;
      virtual PlayerType  getType() const override;
      virtual Force       getForce() const override;

      virtual bool isAlly(const Player player) const override;
      virtual bool isEnemy(const Player player) const override;
      virtual bool isNeutral() const override;

      virtual bool isVictorious() const override;
      virtual bool isDefeated() const override;
      virtual bool leftGame() const override;

      virtual const Unitset &getUnits() const override;
      virtual TilePosition  getStartLocation() const override;

      virtual int  minerals() const override;
      virtual int  gas() const override;

      virtual int  gatheredMinerals() const override;
      virtual int  gatheredGas() const override;
      virtual int  repairedMinerals() const override;
      virtual int  repairedGas() const override;
      virtual int  refundedMinerals() const override;
      virtual int  refundedGas() const override;
      virtual int  spentMinerals() const override;
      virtual int  spentGas() const override;

      virtual int  supplyTotal(const Race race = Races::None) const override;
      virtual int  supplyUsed(const Race race = Races::None) const override;

      virtual int  allUnitCount(const UnitType unit) const override;
      virtual int  visibleUnitCount(const UnitType unit) const override;
      virtual int  completedUnitCount(const UnitType unit) const override;
      virtual int  deadUnitCount(const UnitType unit) const override;
      virtual int  killedUnitCount(const UnitType unit) const override;

      virtual int  getUpgradeLevel(const UpgradeType upgrade) const override;
      virtual bool hasResearched(const TechType tech) const override;
      virtual bool isResearching(const TechType tech) const override;
      virtual bool isUpgrading(const UpgradeType upgrade) const override;

      virtual BWAPI::Color getColor() const override;

      virtual int getUnitScore() const override;
      virtual int getKillScore() const override;
      virtual int getBuildingScore() const override;
      virtual int getRazingScore() const override;
      virtual int getCustomScore() const override;

      virtual bool isObserver() const override;

      virtual int  getMaxUpgradeLevel(const UpgradeType upgrade) const override;
      virtual bool isResearchAvailable(const TechType tech) const override;
      virtual bool isUnitAvailable(const UnitType unit) const override;

      //Internal BWAPI commands:
      /**
       * Constructs new player
       * @param id 0-based Index of the player (11 for resources as it is player 12)
       */
      PlayerImpl(u8 index);
      int getIndex() const;      // Gets 0-based index of the player. (11 for neutral)


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
