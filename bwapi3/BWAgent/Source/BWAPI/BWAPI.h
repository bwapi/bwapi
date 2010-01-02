#pragma once
#include "BWAPIInterface.h"

#include <BWAPITypes\UnitState.h>
#include <BWAPITypes\UnitEvent.h>
#include <BWAPITypes\StaticGameData.h>

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

#include <windows.h>
#include <string>

namespace BWAPI
{
  // return BWAgent.dll revision number
  BWAPI_FUNCTION int GetVersion();

  // returns zero if failed
  BWAPI_FUNCTION bool Connect() throw();

  // returns when usually a callback is called

  BWAPI_FUNCTION CallTypeId WaitForEvent();


  // static data
  BWAPI_FUNCTION BWAPI::StaticGameData* GetStaticGameData();
  BWAPI_FUNCTION void                   PositionMapToScreen(BWAPI::Position* pos);

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
  BWAPI_FUNCTION BWAPI::UnitState*      GetUnit(int unitId);
  BWAPI_FUNCTION BWAPI::UnitType*       GetUnitType(BWAPI::UnitTypeId id);
  BWAPI_FUNCTION BWAPI::TechType*       GetTechType(BWAPI::TechTypeId id);
  BWAPI_FUNCTION BWAPI::UpgradeType*    GetUpgradeType(BWAPI::UpgradeTypeId id);
  BWAPI_FUNCTION BWAPI::WeaponType*     GetWeaponType(BWAPI::WeaponTypeId id);
  BWAPI_FUNCTION BWAPI::DamageType*     GetDamageType(BWAPI::DamageTypeId id);
  BWAPI_FUNCTION BWAPI::ExplosionType*  GetExplosionType(BWAPI::ExplosionTypeId id);
  BWAPI_FUNCTION BWAPI::Race*           GetRace(BWAPI::RaceId id);
  BWAPI_FUNCTION BWAPI::UnitSizeType*   GetUnitSizeType(BWAPI::UnitSizeTypeId id);
  BWAPI_FUNCTION BWAPI::PlayerType*     GetPlayerType(BWAPI::PlayerTypeId id);

  // unit array enumeration. returns -1 when ended
  BWAPI_FUNCTION HANDLE AllUnitsBegin();
  BWAPI_FUNCTION int    AllUnitsNext(HANDLE h);
  BWAPI_FUNCTION void   AllUnitsClose(HANDLE h);

  // events
  BWAPI_FUNCTION BWAPI::UnitAddEvent**    GetUnitsAdded();
  BWAPI_FUNCTION BWAPI::UnitRemoveEvent** GetUnitsRemoved();

  // unit commands
  BWAPI_FUNCTION void OrderAttackPosition(int unitId, int x, int y);
  BWAPI_FUNCTION void OrderAttackUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderRightClickPosition(int unitId, int x, int y);
  BWAPI_FUNCTION void OrderRightClickUnit(int unitId, int targetUnitId);
  BWAPI_FUNCTION void OrderTrain(int unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void OrderBuild(int unitId, BWAPI::UnitTypeId what);
  BWAPI_FUNCTION void OrderBuildAddon(int unitId);
  BWAPI_FUNCTION void OrderResearch(int unitId);
  BWAPI_FUNCTION void OrderUpgrade(int unitId);
  BWAPI_FUNCTION void OrderStop(int unitId);
  BWAPI_FUNCTION void OrderHoldPosition(int unitId);
  BWAPI_FUNCTION void OrderPatrol(int unitId);
  BWAPI_FUNCTION void OrderFollow(int unitId);
  BWAPI_FUNCTION void OrderSetRallyPosition(int unitId);
  BWAPI_FUNCTION void OrderSetRallyUnit(int unitId);
  BWAPI_FUNCTION void OrderRepair(int unitId);
  BWAPI_FUNCTION void OrderMorph(int unitId);
  BWAPI_FUNCTION void OrderBurrow(int unitId);
  BWAPI_FUNCTION void OrderUnburrow(int unitId);
  BWAPI_FUNCTION void OrderSiege(int unitId);
  BWAPI_FUNCTION void OrderUnsiege(int unitId);
  BWAPI_FUNCTION void OrderCloak(int unitId);
  BWAPI_FUNCTION void OrderDecloak(int unitId);
  BWAPI_FUNCTION void OrderLift(int unitId);
  BWAPI_FUNCTION void OrderLand(int unitId);
  BWAPI_FUNCTION void OrderLoad(int unitId);
  BWAPI_FUNCTION void OrderUnload(int unitId);
  BWAPI_FUNCTION void OrderUnloadAll(int unitId);
  BWAPI_FUNCTION void OrderUnloadAllPosition(int unitId);
  BWAPI_FUNCTION void OrderCancelConstruction(int unitId);
  BWAPI_FUNCTION void OrderHaltConstruction(int unitId);
  BWAPI_FUNCTION void OrderCancelMorph(int unitId);
  BWAPI_FUNCTION void OrderCancelTrain(int unitId);
  BWAPI_FUNCTION void OrderCancelTrainSlot(int unitId);
  BWAPI_FUNCTION void OrderCancelAddon(int unitId);
  BWAPI_FUNCTION void OrderCancelResearch(int unitId);
  BWAPI_FUNCTION void OrderCancelUpgrade(int unitId);
  BWAPI_FUNCTION void OrderUseTech(int unitId);
  BWAPI_FUNCTION void OrderUseTechPosition(int unitId);
  BWAPI_FUNCTION void OrderUseTechUnit(int unitId);
}