#pragma once

#include "..\BWAPITypes\UnitCommands.h"
#include "..\BWAPITypes\UnitStruct.h"
namespace Bridge
{
  struct StaticGameDataStructure
  {
    int getLatency;
    int frameCount;
    int getMouseX;
    int getMouseY;
    int getScreenX;
    int getScreenY;
    int mapWidth;
    int mapHeight;
    char mapFilename[260];//size based on broodwar memory
    char mapName[32];//size based on broodwar memory
    int mapHash;
    int getGroundHeight[1024][1024];
    bool isWalkable[1024][1024];
    bool isBuildable[256][256];
    bool isVisible[256][256];
    bool isExplored[256][256];
    bool hasCreep[256][256];
    bool isMultiplayer;
    bool isReplay;
    bool isPaused;
    BWAPI::UnitCommand commandQueue[10000];//may replace with resizable structure if needed
    int lastFreeCommandSlot;
    BWAPI::StateBasic           unitDataBasic[10000];     //holds all units that are not detected (these units could be dead, or simply invisible and undetected)
    int basicCount;
    BWAPI::StateDetected        unitDataDetected[1700];   //holds all detected units that are not visible - limit based on actual starcraft limit
    int detectedCount;
    BWAPI::StateVisible         unitDataVisible[1700];    //holds all visible units that are not fully observable - limit based on actual starcraft limit
    int visibleCount;
    BWAPI::StateFullyObservable unitDataFullyObservable[1700]; //holds all fully observable units - limit based on actual starcraft limit
    int fullyObservableCount;

    //note: if Complete Map Information is enabled, then all units will either be StateFullyObservable, or StateBasic (dead).
  };
}
