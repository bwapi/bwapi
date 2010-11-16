#pragma once

#include "UnitCommand.h"
#include "ForceData.h"
#include "PlayerData.h"
#include "UnitData.h"
#include "BulletData.h"
#include "Event.h"
#include "Command.h"
#include "Shape.h"
namespace BWAPIC
{
  struct Position
  {
    Position() {x=0;y=0;}
    int x;
    int y;
  };
}
namespace BWAPI
{
  struct GameData
  {
    GameData() {}
    int instanceID;

    //forces
    int forceCount;
    ForceData forces[5];

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
    BWAPIC::Position nukeDots[200];

    int gameType;
    int latency;
    int latencyFrames;
    int latencyTime;
    int remainingLatencyFrames;
    int remainingLatencyTime;
    int revision;
    bool isDebug;
    bool hasLatCom;
    int replayFrameCount;
    int frameCount;
    int fps;
    double averageFPS;

    // user input
    int mouseX;
    int mouseY;
    bool mouseState[3];
    bool keyState[256];
    int screenX;
    int screenY;

    bool flags[2];

    // map
    int mapWidth;
    int mapHeight;
    char mapFileName[261];  //size based on broodwar memory
    char mapPathName[261];  //size based on broodwar memory
    char mapName[33];      //size based on broodwar memory
    char mapHash[41];

    //tile data
    int getGroundHeight[256][256];
    bool isWalkable[1024][1024]; 
    bool isBuildable[256][256];
    bool isVisible[256][256];
    bool isExplored[256][256];
    bool hasCreep[256][256];

    unsigned short mapTileRegionId[256][256];
    unsigned short mapSplitTilesMiniTileMask[5000];
    unsigned short mapSplitTilesRegion1[5000];
    unsigned short mapSplitTilesRegion2[5000];
    unsigned short regionGroupIndex[5000];

    // start locations
    int startLocationCount;
    BWAPIC::Position startLocations[8];

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

    //events from server to client
    int eventCount;
    BWAPIC::Event events[10000];

    //strings (used in events, shapes, and commands)
    int stringCount;
    char strings[20000][256];

    //shapes, commands, unitCommands, from client to server
    int shapeCount;
    BWAPIC::Shape shapes[20000];

    int commandCount;
    BWAPIC::Command commands[20000];

    int unitCommandCount;
    BWAPIC::UnitCommand unitCommands[20000];

  };
}
