#pragma once
#include "BWAPIInterface.h"

#include <BWAPITypes\UnitState.h>
#include <BWAPITypes\UnitEvent.h>
#include <BWAPITypes\StaticGameData.h>
#include <BWAPITypes\DynamicGameData.h>
#include <BWAPITypes\CallTypeId.h>
#include <BWAPITypes\Flag.h>

#include <BWAPITypes\UnitType.h>
#include <BWAPITypes\UnitTypeId.h>
#include <BWAPITypes\TechType.h>
#include <BWAPITypes\TechTypeId.h>
#include <BWAPITypes\UpgradeType.h>
#include <BWAPITypes\UpgradeTypeId.h>
#include <BWAPITypes\WeaponType.h>
#include <BWAPITypes\WeaponTypeId.h>
#include <BWAPITypes\DamageType.h>
#include <BWAPITypes\DamageTypeId.h>
#include <BWAPITypes\ExplosionType.h>
#include <BWAPITypes\ExplosionTypeId.h>
#include <BWAPITypes\Race.h>
#include <BWAPITypes\RaceId.h>
#include <BWAPITypes\UnitSizeType.h>
#include <BWAPITypes\UnitSizeTypeId.h>
#include <BWAPITypes\PlayerType.h>
#include <BWAPITypes\PlayerTypeId.h>
#include <BWAPITypes\CoordinateBase.h>

#include <string>

#include <windows.h>
#undef DrawText

namespace BWAPI
{
  // return BWAgent.dll revision number
  BWAPI_FUNCTION int getVersion();

  // returns zero instead throwing, if failed
  BWAPI_FUNCTION bool connect() throw();

  // returns when a callback would have been called.
  // returns the callback type
  BWAPI_FUNCTION CallTypeId waitForEvent();

  //----------------------------------- DATA QUERY --------------------------------------
  // information queries
  BWAPI_FUNCTION const StaticGameData*  getStaticGameData();
  BWAPI_FUNCTION const DynamicGameData* getDynamicGameData();

  // access directly through static data. Use this if error checking is needed
  BWAPI_FUNCTION const UnitState*       getUnit(UnitId unitId);
  BWAPI_FUNCTION const Player*          getPlayer(PlayerId playerId);

  // database related
  BWAPI_FUNCTION const UnitType*        getUnitType(UnitTypeId id);
  BWAPI_FUNCTION const TechType*        getTechType(TechTypeId id);
  BWAPI_FUNCTION const UpgradeType*     getUpgradeType(UpgradeTypeId id);
  BWAPI_FUNCTION const WeaponType*      getWeaponType(WeaponTypeId id);
  BWAPI_FUNCTION const DamageType*      getDamageType(DamageTypeId id);
  BWAPI_FUNCTION const ExplosionType*   getExplosionType(ExplosionTypeId id);
  BWAPI_FUNCTION const Race*            getRace(RaceId id);
  BWAPI_FUNCTION const UnitSizeType*    getUnitSizeType(UnitSizeTypeId id);
  BWAPI_FUNCTION const PlayerType*      getPlayerType(PlayerTypeId id);

  //----------------------------------- OPERATIONS --------------------------------------
  // text operations
  BWAPI_FUNCTION bool printText(const char* text);
  BWAPI_FUNCTION bool sendText(const char* text);

  // draw operations
  BWAPI_FUNCTION void drawText          (CoordinateBase base, Position pos, const char* text);
  BWAPI_FUNCTION void drawRectangle     (CoordinateBase base, Position pos, Position size, int color, bool solid);
  BWAPI_FUNCTION void drawCircle        (CoordinateBase base, Position pos, int r, int color, bool solid);
  BWAPI_FUNCTION void drawEllipse       (CoordinateBase base, Position pos, int w, int h, int color, bool solid);
  BWAPI_FUNCTION void drawLine          (CoordinateBase base, Position pos, Position pos2, int color);
  BWAPI_FUNCTION void drawDot           (CoordinateBase base, Position pos, int color);
  BWAPI_FUNCTION void drawTriangle      (CoordinateBase base, Position posa, Position posb, Position posc, int color, bool solid);

  // macroed draw operations
  static void inline drawTextMap        (Position pos, const char* text)
  {drawText(CoordinateBases::Map, pos, text);}
  static void inline drawTextScreen     (Position pos, const char* text)
  {drawText(CoordinateBases::Screen, pos, text);}
  static void inline drawTextMouse      (Position pos, const char* text)
  {drawText(CoordinateBases::Mouse, pos, text);}

  static void inline drawRectangleMap   (Position pos, Position size, int color, bool solid)
  {drawRectangle(CoordinateBases::Map, pos, size, color, solid);}
  static void inline drawRectangleScreen(Position pos, Position size, int color, bool solid)
  {drawRectangle(CoordinateBases::Screen, pos, size, color, solid);}
  static void inline drawRectangleMouse (Position pos, Position size, int color, bool solid)
  {drawRectangle(CoordinateBases::Mouse, pos, size, color, solid);}

  static void inline drawCircleMap      (Position pos, int r, int color, bool solid)
  {drawCircle(CoordinateBases::Map, pos, r, color, solid);}
  static void inline drawCircleScreen   (Position pos, int r, int color, bool solid)
  {drawCircle(CoordinateBases::Screen, pos, r, color, solid);}
  static void inline drawCircleMouse    (Position pos, int r, int color, bool solid)
  {drawCircle(CoordinateBases::Mouse, pos, r, color, solid);}

  static void inline drawEllipseMap     (Position pos, int w, int h, int color, bool solid)
  {drawEllipse(CoordinateBases::Map, pos, w, h, color, solid);}
  static void inline drawEllipseScreen  (Position pos, int w, int h, int color, bool solid)
  {drawEllipse(CoordinateBases::Screen, pos, w, h, color, solid);}
  static void inline drawEllipseMouse   (Position pos, int w, int h, int color, bool solid)
  {drawEllipse(CoordinateBases::Mouse, pos, w, h, color, solid);}

  static void inline drawLineMap        (Position pos, Position pos2, int color)
  {drawLine(CoordinateBases::Map, pos, pos2, color);}
  static void inline drawLineScreen     (Position pos, Position pos2, int color)
  {drawLine(CoordinateBases::Screen, pos, pos2, color);}
  static void inline drawLineMouse      (Position pos, Position pos2, int color)
  {drawLine(CoordinateBases::Mouse, pos, pos2, color);}

  static void inline drawDotMap         (Position pos, int color)
  {drawDot(CoordinateBases::Map, pos, color);}
  static void inline drawDotScreen      (Position pos, int color)
  {drawDot(CoordinateBases::Screen, pos, color);}
  static void inline drawDotMouse       (Position pos, int color)
  {drawDot(CoordinateBases::Mouse, pos, color);}

  static void inline drawTriangleMap    (Position posa, Position posb, Position posc, int color, bool solid)
  {drawTriangle(CoordinateBases::Map, posa, posb, posc, color, solid);}
  static void inline drawTriangleScreen (Position posa, Position posb, Position posc, int color, bool solid)
  {drawTriangle(CoordinateBases::Screen, posa, posb, posc, color, solid);}
  static void inline drawTriangleMouse  (Position posa, Position posb, Position posc, int color, bool solid)
  {drawTriangle(CoordinateBases::Mouse, posa, posb, posc, color, solid);}

  // commands
  BWAPI_FUNCTION void startGame();
  BWAPI_FUNCTION void pauseGame();
  BWAPI_FUNCTION void leaveGame();
  BWAPI_FUNCTION void resumeGame();
  BWAPI_FUNCTION void restartGame();
  BWAPI_FUNCTION void setLocalSpeed(int speed);
  BWAPI_FUNCTION void pingMinimap(Position pos);
  BWAPI_FUNCTION void enableFlag(Flag flag);
  BWAPI_FUNCTION void setScreenPosition(Position pos);

  // unit commands
  BWAPI_FUNCTION void orderAttackPosition(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderAttackUnit(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderRightClickPosition(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderRightClickUnit(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderTrain(UnitId unitId, UnitTypeId what);
  BWAPI_FUNCTION void orderBuild(UnitId unitId, Position pos, UnitTypeId what);
  BWAPI_FUNCTION void orderBuildAddon(UnitId unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void orderResearch(UnitId unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void orderUpgrade(UnitId unitId, BWAPI::UpgradeTypeId what);
  BWAPI_FUNCTION void orderStop(UnitId unitId);
  BWAPI_FUNCTION void orderHoldPosition(UnitId unitId);
  BWAPI_FUNCTION void orderPatrol(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderFollow(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderSetRallyPosition(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderSetRallyUnit(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderRepair(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderReturnCargo(UnitId unitId);
  BWAPI_FUNCTION void orderMorph(UnitId unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void orderBurrow(UnitId unitId);
  BWAPI_FUNCTION void orderUnburrow(UnitId unitId);
  BWAPI_FUNCTION void orderSiege(UnitId unitId);
  BWAPI_FUNCTION void orderUnsiege(UnitId unitId);
  BWAPI_FUNCTION void orderCloak(UnitId unitId);
  BWAPI_FUNCTION void orderDecloak(UnitId unitId);
  BWAPI_FUNCTION void orderLift(UnitId unitId);
  BWAPI_FUNCTION void orderLand(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderLoad(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderUnload(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderUnloadAll(UnitId unitId);
  BWAPI_FUNCTION void orderUnloadAllPosition(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderCancelConstruction(UnitId unitId);
  BWAPI_FUNCTION void orderHaltConstruction(UnitId unitId);
  BWAPI_FUNCTION void orderCancelMorph(UnitId unitId);
  BWAPI_FUNCTION void orderCancelTrain(UnitId unitId);
  BWAPI_FUNCTION void orderCancelTrainSlot(UnitId unitId, int slotId);
  BWAPI_FUNCTION void orderCancelAddon(UnitId unitId);
  BWAPI_FUNCTION void orderCancelResearch(UnitId unitId);
  BWAPI_FUNCTION void orderCancelUpgrade(UnitId unitId);
  BWAPI_FUNCTION void orderUseTech(UnitId unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void orderUseTechPosition(UnitId unitId, BWAPI::TechTypeId what, Position pos);
  BWAPI_FUNCTION void orderUseTechUnit(UnitId unitId, BWAPI::TechTypeId what, UnitId targetUnitId);
}
