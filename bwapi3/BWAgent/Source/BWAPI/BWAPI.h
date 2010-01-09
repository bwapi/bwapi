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
  BWAPI_FUNCTION void drawLineScreen      (Position pos, Position pos2, int color);
  BWAPI_FUNCTION void drawLine            (Position pos, Position pos2, int color);
  BWAPI_FUNCTION void drawDotScreen       (Position pos, int color);
  BWAPI_FUNCTION void drawDot             (Position pos, int color);

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
  BWAPI_FUNCTION void orderAttackPosition(int unitId, int x, int y);
  BWAPI_FUNCTION void orderAttackUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void orderRightClickPosition(int unitId, int x, int y);
  BWAPI_FUNCTION void orderRightClickUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void orderTrain(int unitId, UnitTypeId what);
  BWAPI_FUNCTION void orderBuild(int unitId, UnitTypeId what);
  BWAPI_FUNCTION void orderBuildAddon(int unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void orderResearch(int unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void orderUpgrade(int unitId, BWAPI::UpgradeTypeId what);
  BWAPI_FUNCTION void orderStop(int unitId);
  BWAPI_FUNCTION void orderHoldPosition(int unitId);
  BWAPI_FUNCTION void orderPatrol(int unitId);
  BWAPI_FUNCTION void orderFollow(int unitId, int targetUnitId);
  BWAPI_FUNCTION void orderSetRallyPosition(int unitId);
  BWAPI_FUNCTION void orderSetRallyUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void orderRepair(int unitId, int targetUnitId);
  BWAPI_FUNCTION void orderReturnCargo(int unitId);
  BWAPI_FUNCTION void orderMorph(int unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void orderBurrow(int unitId);
  BWAPI_FUNCTION void orderUnburrow(int unitId);
  BWAPI_FUNCTION void orderSiege(int unitId);
  BWAPI_FUNCTION void orderUnsiege(int unitId);
  BWAPI_FUNCTION void orderCloak(int unitId);
  BWAPI_FUNCTION void orderDecloak(int unitId);
  BWAPI_FUNCTION void orderLift(int unitId);
  BWAPI_FUNCTION void orderLand(int unitId);
  BWAPI_FUNCTION void orderLoad(int unitId, int targetUnitId);
  BWAPI_FUNCTION void orderUnload(int unitId, int targetUnitId);
  BWAPI_FUNCTION void orderUnloadAll(int unitId);
  BWAPI_FUNCTION void orderUnloadAllPosition(int unitId);
  BWAPI_FUNCTION void orderCancelConstruction(int unitId);
  BWAPI_FUNCTION void orderHaltConstruction(int unitId);
  BWAPI_FUNCTION void orderCancelMorph(int unitId);
  BWAPI_FUNCTION void orderCancelTrain(int unitId);
  BWAPI_FUNCTION void orderCancelTrainSlot(int unitId, int slotId);
  BWAPI_FUNCTION void orderCancelAddon(int unitId);
  BWAPI_FUNCTION void orderCancelResearch(int unitId);
  BWAPI_FUNCTION void orderCancelUpgrade(int unitId);
  BWAPI_FUNCTION void orderUseTech(int unitId);
  BWAPI_FUNCTION void orderUseTechPosition(int unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void orderUseTechUnit(int unitId, BWAPI::TechTypeId what, int targetUnitId);
}
