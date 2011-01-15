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
    //revision and debug will stay at the top of struct so they don't move in memory from revision to revision.
    int revision;
    bool isDebug;

    int instanceID;
    int botAPM_noselects;
    int botAPM_selects;

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
    int enemy;
    int neutral;

	  static const int MAX_EVENTS = 10000;
	  static const int MAX_STRINGS = 20000;
	  static const int MAX_SHAPES = 20000;
	  static const int MAX_COMMANDS = 20000;
	  static const int MAX_UNIT_COMMANDS = 20000;

    //events from server to client
    int eventCount;
    BWAPIC::Event events[MAX_EVENTS];

    //strings (used in events, shapes, and commands)
    int stringCount;
    char strings[MAX_STRINGS][256];

    //shapes, commands, unitCommands, from client to server
    int shapeCount;
    BWAPIC::Shape shapes[MAX_SHAPES];

    int commandCount;
    BWAPIC::Command commands[MAX_COMMANDS];

    int unitCommandCount;
    BWAPIC::UnitCommand unitCommands[MAX_UNIT_COMMANDS];

  };
}
