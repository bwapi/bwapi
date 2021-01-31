#pragma once
#include <gtest/gtest.h>

#include "MockClient.h"

#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/Unit.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include <BWAPI/GameType.h>

#include <string>
#include <set>

class GameFixture : public ::testing::Test {
public:
  GameFixture() : game(client)
  {
    constexpr int msPerFrame = 42;

    game.gameData->frameCount = 0;
    game.gameData->isPaused = false;

    game.gameData->apiVersion = 0;
    game.gameData->engine = "UnitTest";
    game.gameData->engineVersion = "1.0";
    game.gameData->tournament = false;

    game.gameData->gameType = BWAPI::GameTypes::Melee;
    game.gameData->frameCount = 0;
    game.gameData->latencyFrames = 2;
    //game.gameData->turnSize = ; ????
    game.gameData->gameSpeed = 6;
    game.gameData->frameSkip = 0;
    game.gameData->remainingLatencyFrames = 1;
    game.gameData->lastEventTime = 0;

    //game.gameData->replayVisionPlayers = ;

    game.gameData->latencyTime = 2 * msPerFrame;
    game.gameData->remainingLatencyTime = 1 * msPerFrame;
    game.gameData->elapsedTime = 0;
    game.gameData->millisecondsPerFrame = msPerFrame;
    game.gameData->averageFPS = 1000.0f / msPerFrame;

    game.gameData->countdownTimer = 0;
    game.gameData->isPaused = false;
    game.gameData->isInGame = true;
    game.gameData->isMultiplayer = false;
    game.gameData->isBattleNet = false;
    game.gameData->isReplay = false;
    //game.gameData->clientUnitSelection = ; ????? what is this?
    game.gameData->hasGUI = true;

    game.gameData->mapPath = "Test/Test.scx";
    game.gameData->mapName = "Test Map";
    game.gameData->mapFileName = "Test.scx";
    game.gameData->gameName = "Test Game";
    game.gameData->randomSeed = "0";

    game.gameData->startPositions.emplace_back(1, 1);
    game.gameData->startPositions.emplace_back(2, 2);
    game.gameData->startPositions.emplace_back(3, 3);
    game.gameData->startPositions.emplace_back(4, 4);
    game.gameData->startPositions.emplace_back(5, 5);
    game.gameData->startPositions.emplace_back(6, 6);
    game.gameData->startPositions.emplace_back(7, 7);
    game.gameData->startPositions.emplace_back(8, 8);

    //game.gameData->regions;

    game.gameData->player = BWAPI::PlayerID{ 0 };

    game.gameData->screenSize = BWAPI::Position{ 640, 480 };
    game.gameData->screenPosition = BWAPI::Positions::Origin;

    game.gameData->map.size = BWAPI::TilePosition{ 64, 64 };
    game.gameData->map.tileset = 0;
    game.gameData->map.mapHash = "deadbeefdeadbeefdeadbeefdeadbeefdeadbeef";

    BWAPI::Player self = createFakePlayer(0, BWAPI::PlayerTypes::Player, BWAPI::Races::Terran);
    BWAPI::Player enemy = createFakePlayer(1, BWAPI::PlayerTypes::Computer, BWAPI::Races::Zerg);
    //BWAPI::Player neutral = createFakePlayer(11, BWAPI::PlayerTypes::Neutral, BWAPI::Races::None);
  }

  BWAPI::Player createFakePlayer(int playerId, BWAPI::PlayerType playerType, BWAPI::Race race)
  {
    BWAPI::PlayerData& data = const_cast<BWAPI::PlayerData&>(*playerDataStore.emplace(game, BWAPI::PlayerID{ playerId }, BWAPI::ForceID::None).first);
   
    data.isParticipating = true;
    data.isNeutral = false;
    data.type = playerType;
    data.race = race;
    data.name = std::string("Player ") + std::to_string(playerId);

    std::fill(std::begin(data.isAlly), std::end(data.isAlly), false);
    data.isAlly[playerId] = true;

    std::fill(std::begin(data.isEnemy), std::end(data.isEnemy), true);
    data.isEnemy[playerId] = false;

    game.updatePlayer(data);

    return BWAPI::Player{ data };
  }

  BWAPI::Unit createFakeUnit(BWAPI::UnitType unitType = BWAPI::UnitTypes::Terran_Marine, int x = 100, int y = 100, int playerId = 0)
  {
    BWAPI::UnitID unitId = BWAPI::UnitID{ static_cast<int>(unitDataStore.size()) };
    BWAPI::UnitData& data = const_cast<BWAPI::UnitData&>(*unitDataStore.emplace(game, unitId).first);

    data.position = BWAPI::Position{ x, y };
    data.type = unitType;
    data.player = BWAPI::PlayerID{ playerId };
    data.exists = true;
    data.isCompleted = true;
    
    data.hitPoints = unitType.maxHitPoints();
    data.shields = unitType.maxShields();
    data.energy = unitType.maxEnergy();
    
    data.isIdle = true;
    data.isVisible[playerId] = true;
    data.order = BWAPI::Orders::Guard;
    data.secondaryOrder = BWAPI::Orders::Nothing;

    game.addUnit(data);
    
    return BWAPI::Unit{ data };
  }

  void withNoSelf() {
    game.gameData->player = BWAPI::PlayerID::None;
  }

  void withReplay() {
    game.gameData->isReplay = true;
  }
protected:

  MockClient client;
  BWAPI::Game game;

private:
  std::set<BWAPI::UnitData, BWAPI::IDCompare> unitDataStore;
  std::set<BWAPI::PlayerData, BWAPI::IDCompare> playerDataStore;
};

class GameFixture_SmallMap : public GameFixture {
public:
  GameFixture_SmallMap() : GameFixture()
  {
    game.gameData->map.size = BWAPI::TilePosition(64, 96);
    // TODO: Create a map w/ state
  }
};

class Game_SetAlliance : public GameFixture {};
class Game_SetVision : public GameFixture {};
class Game_SetLocalSpeed : public GameFixture {};
class Game_SendText : public GameFixture {};
class Game_SetTextSize : public GameFixture {};
class Game_DrawText : public GameFixture {};
class Game_DrawShape : public GameFixture {};
class Game_printf : public GameFixture {};
class Game_ostream : public GameFixture {};
