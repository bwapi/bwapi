#pragma once

#include <BWAPI/IDs.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/Race.h>
#include <BWAPI/Color.h>

namespace BWAPI
{
  class Game;
  struct PlayerData
  {
    PlayerData(Game &g, PlayerID pid, ForceID force) : game{ g }, id{ pid }, force{ force } { }
    BWAPI::PlayerID id;
    BWAPI::Game &game;
    std::string name;
    BWAPI::Race race;
    BWAPI::PlayerType type;
    BWAPI::ForceID force;
    bool isAlly[12];
    bool isEnemy[12];
    bool isNeutral;
    int  startLocationX;
    int  startLocationY;
    bool isVictorious;
    bool isDefeated;
    bool leftGame;
    bool isParticipating;

    int minerals;
    int gas;
    int gatheredMinerals;
    int gatheredGas;
    int repairedMinerals;
    int repairedGas;
    int refundedMinerals;
    int refundedGas;
    int supplyTotal[3];
    int supplyUsed[3];

    int allUnitCount[UnitTypes::Enum::MAX];
    int visibleUnitCount[UnitTypes::Enum::MAX];
    int completedUnitCount[UnitTypes::Enum::MAX];
    int deadUnitCount[UnitTypes::Enum::MAX];
    int killedUnitCount[UnitTypes::Enum::MAX];

    int  upgradeLevel[UpgradeTypes::Enum::MAX];
    bool hasResearched[TechTypes::Enum::MAX];
    bool isResearching[TechTypes::Enum::MAX];
    bool isUpgrading[UpgradeTypes::Enum::MAX];

    BWAPI::Color color;

    int totalUnitScore;
    int totalKillScore;
    int totalBuildingScore;
    int totalRazingScore;
    int customScore;

    int   maxUpgradeLevel[UpgradeTypes::Enum::MAX];
    bool  isResearchAvailable[TechTypes::Enum::MAX];
    bool  isUnitAvailable[UnitTypes::Enum::MAX];
  };
}
