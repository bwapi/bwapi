#include "BuildingPlacer.h"

#include <BWAPI/Unitset.h>

bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist)
{
  //returns true if we can build this type of unit here with the specified amount of space.
  //space value is stored in this->buildDistance.

  //if we can't build here, we of course can't build here with space
  if (!BWAPI::Broodwar->canBuildHere(position, type))
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
      if ( !type.isRefinery() && !buildable(BWAPI::TilePosition(x,y)) )
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

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist)
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
      if (canBuildHereWithSpace(position, type, buildDist))
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

bool BuildingPlacer::buildable(BWAPI::TilePosition position)
{
  //returns true if this tile is currently buildable, takes into account units on tile
  if ( !BWAPI::Broodwar->isBuildable(position) )
    return false;

  if ( !BWAPI::Broodwar->getUnitsOnTile(position, BWAPI::Filter::IsBuilding && !BWAPI::Filter::IsLifted).empty() )
    return false;

  return true;
}
