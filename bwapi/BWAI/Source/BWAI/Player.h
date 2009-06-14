#pragma once

#include <BWAPI.h>
#include <BWAI/ReservedResources.h>
#include <BWAI/UnitTypeGroup.h>
namespace BWAI
{
  class Unit;
  /** Represents one player in game. Note that there is always player 12 who owns resources. */
  class Player
  {
  public:
    Player(BWAPI::Player* player);

    int getID();
    BWAPI::Race getRace();
    BWAPI::PlayerType playerType();

    int getMinerals() const;
    int getGas() const;
       
    int getSuppliesAvailable() const;
    int getSuppliesUsed() const;
    int getSuppliesFree() const;
    BWAPI::TilePosition getStartLocation() const;

    int getAllUnits(BWAPI::UnitType unit) const;
    int getCompletedUnits(BWAPI::UnitType unit) const;
    int getIncompleteUnits(BWAPI::UnitType unit) const;
    int getDeaths(BWAPI::UnitType unit) const;
    int getKills(BWAPI::UnitType unit) const;

    int getAllUnits(BWAI::UnitTypeGroup::Enum group) const;
    int getCompletedUnits(BWAI::UnitTypeGroup::Enum group) const;
    int getIncompleteUnits(BWAI::UnitTypeGroup::Enum group) const;
    int getDeaths(BWAI::UnitTypeGroup::Enum group) const;
    int getKills(BWAI::UnitTypeGroup::Enum group) const;


    bool researchInProgress(BWAPI::TechType tech) const;    
    bool techResearched(BWAPI::TechType tech) const;
    int upgradeLevel(BWAPI::UpgradeType upgrade) const;
    bool upgradeInProgress(BWAPI::UpgradeType upgrade) const; 

    bool canBuild(BWAPI::UnitType unit) const;
    bool canAfford(BWAPI::UnitType unit) const;
    bool canAffordNow(BWAPI::UnitType unit) const;
    bool canAfford(BWAPI::TechType tech) const;
    bool canAfford(BWAPI::UpgradeType upgrade, int level) const;
    const BWAPI::Player* getPlayer() const;
  private:
    BWAPI::Player* player;
  };
}