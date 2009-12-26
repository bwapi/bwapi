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

#include <windows.h>
#include <string>

// return BWAgent.dll revision number
BWAPI_FUNCTION int BWAPI_CALL BWGetVersion();

// returns zero if failed
BWAPI_FUNCTION bool BWAPI_CALL BWConnect();

// TODO: remove BWTakeover. Instead provide BWWaitForRpc()
// blocks. returns only if Disconnect() called from a callback
// returns false immediately or during run if error occurred
typedef void (BWAPI_CALL *BWMatchFrameCallback)();
typedef void (BWAPI_CALL *BWMatchStartCallback)(bool fromBeginning);
typedef void (BWAPI_CALL *BWUnitAddCallback)(BWAPI::UnitState*);
typedef void (BWAPI_CALL *BWUnitRemoveCallback)(BWAPI::UnitState*);
typedef void (BWAPI_CALL *BWSendTextCallback)(const char*);
BWAPI_FUNCTION bool BWAPI_CALL BWTakeover(
  BWMatchFrameCallback,
  BWMatchStartCallback,
  BWUnitAddCallback,
  BWUnitRemoveCallback,
  BWSendTextCallback);

// static data
BWAPI_FUNCTION BWAPI::StaticGameData* BWAPI_CALL BWGetStaticGameData();
BWAPI_FUNCTION void                   BWAPI_CALL BWPositionMapToScreen(BWAPI::Position* pos);

// text operations
BWAPI_FUNCTION bool BWAPI_CALL BWPrintText(const char* text);
BWAPI_FUNCTION bool BWAPI_CALL BWSendText(const char* text);

// draw operations
BWAPI_FUNCTION void BWAPI_CALL BWDrawText(int x, int y, const char* text);
BWAPI_FUNCTION void BWAPI_CALL BWDrawRectangle(int x, int y, int w, int h, int color, int solid);
BWAPI_FUNCTION void BWAPI_CALL BWDrawCircle(int x, int y, int r, int color, int solid);
BWAPI_FUNCTION void BWAPI_CALL BWDrawLine(int x, int y, int x2, int y2, int color);
BWAPI_FUNCTION void BWAPI_CALL BWDrawDot(int x, int y, int color);

// information queries
BWAPI_FUNCTION BWAPI::UnitState*      BWAPI_CALL BWGetUnit(int unitId);
BWAPI_FUNCTION BWAPI::UnitType*       BWAPI_CALL BWGetUnitType(BWAPI::UnitTypeId type);
BWAPI_FUNCTION BWAPI::TechType*       BWAPI_CALL BWGetTechType(BWAPI::TechTypeId type);
BWAPI_FUNCTION BWAPI::UpgradeType*    BWAPI_CALL BWGetUpgradeType(BWAPI::UpgradeTypeId type);
BWAPI_FUNCTION BWAPI::WeaponType*     BWAPI_CALL BWGetWeaponType(BWAPI::WeaponTypeId type);
BWAPI_FUNCTION BWAPI::DamageType*     BWAPI_CALL BWGetDamageType(BWAPI::DamageTypeId type);
BWAPI_FUNCTION BWAPI::ExplosionType*  BWAPI_CALL BWGetExplosionType(BWAPI::ExplosionTypeId type);
BWAPI_FUNCTION BWAPI::Race*           BWAPI_CALL BWGetRace(BWAPI::RaceId type);
BWAPI_FUNCTION BWAPI::UnitSizeType*   BWAPI_CALL BWGetUnitSizeType(BWAPI::UnitSizeTypeId type);

// unit array enumeration. returns -1 when ended
BWAPI_FUNCTION HANDLE BWAPI_CALL BWAllUnitsBegin();
BWAPI_FUNCTION int    BWAPI_CALL BWAllUnitsNext(HANDLE h);
BWAPI_FUNCTION void   BWAPI_CALL BWAllUnitsClose(HANDLE h);

// unit add event enumeration
BWAPI_FUNCTION HANDLE               BWAPI_CALL BWUnitAddEventsBegin();
BWAPI_FUNCTION BWAPI::UnitAddEvent* BWAPI_CALL BWUnitAddEventsNext(HANDLE h);
BWAPI_FUNCTION void                 BWAPI_CALL BWUnitAddEventsClose(HANDLE h);

// unit remove event enumeration
BWAPI_FUNCTION HANDLE                   BWAPI_CALL BWUnitRemoveEventsBegin();
BWAPI_FUNCTION BWAPI::UnitRemoveEvent*  BWAPI_CALL BWUnitRemoveEventsNext(HANDLE h);
BWAPI_FUNCTION void                     BWAPI_CALL BWUnitRemoveEventsClose(HANDLE h);

// returns last operation's error string.
BWAPI_FUNCTION const char* BWAPI_CALL BWGetLastError();
