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
      BridgeClient::waitForEvent();

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
  BWAPI_FUNCTION void DrawText(Position pos, const char* text)
  {
    BridgeClient::pushDrawText(pos - BridgeClient::gameData->screenPosition, text);
  }
  BWAPI_FUNCTION void DrawTextScreen(Position pos, const char* text)
  {
    BridgeClient::pushDrawText(pos, text);
  }
  //----------------------------------- DRAW RECTANGLE --------------------------------------------
  BWAPI_FUNCTION void DrawRectangle(Position pos, Position size, int color, int solid)
  {
    BridgeClient::pushDrawRectangle(pos - BridgeClient::gameData->screenPosition, size - BridgeClient::gameData->screenPosition, color, !!solid);
  }
  BWAPI_FUNCTION void DrawRectangleScreen(Position pos, Position size, int color, int solid)
  {
    BridgeClient::pushDrawRectangle(pos, size, color, !!solid);
  }
  //----------------------------------- DRAW CIRCLE -----------------------------------------------
  BWAPI_FUNCTION void DrawCircle(Position pos, int r, int color, bool solid)
  {
    BridgeClient::pushDrawCircle(pos - BridgeClient::gameData->screenPosition, r, color, !!solid);
  }
  BWAPI_FUNCTION void DrawCircleScreen(Position pos, int r, int color, bool solid)
  {
    BridgeClient::pushDrawCircle(pos, r, color, !!solid);
  }
  //----------------------------------- DRAW LINE -------------------------------------------------
  BWAPI_FUNCTION void DrawLine(Position pos, Position pos2, int color)
  {
    BridgeClient::pushDrawLine(pos - BridgeClient::gameData->screenPosition, pos2 - BridgeClient::gameData->screenPosition, color);
  }
  BWAPI_FUNCTION void DrawLineScreen(Position pos, Position pos2, int color)
  {
    BridgeClient::pushDrawLine(pos, pos2, color);
  }
  //----------------------------------- DRAW DOT --------------------------------------------------
  BWAPI_FUNCTION void DrawDot(Position pos, int color)
  {
    BridgeClient::pushDrawDot(pos - BridgeClient::gameData->screenPosition, color);
  }
  BWAPI_FUNCTION void DrawDotScreen(Position pos, int color)
  {
    BridgeClient::pushDrawDot(pos, color);
  }
  //----------------------------------- GET STATIC DATA -------------------------------------------
  BWAPI_FUNCTION const StaticGameData* GetStaticGameData()
  {
    if(!BridgeClient::isConnected())
      throw GeneralException("GetStaticGameData requires connection");
    StaticGameData *retval = BridgeClient::gameData;
    return retval;
  }
  //----------------------------------- GET DYNAMIC EVENT CONTAINERS ------------------------------
  BWAPI_FUNCTION const DynamicGameData* GetDynamicGameData()
  {
    if(!BridgeClient::isConnected())
      throw GeneralException("GetDynamicGameData requires connection");
    DynamicGameData *retval = &BridgeClient::dynamicData;
    return retval;
  }
  //----------------------------------- GET UNIT --------------------------------------------------
  BWAPI_FUNCTION const UnitState* GetUnit(UnitId unitId)
  {
    return &BridgeClient::gameData->units.at(unitId);
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION const Player* GetPlayer(PlayerId playerId)
  {
    return &BridgeClient::gameData->players.at(playerId);
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION const UnitType* GetUnitType(UnitTypeId id)
  {
    return &UnitTypes::unitTypeData[id];
  }
  //----------------------------------- GET TECH TYPE ---------------------------------------------
  BWAPI_FUNCTION const TechType* GetTechType(TechTypeId id)
  {
    return &TechTypes::techTypeData[id];
  }
  //----------------------------------- GET UPGRADE TYPE ------------------------------------------
  BWAPI_FUNCTION const UpgradeType* GetUpgradeType(UpgradeTypeId id)
  {
    return &upgradeTypeData[id];
  }
  //----------------------------------- GET WEAPON TYPE -------------------------------------------
  BWAPI_FUNCTION const WeaponType* GetWeaponType(WeaponTypeId id)
  {
    return &WeaponTypes::weaponTypeData[id];
  }
  //----------------------------------- GET DAMAGE TYPE -------------------------------------------
  BWAPI_FUNCTION const DamageType* GetDamageType(DamageTypeId id)
  {
    return &DamageTypes::damageTypeData[id];
  }
  //----------------------------------- GET EXPLOSION TYPE ----------------------------------------
  BWAPI_FUNCTION const ExplosionType* GetExplosionType(ExplosionTypeId id)
  {
    return &ExplosionTypes::explosionTypeData[id];
  }
  //----------------------------------- GET RACE --------------------------------------------------
  BWAPI_FUNCTION const Race* GetRace(RaceId id)
  {
      return &Races::raceData[id];
  }
  //----------------------------------- GET UNIT SIZE TYPE ----------------------------------------
  BWAPI_FUNCTION const UnitSizeType* GetUnitSizeType(UnitSizeTypeId id)
  {
    return &UnitSizeTypes::unitSizeTypeData[id];
  }
  //----------------------------------- GET PLAYER TYPE -------------------------------------------
  BWAPI_FUNCTION const PlayerType* GetPlayerType(PlayerTypeId id)
  {
    return &PlayerTypes::playerTypeData[id];
  }
  //----------------------------------- -----------------------------------------------------------
  //----------------------------------- INSERT ORDER ----------------------------------------------
  UnitCommand& insertOrder()
  {
    Bridge::CommandEntry::UnitOrder entry;
    Bridge::SharedStuff::CommandStack::Index i = BridgeClient::sharedStuff.commands.insert(Util::MemoryFrame::from(entry));

    static UnitCommand safeSpot;
    if(!i.isValid())
      return safeSpot;

    return BridgeClient::sharedStuff.commands.get(i).getAs<Bridge::CommandEntry::UnitOrder>().unitCommand;
  }
  //----------------------------------- ATTACK POSITION ORDER -------------------------------------
  BWAPI_FUNCTION void OrderAttackPosition(int unitId, int x, int y)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackPosition;
    order.unitIndex = unitId;
    order.x = x;
    order.y = y;
  }
  //----------------------------------- ATTACK UNIT ORDER -----------------------------------------
  BWAPI_FUNCTION void OrderAttackUnit(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RIGHT CLICK POSITION ORDER --------------------------------
  BWAPI_FUNCTION void OrderRightClickPosition(int unitId, int x, int y)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickPosition;
    order.unitIndex = unitId;
    order.x = x;
    order.y = y;
  }
  //----------------------------------- RIGHT CLICK UNIT ORDER ------------------------------------
  BWAPI_FUNCTION void OrderRightClickUnit(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- TRAIN ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderTrain(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Train;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BUILD ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderBuild(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Build;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BUILD ADDON ORDER -----------------------------------------
  BWAPI_FUNCTION void OrderBuildAddon(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::BuildAddon;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- RESEARCH ORDER --------------------------------------------
  BWAPI_FUNCTION void OrderResearch(int unitId, TechTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Research;
    order.unitIndex = unitId;
    order.tech = what;
  }
  //----------------------------------- UPGRADE ORDER ---------------------------------------------
  BWAPI_FUNCTION void OrderUpgrade(int unitId, UpgradeTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Upgrade;
    order.unitIndex = unitId;
    order.upgrade = what;
  }
  //----------------------------------- STOP ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderStop(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Stop;
    order.unitIndex = unitId;
  }
  //----------------------------------- HOLD POSITION ORDER ---------------------------------------
  BWAPI_FUNCTION void OrderHoldPosition(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HoldPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- PATROL ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderPatrol(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Patrol;
    order.unitIndex = unitId;
  }
  //----------------------------------- FOLLOW ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderFollow(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Follow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SET RALLY POSITION ORDER ----------------------------------
  BWAPI_FUNCTION void OrderSetRallyPosition(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- SET RALLY UNIT ORDER --------------------------------------
  BWAPI_FUNCTION void OrderSetRallyUnit(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyUnit;
    order.unitIndex = unitId;
  }
  //----------------------------------- REPAIR ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderRepair(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Repair;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RETURN CARGO ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderReturnCargo(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::ReturnCargo;
    order.unitIndex = unitId;
  }
  //----------------------------------- MORPH ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderMorph(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Morph;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BURROW ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderBurrow(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Burrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNBURROW ORDER --------------------------------------------
  BWAPI_FUNCTION void OrderUnburrow(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unburrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SIEGE ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderSiege(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Siege;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNSIEGE ORDER ---------------------------------------------
  BWAPI_FUNCTION void OrderUnsiege(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unsiege;
    order.unitIndex = unitId;
  }
  //----------------------------------- CLOAK ORDER -----------------------------------------------
  BWAPI_FUNCTION void OrderCloak(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Cloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- DECLOAK ORDER ---------------------------------------------
  BWAPI_FUNCTION void OrderDecloak(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Decloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- LIFT ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderLift(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Lift;
    order.unitIndex = unitId;
  }
  //----------------------------------- LAND ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderLand(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Land;
    order.unitIndex = unitId;
  }
  //----------------------------------- LOAD ORDER ------------------------------------------------
  BWAPI_FUNCTION void OrderLoad(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Load;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ORDER ----------------------------------------------
  BWAPI_FUNCTION void OrderUnload(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unload;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ALL ORDER ------------------------------------------
  BWAPI_FUNCTION void OrderUnloadAll(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAll;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNLOAD ALL POSITION ORDER ---------------------------------
  BWAPI_FUNCTION void OrderUnloadAllPosition(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAllPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL CONSTRUCTION ORDER ---------------------------------
  BWAPI_FUNCTION void OrderCancelConstruction(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- HALT CONSTRUCTION ORDER -----------------------------------
  BWAPI_FUNCTION void OrderHaltConstruction(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HaltConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL MORPH ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderCancelMorph(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelMorph;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderCancelTrain(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrain;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN SLOT ORDER -----------------------------------
  BWAPI_FUNCTION void OrderCancelTrainSlot(int unitId, int slotId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrainSlot;
    order.unitIndex = unitId;
    order.extra = slotId;
  }
  //----------------------------------- CANCEL ADDON ORDER ----------------------------------------
  BWAPI_FUNCTION void OrderCancelAddon(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelAddon;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL RESEARCH ORDER -------------------------------------
  BWAPI_FUNCTION void OrderCancelResearch(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelResearch;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL UPGRADE ORDER --------------------------------------
  BWAPI_FUNCTION void OrderCancelUpgrade(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelUpgrade;
    order.unitIndex = unitId;
  }
  //----------------------------------- USE TECH ORDER --------------------------------------------
  BWAPI_FUNCTION void OrderUseTech(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTech;
    order.unitIndex = unitId;

  }
  //----------------------------------- USE TECH POSITION ORDER -----------------------------------
  BWAPI_FUNCTION void OrderUseTechPosition(int unitId, TechTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- USE TECH UNIT ORDER ---------------------------------------
  BWAPI_FUNCTION void OrderUseTechUnit(int unitId, TechTypeId what, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- -----------------------------------------------------------
}
