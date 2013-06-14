#pragma once
#include <BWAPI.h>
#include <RectangleArray.h>
class BuildingPlacer
{
  public:
    BuildingPlacer();
    bool canBuildHere(BWAPI::TilePosition position, BWAPI::UnitType type) const;
    bool canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type) const;
    bool canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist) const;
    BWAPI::TilePosition getBuildLocation(BWAPI::UnitType type) const;
    BWAPI::TilePosition getBuildLocationNear(BWAPI::TilePosition position,BWAPI::UnitType type) const;
    BWAPI::TilePosition getBuildLocationNear(BWAPI::TilePosition position,BWAPI::UnitType type, int buildDist) const;
    bool buildable(BWAPI::TilePosition position) const;
    void reserveTiles(BWAPI::TilePosition position, int width, int height);
    void freeTiles(BWAPI::TilePosition position, int width, int height);
    void setBuildDistance(int distance);
    int getBuildDistance() const;
    bool isReserved(BWAPI::TilePosition position) const;
  private:
    Util::RectangleArray<bool> reserveMap;
    int buildDistance;
};