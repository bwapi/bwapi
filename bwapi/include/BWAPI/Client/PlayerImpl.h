#pragma once
#include <BWAPI.h>
#include "PlayerData.h"
#include <string>

#include <BWAPI/Unitset.h>

namespace BWAPI
{
  class ForceInterface;
  typedef ForceInterface *Force;

  class PlayerImpl : public PlayerInterface
  {
    private:
      int id;
    public:
      PlayerData* self;
      Unitset units;
      void clear();
      PlayerImpl(int id);
      virtual int getID() const override;
      virtual std::string getName() const override;
      virtual const Unitset &getUnits() const override;
      virtual Race getRace() const override;
      virtual PlayerType getType() const override;
      virtual Force getForce() const override;
      virtual bool isAlly(const Player player) const override;
      virtual bool isEnemy(const Player player) const override;
      virtual bool isNeutral() const override;
      virtual TilePosition getStartLocation() const override;
      virtual bool isVictorious() const override;
      virtual bool isDefeated() const override;
      virtual bool leftGame() const override;

      virtual int minerals() const override;
      virtual int gas() const override;
      virtual int gatheredMinerals() const override;
      virtual int gatheredGas() const override;
      virtual int repairedMinerals() const override;
      virtual int repairedGas() const override;
      virtual int refundedMinerals() const override;
      virtual int refundedGas() const override;
      virtual int spentMinerals() const override;
      virtual int spentGas() const override;

      virtual int supplyTotal(const Race race = Races::None) const override;
      virtual int supplyUsed(const Race race = Races::None) const override;

      virtual int allUnitCount(const UnitType unit) const override;
      virtual int visibleUnitCount(const UnitType unit) const override;
      virtual int completedUnitCount(const UnitType unit) const override;
      virtual int deadUnitCount(const UnitType unit) const override;
      virtual int killedUnitCount(const UnitType unit) const override;

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
  };
};
