#pragma once
#include "BWAPIInterface.h"

#include <BWAPITypes\UnitState.h>
#include <BWAPITypes\UnitEvent.h>
#include <BWAPITypes\StaticGameData.h>
#include <BWAPITypes\DynamicGameData.h>

#include <BWAPITypes\UnitType.h>
#include <BWAPITypes\UnitTypeId.h>
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
#include <BWAPITypes\CallTypeId.h>

#include <string>

#include <windows.h>
#undef DrawText

namespace BWAPI
{
  // return BWAgent.dll revision number
  BWAPI_FUNCTION int GetVersion();

  // returns zero instead throwing, if failed
  BWAPI_FUNCTION bool Connect() throw();

  // returns when a callback would have been called.
  // returns the callback type
  BWAPI_FUNCTION CallTypeId WaitForEvent();

  // text operations
  BWAPI_FUNCTION bool PrintText(const char* text);
  BWAPI_FUNCTION bool SendText(const char* text);

  // draw operations
  BWAPI_FUNCTION void DrawText(int x, int y, const char* text);
  BWAPI_FUNCTION void DrawRectangle(int x, int y, int w, int h, int color, int solid);
  BWAPI_FUNCTION void DrawCircle(int x, int y, int r, int color, int solid);
  BWAPI_FUNCTION void DrawLine(int x, int y, int x2, int y2, int color);
  BWAPI_FUNCTION void DrawDot(int x, int y, int color);

  // information queries
  BWAPI_FUNCTION const StaticGameData*  GetStaticGameData();
  BWAPI_FUNCTION const DynamicGameData* GetDynamicGameData();

  // access directly through static data. Use this if error checking is needed
  BWAPI_FUNCTION const UnitState*       GetUnit(UnitId unitId);
  BWAPI_FUNCTION const Player*          GetPlayer(PlayerId playerId);

  // database related
  BWAPI_FUNCTION const UnitType*        GetUnitType(UnitTypeId id);
  BWAPI_FUNCTION const TechType*        GetTechType(TechTypeId id);
  BWAPI_FUNCTION const UpgradeType*     GetUpgradeType(UpgradeTypeId id);
  BWAPI_FUNCTION const WeaponType*      GetWeaponType(WeaponTypeId id);
  BWAPI_FUNCTION const DamageType*      GetDamageType(DamageTypeId id);
  BWAPI_FUNCTION const ExplosionType*   GetExplosionType(ExplosionTypeId id);
  BWAPI_FUNCTION const Race*            GetRace(RaceId id);
  BWAPI_FUNCTION const UnitSizeType*    GetUnitSizeType(UnitSizeTypeId id);
  BWAPI_FUNCTION const PlayerType*      GetPlayerType(PlayerTypeId id);

  // unit commands
  BWAPI_FUNCTION void OrderAttackPosition(int unitId, int x, int y);
  BWAPI_FUNCTION void OrderAttackUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderRightClickPosition(int unitId, int x, int y);
  BWAPI_FUNCTION void OrderRightClickUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderTrain(int unitId, UnitTypeId what);
  BWAPI_FUNCTION void OrderBuild(int unitId, UnitTypeId what);
  BWAPI_FUNCTION void OrderBuildAddon(int unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void OrderResearch(int unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void OrderUpgrade(int unitId, BWAPI::UpgradeTypeId what);
  BWAPI_FUNCTION void OrderStop(int unitId);
  BWAPI_FUNCTION void OrderHoldPosition(int unitId);
  BWAPI_FUNCTION void OrderPatrol(int unitId);
  BWAPI_FUNCTION void OrderFollow(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderSetRallyPosition(int unitId);
  BWAPI_FUNCTION void OrderSetRallyUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderRepair(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderReturnCargo(int unitId);
  BWAPI_FUNCTION void OrderMorph(int unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void OrderBurrow(int unitId);
  BWAPI_FUNCTION void OrderUnburrow(int unitId);
  BWAPI_FUNCTION void OrderSiege(int unitId);
  BWAPI_FUNCTION void OrderUnsiege(int unitId);
  BWAPI_FUNCTION void OrderCloak(int unitId);
  BWAPI_FUNCTION void OrderDecloak(int unitId);
  BWAPI_FUNCTION void OrderLift(int unitId);
  BWAPI_FUNCTION void OrderLand(int unitId);
  BWAPI_FUNCTION void OrderLoad(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderUnload(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderUnloadAll(int unitId);
  BWAPI_FUNCTION void OrderUnloadAllPosition(int unitId);
  BWAPI_FUNCTION void OrderCancelConstruction(int unitId);
  BWAPI_FUNCTION void OrderHaltConstruction(int unitId);
  BWAPI_FUNCTION void OrderCancelMorph(int unitId);
  BWAPI_FUNCTION void OrderCancelTrain(int unitId);
  BWAPI_FUNCTION void OrderCancelTrainSlot(int unitId, int slotId);
  BWAPI_FUNCTION void OrderCancelAddon(int unitId);
  BWAPI_FUNCTION void OrderCancelResearch(int unitId);
  BWAPI_FUNCTION void OrderCancelUpgrade(int unitId);
  BWAPI_FUNCTION void OrderUseTech(int unitId);
  BWAPI_FUNCTION void OrderUseTechPosition(int unitId, BWAPI::TechTypeId what);
  BWAPI_FUNCTION void OrderUseTechUnit(int unitId, BWAPI::TechTypeId what, int targetUnitId);
}