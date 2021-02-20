#pragma once

#include "UnitCommand.h"
#include "PlayerData.h"
#include "RegionData.h"
#include "UnitData.h"
#include "BulletData.h"
#include "../Event.h"
#include "Shape.h"

#include <BWAPI4/Flag.h>
#include <BWAPI4/Input.h>

namespace BWAPIC4
{
  struct Position
  {
    Position() {x=0;y=0;}
    int x;
    int y;
  };
}
namespace BWAPI4
{
  struct GameData
  {
    //revision and debug will stay at the top of struct so they don't move in memory from revision to revision.
    int client_version;
    int revision;
    bool isDebug;

    int instanceID;
    int botAPM_noselects;
    int botAPM_selects;

    //players
    int playerCount;
    PlayerData players[12];

    //units
    int initialUnitCount;
    UnitData units[10000];

    //unit table
    int unitArray[1700];

    //bullets
    BulletData bullets[100];

    // nuke dots
    int nukeDotCount;
    BWAPIC4::Position nukeDots[200];

    int gameType;
    int latency;
    int latencyFrames;
    int latencyTime;
    int remainingLatencyFrames;
    int remainingLatencyTime;
    bool hasLatCom;
    bool hasGUI;
    int replayFrameCount;
    unsigned randomSeed;
    int frameCount;
    int elapsedTime;
    int countdownTimer;
    int fps;
    double averageFPS;

    // user input
    int mouseX;
    int mouseY;
    bool mouseState[M_MAX];
    bool keyState[K_MAX];
    int screenX;
    int screenY;

    bool flags[BWAPI4::Flag::Max];

    // map
    int mapWidth;
    int mapHeight;
    char mapFileName[261];  //size based on broodwar memory
    char mapPathName[261];  //size based on broodwar memory
    char mapName[33];      //size based on broodwar memory
    char mapHash[41];

    //tile data
    int  getGroundHeight[256][256];
    bool isWalkable[1024][1024]; 
    bool isBuildable[256][256];
    bool isVisible[256][256];
    bool isExplored[256][256];
    bool hasCreep[256][256];
    bool isOccupied[256][256];

    unsigned short mapTileRegionId[256][256];
    unsigned short mapSplitTilesMiniTileMask[5000];
    unsigned short mapSplitTilesRegion1[5000];
    unsigned short mapSplitTilesRegion2[5000];

    int regionCount;
    RegionData regions[5000];

    // start locations
    int startLocationCount;
    BWAPIC4::Position startLocations[8];

    // match mode
    bool isInGame;
    bool isMultiplayer;
    bool isBattleNet;
    bool isPaused;
    bool isReplay;

    //selected units
    int selectedUnitCount;
    int selectedUnits[12];

    // players
    int self;
    int enemy;
    int neutral;
  };
}
