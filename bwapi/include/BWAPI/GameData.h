#pragma once

#include <string>
#include <vector>

#include <BWAPI/IDs.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  struct GameData
  {
    int apiVersion;
    std::string engine;
    std::string engineVersion;
    bool tournament;

    int gameType;
    int frameCount;
    int latencyFrames;
    int turnSize;
    int gameSpeed;
    int frameSkip;
    int remainingLatencyFrames;
    int lastEventTime;

    std::vector<PlayerID> replayVisionPlayers;

    float latencyTime;
    float remainingLatencyTime;
    float elapsedTime;
    float millisecondsPerFrame;
    float averageFPS;

    int countdownTimer;
    bool isPaused;
    bool isInGame;
    bool isMultiplayer;
    bool isBattleNet;
    bool isReplay;
    bool clientUnitSelection;
    bool hasGUI;

    std::string mapPath;
    std::string mapName;
    std::string mapFileName;
    std::string gameName;
    std::string randomSeed;

    TilePosition::list startPositions;

    std::vector<RegionID> regions;

    PlayerID player;

    Position screenSize;
    Position screenPosition;

    struct MapData
    {
      TilePosition size;
      int tileset;
      std::string mapHash;

      int groundHeight[256][256];
      bool isBuildable[256][256];
      bool isVisible[256][256];
      bool isExplored[256][256];
      bool hasCreep[256][256];
      bool isOccupied[256][256];

      bool isWalkable[1024][1024];

      unsigned short mapTileRegionId[256][256];
      unsigned short mapSplitTilesMiniTileMask[5000];
      unsigned short mapSplitTilesRegion1[5000];
      unsigned short mapSplitTilesRegion2[5000];
    };

    MapData map;
  };
}
