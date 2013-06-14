#pragma once
#include <Util/Types.h>
#include <string>
#include <BW/Offsets.h>

#include <BWAPI/Player.h>
#include <BWAPI/Client/PlayerData.h>
#include <BWAPI/Unitset.h>

#include "UnitImpl.h"
#include "ForceImpl.h"

namespace BWAPI
{
  // Forwards
  class ForceInterface;
  typedef ForceInterface *Force;
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

      virtual bool isAlly(Player player) const override;
      virtual bool isEnemy(Player player) const override;
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

      virtual int  supplyTotal(Race race = Races::None) const override;
      virtual int  supplyUsed(Race race = Races::None) const override;

      virtual int  allUnitCount(UnitType unit) const override;
      virtual int  visibleUnitCount(UnitType unit) const override;
      virtual int  completedUnitCount(UnitType unit) const override;
      virtual int  deadUnitCount(UnitType unit) const override;
      virtual int  killedUnitCount(UnitType unit) const override;

      virtual int  getUpgradeLevel(UpgradeType upgrade) const override;
      virtual bool hasResearched(TechType tech) const override;
      virtual bool isResearching(TechType tech) const override;
      virtual bool isUpgrading(UpgradeType upgrade) const override;

      virtual BWAPI::Color getColor() const override;

      virtual int getUnitScore() const override;
      virtual int getKillScore() const override;
      virtual int getBuildingScore() const override;
      virtual int getRazingScore() const override;
      virtual int getCustomScore() const override;

      virtual bool isObserver() const override;

      virtual int  getMaxUpgradeLevel(UpgradeType upgrade) const override;
      virtual bool isResearchAvailable(TechType tech) const override;
      virtual bool isUnitAvailable(UnitType unit) const override;

      //Internal BWAPI commands:
      /**
       * Constructs new player
       * @param id 0-based Index of the player (11 for resources as it is player 12)
       */
      PlayerImpl(u8 index);
      ~PlayerImpl();
      int getIndex() const;
      /** Gets 0-based index of the player. (11 for neutral) */
      char* getForceName() const;
      void setID(int newID);

      /** Gets the bw mapping of synchronised version of selected unit for current player */
      BW::CUnit** selectedUnit();
      /**
       * Updates localData according to bw values. (Should be called every frame before local data updates are
       * performed
       */
      void updateData();
      void onGameEnd();

      void setParticipating(bool isParticipating = true);
      void resetResources();
      
    // data members
      ForceImpl   *force;
      PlayerData  data;
      PlayerData  *self;
      Unitset     units;

      s32 _repairedMinerals;
      s32 _repairedGas;
      s32 _refundedMinerals;
      s32 _refundedGas;

      bool wasSeenByBWAPIPlayer;
    private :
      int id;
      u8 index;  /**< Order of the player, is used to load player's information from the memory */
  };
};
