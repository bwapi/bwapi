#pragma once
#include <BWAPI.h>
#include <array>
class BuildingPlacer
{
  public:
    static bool canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist = 2);
    static BWAPI::TilePosition getBuildLocationNear(BWAPI::TilePosition position,BWAPI::UnitType type, int buildDist = 2);
    static bool buildable(BWAPI::TilePosition position);
};