#pragma once

#include "UnitCommand.h"
#include "KnownUnit.h"
#include "UnitState.h"
#include "Player.h"
#include "Force.h"
#include "Position.h"
#include "BuildPosition.h"

#include <Util\StaticVector.h>
#include <Util\StaticSet.h>
#include <Util\StaticString.h>

namespace BWAPI
{
  typedef Util::StaticSet<Player, 12> PlayerArray;
  typedef Force ForceArray[4];
  typedef Util::StaticSet<KnownUnit, 1700> KnownUnitArray;
  typedef Util::StaticVector<BuildPosition, 8> StartLocationArray;

  struct StaticGameData
  {
    int frameCount;
    int getLatency;
    Position mousePosition;
    Position screenPosition;
    BuildPosition mapSize;
    Util::StaticString<260> mapFilename;  //size based on broodwar memory
    Util::StaticString<32>  mapName;      //size based on broodwar memory
    Util::StaticString<256> userInput;    // size based on experiments. 78 in bw, 119 in mp chat room, 255 in bnet
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
    PlayerArray players;
    ForceArray forces;
    KnownUnitArray units;
    StartLocationArray startLocations;
  };
}
