#include "BWAPI.h"
#include "BridgeClient.h"

#include <Util\Version.h>
#include <Util\Types.h>
#include <Util\HandleFactory.h>

#include <Bridge\SharedStuff.h>
#include <Bridge\KnownUnitEventEntry.h>

#include <BWAPIDatabase\UnitTypes.h>
#include <BWAPIDatabase\TechTypes.h>
#include <BWAPIDatabase\UpgradeTypes.h>
#include <BWAPIDatabase\WeaponTypes.h>
#include <BWAPIDatabase\DamageTypes.h>
#include <BWAPIDatabase\ExplosionTypes.h>
#include <BWAPIDatabase\Races.h>
#include <BWAPIDatabase\UnitSizeTypes.h>

// singleton base class
namespace BWAPI
{
//private:

  std::string lastError;
  void resetError()
  {
    lastError = "no error";
  }

//public:
  //----------------------------------- GET VERSION -----------------------------------------------
  BWAPI_FUNCTION int BWAPI_CALL BWGetVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  BWAPI_FUNCTION bool BWAPI_CALL BWConnect()
  {
    resetError();
    if(!BridgeClient::connect())
    {
      lastError = "could not connect: " + BridgeClient::getLastError();
      return 0;
    }
    return 1;
  }
  //----------------------------------- TAKE OVER -------------------------------------------------
  BWAPI_FUNCTION bool BWAPI_CALL BWTakeover(BWMatchFrameCallback onMatchFrame,
    BWMatchStartCallback onMatchStart,
    BWUnitAddCallback onUnitAdd,
    BWUnitRemoveCallback onUnitRemove,
    BWSendTextCallback onSendText)
  {
    resetError();
    try
    {
      while(true)
      {
        if(!BridgeClient::waitForEvent())
          throw GeneralException(__FUNCTION__ ": " + BridgeClient::getLastError());

        // react upon bridge state
        BridgeClient::RpcState rpcState = BridgeClient::getCurrentRpc();
        switch(rpcState)
        {
        case BridgeClient::OnInitMatch:
          {
            // init database
            UnitTypes::init();
            TechTypes::init();
            UpgradeTypes::init();
            WeaponTypes::init();
            DamageTypes::init();
            ExplosionTypes::init();
            Races::init();
            UnitSizeTypes::init();

            if(onMatchStart)onMatchStart(BridgeClient::isMatchStartFromBeginning);
          }break;
        case BridgeClient::OnFrame:
          {
            for each(const std::string &input in BridgeClient::getUserInputStrings())
            {
              if(onSendText)onSendText(input.c_str());
            }
            onMatchFrame();
          }break;
        }
      }
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return false;
    }
    return true;
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool BWAPI_CALL BWPrintText(const char* text)
  {
    return BridgeClient::pushSendText(false, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool BWAPI_CALL BWSendText(const char* text)
  {
    return BridgeClient::pushSendText(true, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWDrawText(int x, int y, const char* text)
  {
    BridgeClient::pushDrawText(x, y, text);
  }
  //----------------------------------- DRAW RECTANGLE --------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWDrawRectangle(int x, int y, int w, int h, int color, int solid)
  {
    BridgeClient::pushDrawRectangle(x, y, w, h, color, !!solid);
  }
  //----------------------------------- DRAW CIRCLE -----------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWDrawCircle(int x, int y, int r, int color, int solid)
  {
    BridgeClient::pushDrawCircle(x, y, r, color, !!solid);
  }
  //----------------------------------- DRAW LINE -------------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWDrawLine(int x, int y, int x2, int y2, int color)
  {
    BridgeClient::pushDrawLine(x, y, x2, y2, color);
  }
  //----------------------------------- DRAW DOT --------------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWDrawDot(int x, int y, int color)
  {
    BridgeClient::pushDrawDot(x, y, color);
  }
  //----------------------------------- GET STATIC DATA -------------------------------------------
  BWAPI_FUNCTION BWAPI::StaticGameData* BWAPI_CALL BWGetStaticGameData()
  {
    BWAPI::StaticGameData *retval = BridgeClient::sharedStaticData;
    return retval;
  }
  //----------------------------------- POSITION MAP TO SCREEN ------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWPositionMapToScreen(BWAPI::Position* pos)
  {
    pos->x -= BridgeClient::sharedStaticData->screenX;
    pos->y -= BridgeClient::sharedStaticData->screenY;
  }
  //----------------------------------- GET UNIT --------------------------------------------------
  BWAPI_FUNCTION BWAPI::UnitState* BWAPI_CALL BWGetUnit(int unitId)
  {
    resetError();
    try
    {
      return &BridgeClient::sharedStuff.knownUnits.getByLinear(unitId);
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION BWAPI::UnitType* BWAPI_CALL BWGetUnitType(BWAPI::UnitTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::UnitTypes::unitTypeData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET TECH TYPE ---------------------------------------------
  BWAPI_FUNCTION BWAPI::TechType* BWAPI_CALL BWGetTechType(BWAPI::TechTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::TechTypes::techTypeData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET UPGRADE TYPE ------------------------------------------
  BWAPI_FUNCTION BWAPI::UpgradeType* BWAPI_CALL BWGetUpgradeType(BWAPI::UpgradeTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::upgradeTypeData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET WEAPON TYPE -------------------------------------------
  BWAPI_FUNCTION BWAPI::WeaponType* BWAPI_CALL BWGetWeaponType(BWAPI::WeaponTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::WeaponTypes::weaponTypeData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET DAMAGE TYPE -------------------------------------------
  BWAPI_FUNCTION BWAPI::DamageType* BWAPI_CALL BWGetDamageType(BWAPI::DamageTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::DamageTypes::damageTypeData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET EXPLOSION TYPE ----------------------------------------
  BWAPI_FUNCTION BWAPI::ExplosionType* BWAPI_CALL BWGetExplosionType(BWAPI::ExplosionTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::ExplosionTypes::explosionTypeData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET RACE --------------------------------------------------
  BWAPI_FUNCTION BWAPI::Race* BWAPI_CALL BWGetRace(BWAPI::RaceId id)
  {
    resetError();
    try
    {
      return &BWAPI::Races::raceData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET UNIT SIZE TYPE ----------------------------------------
  BWAPI_FUNCTION BWAPI::UnitSizeType* BWAPI_CALL BWGetUnitSizeType(BWAPI::UnitSizeTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::UnitSizeTypes::unitSizeTypeData[id];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  //----------------------------------- GET PLAYER TYPE -------------------------------------------
  /*
  BWAPI_FUNCTION BWAPI::PlayerType* BWAPI_CALL BWGetPlayerType(BWAPI::UnitPlayerTypeId id)
  {
    resetError();
    try
    {
      return &BWAPI::playerTypeData[unitId];
    }
    catch(GeneralException &exception)
    {
      lastError = exception.getMessage();
      return NULL;
    }
  }
  */
  //----------------------------------- -----------------------------------------------------------
  //----------------------------------- ALL UNITS ITERATION ---------------------------------------
  struct AllUnitsHandle
  {
    Bridge::SharedStuff::KnownUnitSet::Index index;
  };
  Util::HandleFactory<AllUnitsHandle> allUnitsHandleFactory;
  //----------------------------------- ALL UNITS BEGIN -------------------------------------------
  BWAPI_FUNCTION HANDLE BWAPI_CALL BWAllUnitsBegin()
  {
    AllUnitsHandle *handle = allUnitsHandleFactory.create();
    handle->index = BridgeClient::sharedStuff.knownUnits.begin();
    return handle;
  }
  //----------------------------------- ALL UNITS NEXT --------------------------------------------
  BWAPI_FUNCTION int BWAPI_CALL BWAllUnitsNext(HANDLE h)
  {
    AllUnitsHandle &handle = *(AllUnitsHandle*)h;
    if(!handle.index.isValid())
      return -1;
    int retval = BridgeClient::sharedStuff.knownUnits.getLinearByIndex(handle.index);
    handle.index = BridgeClient::sharedStuff.knownUnits.getNext(handle.index);
    return retval;
  }
  //----------------------------------- ALL UNITS CLOSE -------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWAllUnitsClose(HANDLE h)
  {
    allUnitsHandleFactory.release((AllUnitsHandle*)h);
  }
  //----------------------------------- -----------------------------------------------------------
  //----------------------------------- UNIT EVENTS ITERATION -------------------------------------
  struct UnitAddEventsHandle
  {
    Bridge::SharedStuff::KnownUnitAddEventStack::Index index;
  };
  Util::HandleFactory<UnitAddEventsHandle> unitAddEventsHandleFactory;
  //----------------------------------- UNIT EVENTS BEGIN -----------------------------------------
  BWAPI_FUNCTION HANDLE BWAPI_CALL BWUnitAddEventsBegin()
  {
    UnitAddEventsHandle *handle = unitAddEventsHandleFactory.create();
    handle->index = BridgeClient::sharedStuff.knownUnitAddEvents.begin();
    return handle;
  }
  //----------------------------------- UNIT EVENTS NEXT ------------------------------------------
  BWAPI_FUNCTION BWAPI::UnitAddEvent* BWAPI_CALL BWUnitAddEventsNext(HANDLE h)
  {
    UnitAddEventsHandle &handle = *(UnitAddEventsHandle*)h;
    if(!handle.index.isValid())
      return NULL;

    // get stack entry
    Util::MemoryFrame mem = BridgeClient::sharedStuff.knownUnitAddEvents.get(handle.index);

    // next index, save into handle
    handle.index = BridgeClient::sharedStuff.knownUnitAddEvents.getNext(handle.index);

    // retrieve the added unit slot reference
    BWAPI::UnitAddEvent &retval = mem.getAs<Bridge::KnownUnitAddEventEntry>().data;
    return &retval;
  }
  //----------------------------------- UNIT EVENTS CLOSE -----------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWUnitAddEventsClose(HANDLE h)
  {
    unitAddEventsHandleFactory.release((UnitAddEventsHandle*)h);
  }
  //----------------------------------- -----------------------------------------------------------
  //----------------------------------- UNIT EVENTS ITERATION -------------------------------------
  struct UnitRemoveEventsHandle
  {
    Bridge::SharedStuff::KnownUnitRemoveEventStack::Index index;
  };
  Util::HandleFactory<UnitRemoveEventsHandle> unitRemoveEventsHandleFactory;
  //----------------------------------- UNIT EVENTS BEGIN -----------------------------------------
  BWAPI_FUNCTION HANDLE BWAPI_CALL BWUnitRemoveEventsBegin()
  {
    UnitRemoveEventsHandle *handle = unitRemoveEventsHandleFactory.create();
    handle->index = BridgeClient::sharedStuff.knownUnitRemoveEvents.begin();
    return handle;
  }
  //----------------------------------- UNIT EVENTS NEXT ------------------------------------------
  BWAPI_FUNCTION BWAPI::UnitRemoveEvent* BWAPI_CALL BWUnitRemoveEventsNext(HANDLE h)
  {
    UnitRemoveEventsHandle &handle = *(UnitRemoveEventsHandle*)h;
    if(!handle.index.isValid())
      return NULL;

    // get stack entry
    Util::MemoryFrame mem = BridgeClient::sharedStuff.knownUnitRemoveEvents.get(handle.index);

    // next index, save into handle
    handle.index = BridgeClient::sharedStuff.knownUnitRemoveEvents.getNext(handle.index);

    // retrieve the removed unit slot reference
    BWAPI::UnitRemoveEvent &retval = mem.getAs<Bridge::KnownUnitRemoveEventEntry>().data;
    return &retval;
  }
  //----------------------------------- UNIT EVENTS CLOSE -----------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWUnitRemoveEventsClose(HANDLE h)
  {
    unitRemoveEventsHandleFactory.release((UnitRemoveEventsHandle*)h);
  }
  //----------------------------------- GET LAST ERROR --------------------------------------------
  BWAPI_FUNCTION const char* BWAPI_CALL BWGetLastError()
  {
    return lastError.c_str();
  }
  //----------------------------------- -----------------------------------------------------------
}
