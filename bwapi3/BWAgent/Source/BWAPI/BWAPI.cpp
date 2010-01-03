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
  StaticGameData gameData;
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
    for each(Position pos in gameData.startLocations)
    {
      pos;
    }
  }
  //----------------------------------- GET UNIT --------------------------------------------------
  BWAPI_FUNCTION const BWAPI::UnitState* GetUnit(UnitId unitId)
  {
    return &gameData.units.at(unitId).state;
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
  //----------------------------------- ATTACK POSITION ORDER -------------------------------------
  BWAPI_FUNCTION void OrderAttackPosition(int unitId, int x, int y)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackPosition;
    order.unitIndex = unitId;
    order.x = x;
    order.y = y;
  }
  //----------------------------------- ATTACK UNIT ORDER -----------------------------------------
  BWAPI_FUNCTION void OrderAttackUnit(int unitId, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RIGHT CLICK POSITION ORDER --------------------------------
  BWAPI_FUNCTION void OrderRightClickPosition(int unitId, int x, int y)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickPosition;
    order.unitIndex = unitId;
    order.x = x;
    order.y = y;
  }
  //----------------------------------- RIGHT CLICK UNIT ORDER ------------------------------------
  BWAPI_FUNCTION void OrderRightClickUnit(int unitId, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- TRAIN ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderTrain(int unitId, BWAPI::UnitTypeId what)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Train;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BUILD ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderBuild(int unitId, BWAPI::UnitTypeId what)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Build;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BUILD ADDON ORDER -----------------------------------------
  BWAPI_FUNCTION void OrderBuildAddon(int unitId, BWAPI::UnitTypeId what)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::BuildAddon;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- RESEARCH ORDER --------------------------------------------
  BWAPI_FUNCTION void OrderResearch(int unitId, BWAPI::TechTypeId what)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Research;
    order.unitIndex = unitId;
    order.tech = what;
  }
  //----------------------------------- UPGRADE ORDER ---------------------------------------------
  BWAPI_FUNCTION void OrderUpgrade(int unitId, BWAPI::UpgradeTypeId what)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Upgrade;
    order.unitIndex = unitId;
    order.upgrade = what;
  }
  //----------------------------------- STOP ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderStop(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Stop;
    order.unitIndex = unitId;
  }
  //----------------------------------- HOLD POSITION ORDER ---------------------------------------
  BWAPI_FUNCTION void OrderHoldPosition(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HoldPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- PATROL ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderPatrol(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Patrol;
    order.unitIndex = unitId;
  }
  //----------------------------------- FOLLOW ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderFollow(int unitId, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Follow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SET RALLY POSITION ORDER ----------------------------------
  BWAPI_FUNCTION void OrderSetRallyPosition(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- SET RALLY UNIT ORDER --------------------------------------
  BWAPI_FUNCTION void OrderSetRallyUnit(int unitId, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyUnit;
    order.unitIndex = unitId;
  }
  //----------------------------------- REPAIR ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderRepair(int unitId, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Repair;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RETURN CARGO ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderReturnCargo(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::ReturnCargo;
    order.unitIndex = unitId;
  }
  //----------------------------------- MORPH ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderMorph(int unitId, BWAPI::UnitTypeId what)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Morph;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BURROW ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderBurrow(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Burrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNBURROW ORDER --------------------------------------------
  BWAPI_FUNCTION void OrderUnburrow(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unburrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SIEGE ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderSiege(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Siege;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNSIEGE ORDER ---------------------------------------------
  BWAPI_FUNCTION void OrderUnsiege(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unsiege;
    order.unitIndex = unitId;
  }
  //----------------------------------- CLOAK ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderCloak(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Cloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- DECLOAK ORDER ---------------------------------------------
  BWAPI_FUNCTION void OrderDecloak(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Decloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- LIFT ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderLift(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Lift;
    order.unitIndex = unitId;
  }
  //----------------------------------- LAND ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderLand(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Land;
    order.unitIndex = unitId;
  }
  //----------------------------------- LOAD ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderLoad(int unitId, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Load;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderUnload(int unitId, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unload;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ALL ORDER ------------------------------------------
  BWAPI_FUNCTION void OrderUnloadAll(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAll;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNLOAD ALL POSITION ORDER ---------------------------------
  BWAPI_FUNCTION void OrderUnloadAllPosition(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAllPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL CONSTRUCTION ORDER ---------------------------------
  BWAPI_FUNCTION void OrderCancelConstruction(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- HALT CONSTRUCTION ORDER -----------------------------------
  BWAPI_FUNCTION void OrderHaltConstruction(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HaltConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL MORPH ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderCancelMorph(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelMorph;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderCancelTrain(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrain;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN SLOT ORDER -----------------------------------
  BWAPI_FUNCTION void OrderCancelTrainSlot(int unitId, int slotId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrainSlot;
    order.unitIndex = unitId;
    order.extra = slotId;
  }
  //----------------------------------- CANCEL ADDON ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderCancelAddon(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelAddon;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL RESEARCH ORDER -------------------------------------
  BWAPI_FUNCTION void OrderCancelResearch(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelResearch;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL UPGRADE ORDER --------------------------------------
  BWAPI_FUNCTION void OrderCancelUpgrade(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelUpgrade;
    order.unitIndex = unitId;
  }
  //----------------------------------- USE TECH ORDER --------------------------------------------
  BWAPI_FUNCTION void OrderUseTech(int unitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTech;
    order.unitIndex = unitId;

  }
  //----------------------------------- USE TECH POSITION ORDER -----------------------------------
  BWAPI_FUNCTION void OrderUseTechPosition(int unitId, BWAPI::TechTypeId what)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- USE TECH UNIT ORDER ---------------------------------------
  BWAPI_FUNCTION void OrderUseTechUnit(int unitId, BWAPI::TechTypeId what, int targetUnitId)
  {
    BWAPI::UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- -----------------------------------------------------------
}
