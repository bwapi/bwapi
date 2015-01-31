#pragma once
#include <BWAPI.h>
#include <array>
class BuildingPlacer
{
  public:
    bool canBuildHere(BWAPI::TilePosition position, BWAPI::UnitType type) const;
    bool canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type) const;
    bool canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist) const;
    BWAPI::TilePosition getBuildLocation(BWAPI::UnitType type) const;
    BWAPI::TilePosition getBuildLocationNear(BWAPI::TilePosition position,BWAPI::UnitType type) const;
    BWAPI::TilePosition getBuildLocationNear(BWAPI::TilePosition position,BWAPI::UnitType type, int buildDist) const;
    static bool buildable(BWAPI::TilePosition position);
};