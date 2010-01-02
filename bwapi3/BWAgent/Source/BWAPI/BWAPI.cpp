#include "BWAPI.h"
#include "BridgeClient.h"

#include <Util\Version.h>
#include <Util\Types.h>
#include <Util\HandleFactory.h>

#include <Bridge\SharedStuff.h>
#include <Bridge\EventEntry.h>
#include <Bridge\CommandEntry.h>

#include <BWAPIDatabase\UnitTypes.h>
#include <BWAPIDatabase\TechTypes.h>
#include <BWAPIDatabase\UpgradeTypes.h>
#include <BWAPIDatabase\WeaponTypes.h>
#include <BWAPIDatabase\DamageTypes.h>
#include <BWAPIDatabase\ExplosionTypes.h>
#include <BWAPIDatabase\Races.h>
#include <BWAPIDatabase\UnitSizeTypes.h>
#include <BWAPIDatabase\PlayerTypes.h>

namespace BWAPI
{
//private:
//public:
  //----------------------------------- STATIC DATA -----------------------------------------------
  BWAPI::StaticGameData gameData;
  //----------------------------------- GET VERSION -----------------------------------------------
  BWAPI_FUNCTION int GetVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  BWAPI_FUNCTION bool Connect() throw()
  {
    return BridgeClient::connect();
  }
  //----------------------------------- WAIT FOR EVENT --------------------------------------------
  BWAPI_FUNCTION CallTypeId WaitForEvent()
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

        }return CallTypeIds::OnMatchStart;
      case BridgeClient::OnFrame:
        {
        }return CallTypeIds::OnFrame;
      }
    }
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool PrintText(const char* text)
  {
    return BridgeClient::pushSendText(false, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool SendText(const char* text)
  {
    return BridgeClient::pushSendText(true, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION void DrawText(int x, int y, const char* text)
  {
    BridgeClient::pushDrawText(x, y, text);
  }
  //----------------------------------- DRAW RECTANGLE --------------------------------------------
  BWAPI_FUNCTION void DrawRectangle(int x, int y, int w, int h, int color, int solid)
  {
    BridgeClient::pushDrawRectangle(x, y, w, h, color, !!solid);
  }
  //----------------------------------- DRAW CIRCLE -----------------------------------------------
  BWAPI_FUNCTION void DrawCircle(int x, int y, int r, int color, int solid)
  {
    BridgeClient::pushDrawCircle(x, y, r, color, !!solid);
  }
  //----------------------------------- DRAW LINE -------------------------------------------------
  BWAPI_FUNCTION void DrawLine(int x, int y, int x2, int y2, int color)
  {
    BridgeClient::pushDrawLine(x, y, x2, y2, color);
  }
  //----------------------------------- DRAW DOT --------------------------------------------------
  BWAPI_FUNCTION void DrawDot(int x, int y, int color)
  {
    BridgeClient::pushDrawDot(x, y, color);
  }
  //----------------------------------- GET STATIC DATA -------------------------------------------
  BWAPI_FUNCTION const BWAPI::StaticGameData* GetStaticGameData()
  {
    BWAPI::StaticGameData *retval = BridgeClient::sharedStaticData;
    return retval;
  }
  //----------------------------------- POSITION MAP TO SCREEN ------------------------------------
  BWAPI_FUNCTION void PositionMapToScreen(BWAPI::Position* pos)
  {
    pos->x -= BridgeClient::sharedStaticData->screenX;
    pos->y -= BridgeClient::sharedStaticData->screenY;
  }
  //----------------------------------- GET UNIT --------------------------------------------------
  BWAPI_FUNCTION const BWAPI::UnitState* GetUnit(UnitId unitId)
  {
    return &BridgeClient::sharedStuff.knownUnits.getByLinear(unitId).state;
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION const BWAPI::Player* GetPlayer(PlayerId playerId)
  {
    return &gameData.players.at(playerId);
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION const BWAPI::UnitType* GetUnitType(BWAPI::UnitTypeId id)
  {
    return &BWAPI::UnitTypes::unitTypeData[id];
  }
  //----------------------------------- GET TECH TYPE ---------------------------------------------
  BWAPI_FUNCTION const BWAPI::TechType* GetTechType(BWAPI::TechTypeId id)
  {
    return &BWAPI::TechTypes::techTypeData[id];
  }
  //----------------------------------- GET UPGRADE TYPE ------------------------------------------
  BWAPI_FUNCTION const BWAPI::UpgradeType* GetUpgradeType(BWAPI::UpgradeTypeId id)
  {
    return &BWAPI::upgradeTypeData[id];
  }
  //----------------------------------- GET WEAPON TYPE -------------------------------------------
  BWAPI_FUNCTION const BWAPI::WeaponType* GetWeaponType(BWAPI::WeaponTypeId id)
  {
    return &BWAPI::WeaponTypes::weaponTypeData[id];
  }
  //----------------------------------- GET DAMAGE TYPE -------------------------------------------
  BWAPI_FUNCTION const BWAPI::DamageType* GetDamageType(BWAPI::DamageTypeId id)
  {
    return &BWAPI::DamageTypes::damageTypeData[id];
  }
  //----------------------------------- GET EXPLOSION TYPE ----------------------------------------
  BWAPI_FUNCTION const BWAPI::ExplosionType* GetExplosionType(BWAPI::ExplosionTypeId id)
  {
    return &BWAPI::ExplosionTypes::explosionTypeData[id];
  }
  //----------------------------------- GET RACE --------------------------------------------------
  BWAPI_FUNCTION const BWAPI::Race* GetRace(BWAPI::RaceId id)
  {
      return &BWAPI::Races::raceData[id];
  }
  //----------------------------------- GET UNIT SIZE TYPE ----------------------------------------
  BWAPI_FUNCTION const BWAPI::UnitSizeType* GetUnitSizeType(BWAPI::UnitSizeTypeId id)
  {
    return &BWAPI::UnitSizeTypes::unitSizeTypeData[id];
  }
  //----------------------------------- GET PLAYER TYPE -------------------------------------------
  BWAPI_FUNCTION const BWAPI::PlayerType* GetPlayerType(BWAPI::PlayerTypeId id)
  {
    return &BWAPI::PlayerTypes::playerTypeData[id];
  }
  //----------------------------------- -----------------------------------------------------------
  //----------------------------------- ALL UNITS ITERATION ---------------------------------------
  struct AllUnitsHandle
  {
    Bridge::SharedStuff::KnownUnitSet::Index index;
  };
  Util::HandleFactory<AllUnitsHandle> allUnitsHandleFactory;
  //----------------------------------- ALL UNITS BEGIN -------------------------------------------
  BWAPI_FUNCTION HANDLE AllUnitsBegin()
  {
    AllUnitsHandle *handle = allUnitsHandleFactory.create();
    handle->index = BridgeClient::sharedStuff.knownUnits.begin();
    return handle;
  }
  //----------------------------------- ALL UNITS NEXT --------------------------------------------
  BWAPI_FUNCTION int AllUnitsNext(HANDLE h)
  {
    AllUnitsHandle &handle = *(AllUnitsHandle*)h;
    if(!handle.index.isValid())
      return -1;
    int retval = BridgeClient::sharedStuff.knownUnits.getLinearByIndex(handle.index);
    handle.index = BridgeClient::sharedStuff.knownUnits.getNext(handle.index);
    return retval;
  }
  //----------------------------------- ALL UNITS CLOSE -------------------------------------------
  BWAPI_FUNCTION void AllUnitsClose(HANDLE h)
  {
    allUnitsHandleFactory.release((AllUnitsHandle*)h);
  }
  //----------------------------------- -----------------------------------------------------------
  BWAPI_FUNCTION const BWAPI::UnitAddEvent** GetUnitsAdded()
  {
    return &BridgeClient::knownUnitAddEvents[0];
  }
  BWAPI_FUNCTION const BWAPI::UnitRemoveEvent**  GetUnitsRemoved()
  {
    return &BridgeClient::knownUnitRemoveEvents[0];
  }
  //----------------------------------- INSERT ORDER ----------------------------------------------
  BWAPI::UnitCommand& insertOrder()
  {
    Bridge::CommandEntry::UnitOrder entry;
    Bridge::SharedStuff::CommandStack::Index i = BridgeClient::sharedStuff.commands.insert(Util::MemoryFrame::from(entry));

    static BWAPI::UnitCommand safeSpot;
    if(!i.isValid())
      return safeSpot;

    return BridgeClient::sharedStuff.commands.get(i).getAs<Bridge::CommandEntry::UnitOrder>().unitCommand;
  }
  //----------------------------------- STOP ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderStop(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Stop;
    order.unitIndex = unitId;
  }
  //----------------------------------- -----------------------------------------------------------
}
