#pragma once

#include "UnitCommand.h"
#include "ForceData.h"
#include "PlayerData.h"
#include "UnitData.h"
#include "Event.h"
#include "Command.h"
#include "Shape.h"

namespace BWAPIC
{
  struct GameData
  {
    int frameCount;

    // BWAPI event related
    bool isOnMatchStart;
    bool isFromBeginning;

    // user input
    int mouseX;
    int mouseY;
    int screenX;
    int screenY;

    // map
    int mapWidth;
    int mapHeight;
    char mapFilename[260];  //size based on broodwar memory
    char mapName[32];      //size based on broodwar memory

    //userInput?
    char userInput[256];    // size based on experiments. 78 in bw, 119 in mp chat room, 255 in bnet

    int mapHash;
    int getGroundHeight[1024][1024];
    bool isWalkable[1024][1024]; 
    bool isBuildable[256][256];
    bool isVisible[256][256];
    bool isExplored[256][256];
    bool hasCreep[256][256];

    // match mode
    bool isInGame;
    bool isMultiplayer;
    bool isReplay;
    bool isPaused;
    int latency;

    // players
    int self;

    int selectedUnitCount;
    int selectedUnits[12];

    // start locations
    int startLocationCount;
    int startLocationsX[8];
    int startLocationsY[8];

    int eventCount;
    Event events[1000];

    int stringCount;
    char strings[1000][256];

    int shapeCount;
    Shape shapes[10000];

    int commandCount;
    Command commands[10000];

    int unitCommandCount;
    UnitCommand unitCommands[10000];

    int forceCount;
    ForceData forces[4];

    int playerCount;
    PlayerData players[12];

    UnitData units[10000];

    bool mouseState[3];
    bool keyState[256];
  };
}
