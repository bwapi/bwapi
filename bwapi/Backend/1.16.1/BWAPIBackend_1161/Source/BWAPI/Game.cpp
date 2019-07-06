#include <BWAPI/Position.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Region.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Player.h>

#include <BWAPI/UnitSizeType.h>
#include <BWAPI/DamageType.h>
#include <BWAPI/ExplosionType.h>
#include <BWAPI/WeaponType.h>

#include <cstdarg>

// Needed by other compilers.
#include <cstring>

#include "GameImpl.h"

namespace BWAPI
{
  //------------------------------------ DAMAGE CALCULATION ------------------------------------------
  void GameImpl::setScreenPosition(BWAPI::Position p)
  {
    this->setScreenPosition(p.x, p.y);
  }
  void GameImpl::pingMinimap(BWAPI::Position p)
  {
    this->pingMinimap(p.x, p.y);
  }
  void GameImpl::vSendText(const char *format, va_list arg)
  {
    this->vSendTextEx(false, format, arg);
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
  bool GameImpl::isWalkable(BWAPI::WalkPosition position) const
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
  Unitset GameImpl::getUnitsOnTile(int tileX, int tileY, const UnitFilter &pred) const
  {
    return this->getUnitsOnTile(TilePosition(tileX,tileY), pred);
  }
  Unitset GameImpl::getUnitsOnTile(BWAPI::TilePosition tile, const UnitFilter &pred) const
  {
    if ( !tile )  // if tileposition not valid
      return Unitset::none;

    Position p(tile); // convert to pixel position
    return this->getUnitsInRectangle(p.x, p.y, p.x + 32, p.y + 32, pred);
  }
  Unitset GameImpl::getUnitsInRadius(int x, int y, int radius, const UnitFilter &pred) const
  {
    return this->getUnitsInRectangle(x - radius,
                                     y - radius,
                                     x + radius,
                                     y + radius,
                                     [&x,&y,&radius,&pred](Unit u){ return u->getDistance(Position(x,y)) <= radius && (!pred.isValid() || pred(u)); });
  }
  Unitset GameImpl::getUnitsInRadius(Position center, int radius, const UnitFilter &pred) const
  {
    return this->getUnitsInRadius(center.x, center.y, radius, pred);
  }
  Unitset GameImpl::getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight, const UnitFilter &pred) const
  {
    return this->getUnitsInRectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, pred);
  }
  Unit GameImpl::getClosestUnit(Position center, const UnitFilter &pred, int radius) const
  {
    return this->getClosestUnitInRectangle(center,
                                            [&](Unit u){ return u->getDistance(center) <= radius && (!pred.isValid() || pred(u));},
                                            center.x - radius,
                                            center.y - radius,
                                            center.x + radius,
                                            center.y + radius);
  }
  //------------------------------------------ REGIONS -----------------------------------------------
  BWAPI::Region GameImpl::getRegionAt(BWAPI::Position position) const
  {
    return this->getRegionAt(position.x, position.y);
  }
  bool GameImpl::hasPath(Position source, Position destination) const
  {
    if (source.isValid() && destination.isValid())
    {
      Region rgnA = getRegionAt(source);
      Region rgnB = getRegionAt(destination);
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
  void GameImpl::drawTextMap(int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Map, x, y, format, ap);
    va_end(ap);
  }
  void GameImpl::drawTextMouse(int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Mouse, x, y, format, ap);
    va_end(ap);
  }
  void GameImpl::drawTextScreen(int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Screen, x, y, format, ap);
    va_end(ap);
  }
  void GameImpl::drawTextMap(Position p, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Map, p.x, p.y, format, ap);
    va_end(ap);
  }
  void GameImpl::drawTextMouse(Position p, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Mouse, p.x, p.y, format, ap);
    va_end(ap);
  }
  void GameImpl::drawTextScreen(Position p, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Screen, p.x, p.y, format, ap);
    va_end(ap);
  }
  //------------------------------------------ DRAW BOX -----------------------------------------------
  void GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    this->drawBox(CoordinateType::Map, left, top, right, bottom, color, isSolid);
  }
  void GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    this->drawBox(CoordinateType::Mouse, left, top, right, bottom, color, isSolid);
  }
  void GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    this->drawBox(CoordinateType::Screen, left, top, right, bottom, color, isSolid);
  }
  void GameImpl::drawBoxMap(Position leftTop, Position rightBottom, Color color, bool isSolid)
  {
    this->drawBoxMap(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y, color, isSolid);
  }
  void GameImpl::drawBoxMouse(Position leftTop, Position rightBottom, Color color, bool isSolid)
  {
    this->drawBoxMouse(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y, color, isSolid);
  }
  void GameImpl::drawBoxScreen(Position leftTop, Position rightBottom, Color color, bool isSolid)
  {
    this->drawBoxScreen(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y, color, isSolid);
  }
  //------------------------------------------ DRAW TRIANGLE -----------------------------------------------
  void GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    this->drawTriangle(CoordinateType::Map, ax, ay, bx, by, cx, cy, color, isSolid);
  }
  void GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    this->drawTriangle(CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color, isSolid);
  }
  void GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    this->drawTriangle(CoordinateType::Screen, ax, ay, bx, by, cx, cy, color, isSolid);
  }
  void GameImpl::drawTriangleMap(Position a, Position b, Position c, Color color, bool isSolid)
  {
    this->drawTriangleMap(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
  }
  void GameImpl::drawTriangleMouse(Position a, Position b, Position c, Color color, bool isSolid)
  {
    this->drawTriangleMouse(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
  }
  void GameImpl::drawTriangleScreen(Position a, Position b, Position c, Color color, bool isSolid)
  {
    this->drawTriangleScreen(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
  }
  //------------------------------------------ DRAW CIRCLE -----------------------------------------------
  void GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    this->drawCircle(CoordinateType::Map, x, y, radius, color, isSolid);
  }
  void GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    this->drawCircle(CoordinateType::Mouse, x, y, radius, color, isSolid);
  }
  void GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    this->drawCircle(CoordinateType::Screen, x, y, radius, color, isSolid);
  }
  void GameImpl::drawCircleMap(Position p, int radius, Color color, bool isSolid)
  {
    this->drawCircleMap(p.x, p.y, radius, color, isSolid);
  }
  void GameImpl::drawCircleMouse(Position p, int radius, Color color, bool isSolid)
  {
    this->drawCircleMouse(p.x, p.y, radius, color, isSolid);
  }
  void GameImpl::drawCircleScreen(Position p, int radius, Color color, bool isSolid)
  {
    this->drawCircleScreen(p.x, p.y, radius, color, isSolid);
  }
  //------------------------------------------ DRAW ELLIPSE -----------------------------------------------
  void GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipse(CoordinateType::Map, x, y, xrad, yrad, color, isSolid);
  }
  void GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipse(CoordinateType::Mouse, x, y, xrad, yrad, color, isSolid);
  }
  void GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipse(CoordinateType::Screen, x, y, xrad, yrad, color, isSolid);
  }
  void GameImpl::drawEllipseMap(Position p, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipseMap(p.x, p.y, xrad, yrad, color, isSolid);
  }
  void GameImpl::drawEllipseMouse(Position p, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipseMouse(p.x, p.y, xrad, yrad, color, isSolid);
  }
  void GameImpl::drawEllipseScreen(Position p, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipseScreen(p.x, p.y, xrad, yrad, color, isSolid);
  }
  //------------------------------------------ DRAW DOT -----------------------------------------------
  void GameImpl::drawDotMap(int x, int y, Color color)
  {
    this->drawDot(CoordinateType::Map, x, y, color);
  }
  void GameImpl::drawDotMouse(int x, int y, Color color)
  {
    this->drawDot(CoordinateType::Mouse, x, y, color);
  }
  void GameImpl::drawDotScreen(int x, int y, Color color)
  {
    this->drawDot(CoordinateType::Screen, x, y, color);
  }
  void GameImpl::drawDotMap(Position p, Color color)
  {
    this->drawDotMap(p.x, p.y, color);
  }
  void GameImpl::drawDotMouse(Position p, Color color)
  {
    this->drawDotMouse(p.x, p.y, color);
  }
  void GameImpl::drawDotScreen(Position p, Color color)
  {
    this->drawDotScreen(p.x, p.y, color);
  }
  //------------------------------------------ DRAW LINE -----------------------------------------------
  void GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    this->drawLine(CoordinateType::Map, x1, y1, x2, y2, color);
  }
  void GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    this->drawLine(CoordinateType::Mouse, x1, y1, x2, y2, color);
  }
  void GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    this->drawLine(CoordinateType::Screen, x1, y1, x2, y2, color);
  }
  void GameImpl::drawLineMap(Position a, Position b, Color color)
  {
    this->drawLineMap(a.x, a.y, b.x, b.y, color);
  }
  void GameImpl::drawLineMouse(Position a, Position b, Color color)
  {
    this->drawLineMouse(a.x, a.y, b.x, b.y, color);
  }
  void GameImpl::drawLineScreen(Position a, Position b, Color color)
  {
    this->drawLineScreen(a.x, a.y, b.x, b.y, color);
  }

}

