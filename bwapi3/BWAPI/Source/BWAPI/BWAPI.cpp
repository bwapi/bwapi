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
  BWAPI_FUNCTION int getVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  BWAPI_FUNCTION bool connect() throw()
  {
    return BridgeClient::connect();
  }
  //----------------------------------- WAIT FOR EVENT --------------------------------------------
  BWAPI_FUNCTION CallTypeId waitForEvent()
  {
    while(true)
    {
      BridgeClient::waitForEvent();

      // react upon bridge state
      BridgeClient::RpcState rpcState = BridgeClient::getCurrentRpc();

      switch(rpcState)
      {
      case BridgeClient::OnFrame:
        {
          if(BridgeClient::gameData->isOnMatchStart)
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
          }
        }return CallTypeIds::OnFrame;
      }
    }
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool printText(const char* text)
  {
    return BridgeClient::pushSendText(false, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool sendText(const char* text)
  {
    return BridgeClient::pushSendText(true, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION void drawText(CoordinateBase base, Position pos, const char* text)
  {
    BridgeClient::pushDrawText(base, pos, text);
  }
  //----------------------------------- DRAW RECTANGLE --------------------------------------------
  BWAPI_FUNCTION void drawRectangle(CoordinateBase base, Position pos, Position size, int color, bool solid)
  {
    BridgeClient::pushDrawRectangle(base, pos, size, color, !!solid);
  }
  //----------------------------------- DRAW CIRCLE -----------------------------------------------
  BWAPI_FUNCTION void drawCircle(CoordinateBase base, Position pos, int r, int color, bool solid)
  {
    BridgeClient::pushDrawCircle(base, pos, r, color, !!solid);
  }
  //----------------------------------- DRAW ELLIPSE ----------------------------------------------
  BWAPI_FUNCTION void drawEllipse(CoordinateBase base, Position pos, int w, int h, int color, bool solid)
  {
    BridgeClient::pushDrawEllipse(base, pos, w, h, color, !!solid);
  }
  //----------------------------------- DRAW LINE -------------------------------------------------
  BWAPI_FUNCTION void drawLine(CoordinateBase base, Position pos, Position pos2, int color)
  {
    BridgeClient::pushDrawLine(base, pos, pos2, color);
  }
  //----------------------------------- DRAW DOT --------------------------------------------------
  BWAPI_FUNCTION void drawDot(CoordinateBase base, Position pos, int color)
  {
    BridgeClient::pushDrawDot(base, pos, color);
  }
  //----------------------------------- DRAW TRIANGLE ---------------------------------------------
  BWAPI_FUNCTION void drawTriangle(CoordinateBase base, Position posa, Position posb, Position posc, int color, bool solid)
  {
    BridgeClient::pushDrawTriangle(base, posa, posb, posc, color, solid);
  }
  //----------------------------------- GET STATIC DATA -------------------------------------------
  BWAPI_FUNCTION const StaticGameData* getStaticGameData()
  {
    if(!BridgeClient::isConnected())
      throw GeneralException("GetStaticGameData requires connection");
    StaticGameData *retval = BridgeClient::gameData;
    return retval;
  }
  //----------------------------------- GET DYNAMIC EVENT CONTAINERS ------------------------------
  BWAPI_FUNCTION const DynamicGameData* getDynamicGameData()
  {
    if(!BridgeClient::isConnected())
      throw GeneralException("GetDynamicGameData requires connection");
    DynamicGameData *retval = &BridgeClient::dynamicData;
    return retval;
  }
  //----------------------------------- GET UNIT --------------------------------------------------
  BWAPI_FUNCTION const UnitState* getUnit(UnitId unitId)
  {
    return &BridgeClient::gameData->units.at(unitId);
  }
  //----------------------------------- GET PLAYER ------------------------------------------------
  BWAPI_FUNCTION const Player* getPlayer(PlayerId playerId)
  {
    return &BridgeClient::gameData->players.at(playerId);
  }
  //----------------------------------- GET FORCE -------------------------------------------------
  BWAPI_FUNCTION const Force* getForce(ForceId forceId)
  {
    return &BridgeClient::gameData->forces[forceId];
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION const UnitType* getUnitType(UnitTypeId id)
  {
    return &UnitTypes::unitTypeData[id];
  }
  //----------------------------------- GET TECH TYPE ---------------------------------------------
  BWAPI_FUNCTION const TechType* getTechType(TechTypeId id)
  {
    return &TechTypes::techTypeData[id];
  }
  //----------------------------------- GET UPGRADE TYPE ------------------------------------------
  BWAPI_FUNCTION const UpgradeType* getUpgradeType(UpgradeTypeId id)
  {
    return &upgradeTypeData[id];
  }
  //----------------------------------- GET WEAPON TYPE -------------------------------------------
  BWAPI_FUNCTION const WeaponType* getWeaponType(WeaponTypeId id)
  {
    return &WeaponTypes::weaponTypeData[id];
  }
  //----------------------------------- GET DAMAGE TYPE -------------------------------------------
  BWAPI_FUNCTION const DamageType* getDamageType(DamageTypeId id)
  {
    return &DamageTypes::damageTypeData[id];
  }
  //----------------------------------- GET EXPLOSION TYPE ----------------------------------------
  BWAPI_FUNCTION const ExplosionType* getExplosionType(ExplosionTypeId id)
  {
    return &ExplosionTypes::explosionTypeData[id];
  }
  //----------------------------------- GET RACE --------------------------------------------------
  BWAPI_FUNCTION const Race* getRace(RaceId id)
  {
      return &Races::raceData[id];
  }
  //----------------------------------- GET UNIT SIZE TYPE ----------------------------------------
  BWAPI_FUNCTION const UnitSizeType* getUnitSizeType(UnitSizeTypeId id)
  {
    return &UnitSizeTypes::unitSizeTypeData[id];
  }
  //----------------------------------- GET PLAYER TYPE -------------------------------------------
  BWAPI_FUNCTION const PlayerType* getPlayerType(PlayerTypeId id)
  {
    return &PlayerTypes::playerTypeData[id];
  }
  //----------------------------------- -----------------------------------------------------------
  //----------------------------------- INSERT COMMAND --------------------------------------------
  template<typename T>
    void pushCommand(const T& cmd)
    {
      Bridge::SharedStuff::CommandStack::Index i = BridgeClient::sharedStuff.commands.insert(Util::MemoryFrame::from(cmd));
    }
  //----------------------------------- COMMANDS --------------------------------------------------
  BWAPI_FUNCTION void startGame()
  {
    pushCommand(Bridge::CommandEntry::StartGame());
  }
  BWAPI_FUNCTION void pauseGame()
  {
    pushCommand(Bridge::CommandEntry::PauseGame());
  }
  BWAPI_FUNCTION void leaveGame()
  {
    pushCommand(Bridge::CommandEntry::LeaveGame());
  }
  BWAPI_FUNCTION void resumeGame()
  {
    pushCommand(Bridge::CommandEntry::ResumeGame());
  }
  BWAPI_FUNCTION void restartGame()
  {
    pushCommand(Bridge::CommandEntry::RestartGame());
  }
  BWAPI_FUNCTION void setLocalSpeed(int speed)
  {
    Bridge::CommandEntry::SetLocalSpeed cmd;
    cmd.speed = speed;
    pushCommand(cmd);
  }
  BWAPI_FUNCTION void pingMinimap(Position pos)
  {
    Bridge::CommandEntry::PingMinimap cmd;
    cmd.pos = pos;
    pushCommand(cmd);
  }
  BWAPI_FUNCTION void enableFlag(Flag flag)
  {
    Bridge::CommandEntry::EnableFlag cmd;
    cmd.flag = flag;
    pushCommand(cmd);
  }
  BWAPI_FUNCTION void setScreenPosition(Position pos)
  {
    Bridge::CommandEntry::SetScreenPosition cmd;
    cmd.pos = pos;
    pushCommand(cmd);
  }
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
  BWAPI_FUNCTION void orderAttackPosition(UnitId unitId, Position pos)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackPosition;
    order.unitIndex = unitId;
    order.position = pos;
  }
  //----------------------------------- ATTACK UNIT ORDER -----------------------------------------
  BWAPI_FUNCTION void orderAttackUnit(UnitId unitId, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RIGHT CLICK POSITION ORDER --------------------------------
  BWAPI_FUNCTION void orderRightClickPosition(UnitId unitId, Position pos)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickPosition;
    order.unitIndex = unitId;
    order.position = pos;
  }
  //----------------------------------- RIGHT CLICK UNIT ORDER ------------------------------------
  BWAPI_FUNCTION void orderRightClickUnit(UnitId unitId, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- TRAIN ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderTrain(UnitId unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Train;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BUILD ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderBuild(UnitId unitId, Position pos, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Build;
    order.unitIndex = unitId;
    order.unitType = what;
    order.position = pos;
  }
  //----------------------------------- BUILD ADDON ORDER -----------------------------------------
  BWAPI_FUNCTION void orderBuildAddon(UnitId unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::BuildAddon;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- RESEARCH ORDER --------------------------------------------
  BWAPI_FUNCTION void orderResearch(UnitId unitId, TechTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Research;
    order.unitIndex = unitId;
    order.tech = what;
  }
  //----------------------------------- UPGRADE ORDER ---------------------------------------------
  BWAPI_FUNCTION void orderUpgrade(UnitId unitId, UpgradeTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Upgrade;
    order.unitIndex = unitId;
    order.upgrade = what;
  }
  //----------------------------------- STOP ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderStop(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Stop;
    order.unitIndex = unitId;
  }
  //----------------------------------- HOLD POSITION ORDER ---------------------------------------
  BWAPI_FUNCTION void orderHoldPosition(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HoldPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- PATROL ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderPatrol(UnitId unitId, Position pos)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Patrol;
    order.unitIndex = unitId;
    order.position = pos;
  }
  //----------------------------------- FOLLOW ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderFollow(UnitId unitId, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Follow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SET RALLY POSITION ORDER ----------------------------------
  BWAPI_FUNCTION void orderSetRallyPosition(UnitId unitId, Position pos)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyPosition;
    order.unitIndex = unitId;
    order.position = pos;
  }
  //----------------------------------- SET RALLY UNIT ORDER --------------------------------------
  BWAPI_FUNCTION void orderSetRallyUnit(UnitId unitId, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyUnit;
    order.unitIndex = unitId;
  }
  //----------------------------------- REPAIR ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderRepair(UnitId unitId, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Repair;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RETURN CARGO ORDER ----------------------------------------
  BWAPI_FUNCTION void orderReturnCargo(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::ReturnCargo;
    order.unitIndex = unitId;
  }
  //----------------------------------- MORPH ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderMorph(UnitId unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Morph;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BURROW ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderBurrow(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Burrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNBURROW ORDER --------------------------------------------
  BWAPI_FUNCTION void orderUnburrow(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unburrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SIEGE ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderSiege(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Siege;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNSIEGE ORDER ---------------------------------------------
  BWAPI_FUNCTION void orderUnsiege(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unsiege;
    order.unitIndex = unitId;
  }
  //----------------------------------- CLOAK ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderCloak(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Cloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- DECLOAK ORDER ---------------------------------------------
  BWAPI_FUNCTION void orderDecloak(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Decloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- LIFT ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderLift(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Lift;
    order.unitIndex = unitId;
  }
  //----------------------------------- LAND ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderLand(UnitId unitId, Position pos)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Land;
    order.unitIndex = unitId;
    order.position = pos;
  }
  //----------------------------------- LOAD ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderLoad(UnitId unitId, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Load;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderUnload(UnitId unitId, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unload;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ALL ORDER ------------------------------------------
  BWAPI_FUNCTION void orderUnloadAll(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAll;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNLOAD ALL POSITION ORDER ---------------------------------
  BWAPI_FUNCTION void orderUnloadAllPosition(UnitId unitId, Position pos)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAllPosition;
    order.unitIndex = unitId;
    order.position = pos;
  }
  //----------------------------------- CANCEL CONSTRUCTION ORDER ---------------------------------
  BWAPI_FUNCTION void orderCancelConstruction(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- HALT CONSTRUCTION ORDER -----------------------------------
  BWAPI_FUNCTION void orderHaltConstruction(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HaltConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL MORPH ORDER ----------------------------------------
  BWAPI_FUNCTION void orderCancelMorph(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelMorph;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN ORDER ----------------------------------------
  BWAPI_FUNCTION void orderCancelTrain(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrain;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN SLOT ORDER -----------------------------------
  BWAPI_FUNCTION void orderCancelTrainSlot(UnitId unitId, int slotId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrainSlot;
    order.unitIndex = unitId;
    order.extra = slotId;
  }
  //----------------------------------- CANCEL ADDON ORDER ----------------------------------------
  BWAPI_FUNCTION void orderCancelAddon(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelAddon;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL RESEARCH ORDER -------------------------------------
  BWAPI_FUNCTION void orderCancelResearch(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelResearch;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL UPGRADE ORDER --------------------------------------
  BWAPI_FUNCTION void orderCancelUpgrade(UnitId unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelUpgrade;
    order.unitIndex = unitId;
  }
  //----------------------------------- USE TECH ORDER --------------------------------------------
  BWAPI_FUNCTION void orderUseTech(UnitId unitId, TechTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTech;
    order.unitIndex = unitId;
    order.tech = what;

  }
  //----------------------------------- USE TECH POSITION ORDER -----------------------------------
  BWAPI_FUNCTION void orderUseTechPosition(UnitId unitId, TechTypeId what, Position pos)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechPosition;
    order.unitIndex = unitId;
    order.tech = what;
    order.position = pos;
  }
  //----------------------------------- USE TECH UNIT ORDER ---------------------------------------
  BWAPI_FUNCTION void orderUseTechUnit(UnitId unitId, TechTypeId what, UnitId targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechUnit;
    order.unitIndex = unitId;
    order.tech = what;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- -----------------------------------------------------------
}
