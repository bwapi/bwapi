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
  BWAPI_FUNCTION void drawTextScreen      (Position pos, const char* text);
  BWAPI_FUNCTION void drawText            (Position pos, const char* text);
  BWAPI_FUNCTION void drawRectangleScreen (Position pos, Position size, int color, bool solid);
  BWAPI_FUNCTION void drawRectangle       (Position pos, Position size, int color, bool solid);
  BWAPI_FUNCTION void drawCircleScreen    (Position pos, int r, int color, bool solid);
  BWAPI_FUNCTION void drawCircle          (Position pos, int r, int color, bool solid);
  BWAPI_FUNCTION void drawEllipseScreen   (Position pos, int w, int h, int color, bool solid);
  BWAPI_FUNCTION void drawEllipse         (Position pos, int w, int h, int color, bool solid);
  BWAPI_FUNCTION void drawLineScreen      (Position pos, Position pos2, int color);
  BWAPI_FUNCTION void drawLine            (Position pos, Position pos2, int color);
  BWAPI_FUNCTION void drawDotScreen       (Position pos, int color);
  BWAPI_FUNCTION void drawDot             (Position pos, int color);
  BWAPI_FUNCTION void drawTriangleScreen  (Position posa, Position posb, Position posc, int color, bool solid);
  BWAPI_FUNCTION void drawTriangle        (Position posa, Position posb, Position posc, int color, bool solid);

  // commands
  BWAPI_FUNCTION void startGame();
  BWAPI_FUNCTION void pauseGame();
  BWAPI_FUNCTION void leaveGame();
  BWAPI_FUNCTION void resumeGame();
  BWAPI_FUNCTION void restartGame();
  BWAPI_FUNCTION void setLocalSpeed(int speed);
  BWAPI_FUNCTION void pingMinimap(Position pos);
  BWAPI_FUNCTION void enableFlag(Flag flag);

  // unit commands
  BWAPI_FUNCTION void orderAttackPosition(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderAttackUnit(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderRightClickPosition(UnitId unitId, Position pos);
  BWAPI_FUNCTION void orderRightClickUnit(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderTrain(UnitId unitId, UnitTypeId what);
  BWAPI_FUNCTION void orderBuild(UnitId unitId, UnitTypeId what);
  BWAPI_FUNCTION void orderBuildAddon(UnitId unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void orderResearch(UnitId unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void orderUpgrade(UnitId unitId, BWAPI::UpgradeTypeId what);
  BWAPI_FUNCTION void orderStop(UnitId unitId);
  BWAPI_FUNCTION void orderHoldPosition(UnitId unitId);
  BWAPI_FUNCTION void orderPatrol(UnitId unitId);
  BWAPI_FUNCTION void orderFollow(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderSetRallyPosition(UnitId unitId);
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
  BWAPI_FUNCTION void orderLand(UnitId unitId);
  BWAPI_FUNCTION void orderLoad(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderUnload(UnitId unitId, UnitId targetUnitId);
  BWAPI_FUNCTION void orderUnloadAll(UnitId unitId);
  BWAPI_FUNCTION void orderUnloadAllPosition(UnitId unitId);
  BWAPI_FUNCTION void orderCancelConstruction(UnitId unitId);
  BWAPI_FUNCTION void orderHaltConstruction(UnitId unitId);
  BWAPI_FUNCTION void orderCancelMorph(UnitId unitId);
  BWAPI_FUNCTION void orderCancelTrain(UnitId unitId);
  BWAPI_FUNCTION void orderCancelTrainSlot(UnitId unitId, int slotId);
  BWAPI_FUNCTION void orderCancelAddon(UnitId unitId);
  BWAPI_FUNCTION void orderCancelResearch(UnitId unitId);
  BWAPI_FUNCTION void orderCancelUpgrade(UnitId unitId);
  BWAPI_FUNCTION void orderUseTech(UnitId unitId);
  BWAPI_FUNCTION void orderUseTechPosition(UnitId unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void orderUseTechUnit(UnitId unitId, BWAPI::TechTypeId what, UnitId targetUnitId);
}
