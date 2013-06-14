#include "BuildingPlacer.h"

#include <BWAPI/Unitset.h>

BuildingPlacer::BuildingPlacer()
{
  reserveMap.resize(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
  reserveMap.setTo(false);
  this->buildDistance = 1;
}
bool BuildingPlacer::canBuildHere(BWAPI::TilePosition position, BWAPI::UnitType type) const
{
  //returns true if we can build this type of unit here. Takes into account reserved tiles.
  if (!BWAPI::Broodwar->canBuildHere(position, type))
    return false;
  for ( int x = position.x; x < position.x + type.tileSize().x; ++x )
    for ( int y = position.y; y < position.y + type.tileSize().y; ++y )
    {
      if (reserveMap[x][y])
        return false;
    }
  return true;
}
bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type) const
{
  return canBuildHereWithSpace(position,type,this->buildDistance);
}
bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist) const
{
  //returns true if we can build this type of unit here with the specified amount of space.
  //space value is stored in this->buildDistance.

  //if we can't build here, we of course can't build here with space
  if (!this->canBuildHere(position, type))
    return false;

  int width  = type.tileWidth();
  int height = type.tileHeight();

  //make sure we leave space for add-ons. These types of units can have addons:
  if ( type.canBuildAddon() )
  {
    width += 2;
  }
  BWAPI::TilePosition start(position - BWAPI::TilePosition(buildDist,buildDist));
  BWAPI::TilePosition end(position + BWAPI::TilePosition(width, height) + BWAPI::TilePosition(buildDist,buildDist));
  if ( !start || !end )
    return false;

  for ( int x = start.x; x < end.x; ++x )
    for ( int y = start.y; y < end.y; ++y )
    {
      if ( !type.isRefinery() && (!buildable(BWAPI::TilePosition(x,y)) || reserveMap[x][y]) )
        return false;
    }

  if (position.x > 3)
  {
    BWAPI::Position start2( BWAPI::TilePosition(start.x-2, start.y) );
    BWAPI::Position end2( BWAPI::TilePosition(start.x, end.y) );
    if ( !BWAPI::Broodwar->getUnitsInRectangle(start2.makeValid(), end2.makeValid(), !BWAPI::Filter::IsLifted && BWAPI::Filter::CanBuildAddon).empty() )
      return false;
  }
  return true;
}
BWAPI::TilePosition BuildingPlacer::getBuildLocation(BWAPI::UnitType type) const
{
  //returns a valid build location if one exists, scans the map left to right
  int wid = BWAPI::Broodwar->mapWidth(), hgt = BWAPI::Broodwar->mapHeight();
  for ( int x = 0; x < wid; ++x )
    for ( int y = 0; y < hgt; ++y )
    {
      BWAPI::TilePosition tp(x,y);
      if (this->canBuildHere( tp, type))
        return tp;
    }
  return BWAPI::TilePositions::None;
}

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(BWAPI::TilePosition position, BWAPI::UnitType type) const
{
  return getBuildLocationNear(position, type,this->buildDistance);
}

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist) const
{
  //returns a valid build location near the specified tile position.
  //searches outward in a spiral.
  int length = 1;
  int j      = 0;
  bool first = true;
  int dx     = 0;
  int dy     = 1;
  while (length < BWAPI::Broodwar->mapWidth()) //We'll ride the spiral to the end
  {
    //if we can build here, return this tile position
    if ( position )
      if (this->canBuildHereWithSpace(position, type, buildDist))
        return position;

    //otherwise, move to another position
    position += BWAPI::TilePosition(dx, dy);
    //count how many steps we take in this direction
    j++;
    if (j == length) //if we've reached the end, its time to turn
    {
      //reset step counter
      j = 0;

      //Spiral out. Keep going.
      if (!first)
        length++; //increment step counter if needed

      //first=true for every other turn so we spiral out at the right rate
      first = !first;

      //turn counter clockwise 90 degrees:
      if (dx == 0)
      {
        dx = dy;
        dy = 0;
      }
      else
      {
        dy = -dx;
        dx = 0;
      }
    }
    //Spiral out. Keep going.
  }
  return BWAPI::TilePositions::None;
}

bool BuildingPlacer::buildable(BWAPI::TilePosition position) const
{
  //returns true if this tile is currently buildable, takes into account units on tile
  if ( !BWAPI::Broodwar->isBuildable(position) )
    return false;

  if ( !BWAPI::Broodwar->getUnitsOnTile(position, BWAPI::Filter::IsBuilding && !BWAPI::Filter::IsLifted).empty() )
    return false;

  return true;
}

void BuildingPlacer::reserveTiles(BWAPI::TilePosition position, int width, int height)
{
  BWAPI::TilePosition end(position + BWAPI::TilePosition(width,height));
  end.setMin((int)reserveMap.getWidth(), (int)reserveMap.getHeight());

  for ( int x = position.x; x < end.x; ++x )
    for ( int y = position.y; y < end.y; ++y )
      reserveMap[x][y] = true;
}

void BuildingPlacer::freeTiles(BWAPI::TilePosition position, int width, int height)
{
  BWAPI::TilePosition end(position + BWAPI::TilePosition(width,height));
  end.setMin((int)reserveMap.getWidth(), (int)reserveMap.getHeight());

  for ( int x = position.x; x < end.x; ++x )
    for ( int y = position.y; y < end.y; ++y )
      reserveMap[x][y] = false;
}

void BuildingPlacer::setBuildDistance(int distance)
{
  this->buildDistance = distance;
}
int BuildingPlacer::getBuildDistance() const
{
  return this->buildDistance;
}
bool BuildingPlacer::isReserved(BWAPI::TilePosition position) const
{
  if ( !position )
    return false;
  return reserveMap[position.x][position.y];
}