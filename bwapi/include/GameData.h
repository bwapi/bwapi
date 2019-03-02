#pragma once

#include <vector>

#include <BWAPI/IDs.h>
#include <BWAPI/Position.h>

namespace BWAPI {
  struct GameData {
    int apiVersion;
    std::string engine;
    std::string engineVersion;
    bool tournament;

    int gameType;
    int frame;
    int latencyFrames;
    int turnSize;
    int gameSpeed;
    int frameSkip;
    int remainingLatencyFrames;

    std::vector<PlayerID> replayVisionPlayers;

    float remainingLatencyTime;
    float elapsedTime;
    float millisecondsPerFrame;
    float averageFPS;

    int countdownTimer;
    bool isPaused;
    bool isInGame;
    bool isMultiplayer;
    bool clientUnitSelection;
    bool hasGUI;

    std::string mapPath;
    std::string mapName;
    std::string gameName;
    std::string randomSeed;

    std::vector<UnitID> units;
    std::vector<PlayerID> players;

    std::vector<RegionID> regions;
    std::vector<TilePosition> startPositions;

    PlayerID player;

    Position screenSize;
    Position screenPosition;

    struct MapData {
      TilePosition size;
      int tileset;

      int groundHeight[256][256];
      bool isBuildable[256][256];
      bool isVisible[256][256];
      bool isExplored[256][256];
      bool hasCreep[256][256];
      bool isOccupied[256][256];

      bool isWalkable[1024][1024];
    };

    MapData map;
  };
} // namespace BWAPI
