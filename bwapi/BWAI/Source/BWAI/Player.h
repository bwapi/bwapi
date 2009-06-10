#pragma once

#include <BWAPI.h>

#include <BW/UnitType.h>
#include <BW/PlayerType.h>
#include <BWAI/ReservedResources.h>
namespace BWAPI { class Player; }
namespace BWAI
{
  class Unit;
  /** Represents one player in game. Note that there is always player 12 who owns resources. */
  class Player
  {
  public:
    Player(BWAPI::Player* player);

    u8 getID();
    BWAPI::Race getRace();
    BW::PlayerType::Enum playerType();

    int getMinerals() const;
    int getGas() const;
       
    int getSuppliesAvailable() const;
    int getSuppliesUsed() const;
    int getSuppliesFree() const;

    int getAllUnits(BW::UnitType unit) const;
    int getCompletedUnits(BW::UnitType unit) const;
    int getCompletedUnits(BW::UnitType unit, BWAPI::Race race) const;
    int getIncompleteUnits(BW::UnitType unit) const;
    int getDeaths(BW::UnitType unit) const;
    int getKills(BW::UnitType unit) const;
    bool researchInProgress(BWAPI::TechType tech) const;    
    bool techResearched(BWAPI::TechType tech) const;
    int upgradeLevel(BWAPI::UpgradeType upgrade) const;
    bool upgradeInProgress(BWAPI::UpgradeType upgrade) const; 

    bool canBuild(BW::UnitType unit) const;
    bool canAfford(BW::UnitType unit) const;
    bool canAffordNow(BW::UnitType unit) const;
    bool canAfford(BWAPI::TechType tech) const;
    bool canAfford(BWAPI::UpgradeType upgrade, int level) const;
    const BWAPI::Player* getPlayer() const;
  private:
    BWAPI::Player* player;
  };
}