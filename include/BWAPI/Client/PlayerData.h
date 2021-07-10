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

    PlayerData(const PlayerData& other) = delete;
    PlayerData& operator=(const PlayerData& other) = delete;


    BWAPI::PlayerID id;
    BWAPI::Game &game;
    std::string name;
    BWAPI::Race race{ Races::None };
    BWAPI::PlayerType type{ PlayerTypes::None };
    BWAPI::ForceID force{ ForceID::None };
    bool isAlly[12] = {};
    bool isEnemy[12] = {};
    bool isNeutral = false;
    int  startLocationX = 0;
    int  startLocationY = 0;
    bool isVictorious = false;
    bool isDefeated = false;
    bool leftGame = false;
    bool isParticipating = false;

    int minerals = 0;
    int gas = 0;
    int gatheredMinerals = 0;
    int gatheredGas = 0;
    int repairedMinerals = 0;
    int repairedGas = 0;
    int refundedMinerals = 0;
    int refundedGas = 0;
    int supplyTotal[3] = {};
    int supplyUsed[3] = {};

    int allUnitCount[UnitTypes::Enum::MAX] = {};
    int visibleUnitCount[UnitTypes::Enum::MAX] = {};
    int completedUnitCount[UnitTypes::Enum::MAX] = {};
    int deadUnitCount[UnitTypes::Enum::MAX] = {};
    int killedUnitCount[UnitTypes::Enum::MAX] = {};

    int  upgradeLevel[UpgradeTypes::Enum::MAX] = {};
    bool hasResearched[TechTypes::Enum::MAX] = {};
    bool isResearching[TechTypes::Enum::MAX] = {};
    bool isUpgrading[UpgradeTypes::Enum::MAX] = {};

    BWAPI::Color color{ Colors::Black };

    int totalUnitScore = 0;
    int totalKillScore = 0;
    int totalBuildingScore = 0;
    int totalRazingScore = 0;
    int customScore = 0;

    int   maxUpgradeLevel[UpgradeTypes::Enum::MAX] = {};
    bool  isResearchAvailable[TechTypes::Enum::MAX] = {};
    bool  isUnitAvailable[UnitTypes::Enum::MAX] = {};
  };
}
