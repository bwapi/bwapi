#pragma once

#include "PlayerData.h"
#include "UnitData.h"

namespace BWAPI4
{
  struct GameData
  {
    //players
    PlayerData players[12];

    //units
    UnitData units[10000];

    bool hasLatCom;
    bool hasGUI;

    //tile data
    bool isVisible[256][256];
    bool isExplored[256][256];
    bool hasCreep[256][256];
    bool isOccupied[256][256];

    unsigned short mapSplitTilesRegion1[5000];
    unsigned short mapSplitTilesRegion2[5000];
  };
}
