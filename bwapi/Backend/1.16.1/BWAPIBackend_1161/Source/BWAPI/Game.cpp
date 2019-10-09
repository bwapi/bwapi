#include <BWAPI4/Position.h>
#include <BWAPI4/CoordinateType.h>
#include <BWAPI4/Color.h>
#include <BWAPI4/Unitset.h>
#include <BWAPI/RegionImpl.h>

#include <BWAPI4/UnitSizeType.h>
#include <BWAPI4/DamageType.h>
#include <BWAPI4/ExplosionType.h>
#include <BWAPI4/WeaponType.h>

#include <cstdarg>

// Needed by other compilers.
#include <cstring>

#include "GameImpl.h"

namespace BWAPI4
{
  //------------------------------------ DAMAGE CALCULATION ------------------------------------------
  void GameImpl::setScreenPosition(BWAPI4::Position p)
  {
    this->setScreenPosition(p.x, p.y);
  }
  void GameImpl::pingMinimap(BWAPI4::Position p)
  {
    this->pingMinimap(p.x, p.y);
  }
  void GameImpl::sendText(const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vSendTextEx(false, format, ap);
    va_end(ap);
  }
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vSendTextEx(toAllies, format, ap);
    va_end(ap);
  };
  void GameImpl::printf(const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vPrintf(format, ap);
    va_end(ap);
  };
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPower(int tileX, int tileY, UnitType unitType) const
  {
    if ( unitType.isValid() && unitType < UnitTypes::None )
      return hasPowerPrecise( tileX*32 + unitType.tileWidth()*16, tileY*32 + unitType.tileHeight()*16, unitType);
    return hasPowerPrecise( tileX*32, tileY*32, UnitTypes::None);
  }
  bool GameImpl::hasPower(TilePosition position, UnitType unitType) const
  {
    return hasPower(position.x, position.y, unitType);
  }
  bool GameImpl::hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPowerPrecise( tileX*32 + tileWidth*16, tileY*32 + tileHeight*16, unitType);
  }
  bool GameImpl::hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPower(position.x, position.y, tileWidth, tileHeight, unitType);
  }
  bool GameImpl::hasPowerPrecise(Position position, UnitType unitType) const
  {
    return hasPowerPrecise(position.x, position.y, unitType);
  }
  //------------------------------------------ MAP DATA -----------------------------------------------
  bool GameImpl::isWalkable(BWAPI4::WalkPosition position) const
  {
    return this->isWalkable(position.x, position.y);
  }
  int GameImpl::getGroundHeight(TilePosition position) const
  {
    return this->getGroundHeight(position.x, position.y);
  }
  bool GameImpl::isBuildable(TilePosition position, bool includeBuildings) const
  {
    return this->isBuildable(position.x, position.y, includeBuildings);
  }
  bool GameImpl::isVisible(TilePosition position) const
  {
    return this->isVisible(position.x, position.y);
  }
  bool GameImpl::isExplored(TilePosition position) const
  {
    return this->isExplored(position.x, position.y);
  }
  bool GameImpl::hasCreep(TilePosition position) const
  {
    return this->hasCreep(position.x, position.y);
  }
  Unitset GameImpl::getUnitsOnTile(int tileX, int tileY, std::function<bool(Unit)> pred) const
  {
    return this->getUnitsOnTile(TilePosition(tileX,tileY), pred);
  }
  Unitset GameImpl::getUnitsOnTile(BWAPI4::TilePosition tile, std::function<bool(Unit)> pred) const
  {
    if ( !tile )  // if tileposition not valid
      return Unitset::none;

    Position p(tile); // convert to pixel position
    return this->getUnitsInRectangle(p.x, p.y, p.x + 32, p.y + 32, pred);
  }
  Unitset GameImpl::getUnitsInRectangle(BWAPI4::Position topLeft, BWAPI4::Position bottomRight, std::function<bool(Unit)> pred) const
  {
    return this->getUnitsInRectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, pred);
  }
  //------------------------------------------ REGIONS -----------------------------------------------
  BWAPI4::RegionImpl* GameImpl::getRegionAt(BWAPI4::Position position) const
  {
    return this->getRegionAt(position.x, position.y);
  }
  bool GameImpl::hasPath(Position source, Position destination) const
  {
    if (source.isValid() && destination.isValid())
    {
      RegionImpl* rgnA = getRegionAt(source);
      RegionImpl* rgnB = getRegionAt(destination);
      if (rgnA && rgnB && rgnA->getRegionGroupID() == rgnB->getRegionGroupID())
        return this->setLastError();
    }
    return this->setLastError(Errors::Unreachable_Location);
  }
  //------------------------------------------ DRAW TEXT ----------------------------------------------
  void GameImpl::drawText(CoordinateType::Enum ctype, int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(ctype, x, y, format, ap);
    va_end(ap);
  }
}

