#include "OrderTypes.h"
#include <BWAPI/Unit.h>
#include <BW/Offsets.h>
#include <BW/UnitID.h>
#include <BW/OrderID.h>
#include <Util/Exceptions.h>

#include "../../Debug.h"
namespace BW
{
  namespace Orders
  {

    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(BWAPI::UnitImpl* target, u8 OrderID, bool queued)
        : always0x15(0x15)
        , target(target)
        , always0xe4(BW::UnitID::None)
        , order(OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const BW::Position& target, u8 OrderID, bool queued)
        : always0x15(0x15)
        , target(target)
        , always0xe4(BW::UnitID::None)
        , order(OrderID)
        , type(queued ? 1 : 0)
    {
    }
    Attack::Attack(const BWAPI::Position& target, u8 OrderID, bool queued)
        : always0x15(0x15)
        , target(BW::Position((u16)target.x(), (u16)target.y()))
        , always0xe4(BW::UnitID::None)
        , order(OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const PositionUnitTarget& target, u8 OrderID, bool queued)
        : always0x15(0x15)
        , target(target)
        , always0xe4(BW::UnitID::None)
        , order(OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(BWAPI::UnitImpl* target, bool queued)
        : always0x14(0x14)
        , target(target)
        , always0xe4(BW::UnitID::None)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const BW::Position& target, bool queued)
        : always0x14(0x14)
        , target(target)
        , always0xe4(BW::UnitID::None)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const PositionUnitTarget& target, bool queued)
        : always0x14(0x14)
        , target(target)
        , always0xe4(BW::UnitID::None)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- SHIFT SELECT CONSTRUCTOR ---------------------------------------
    SelectAdd::SelectAdd(u8 count, BWAPI::UnitImpl **units)
        : always0x0A(0x0A)
    {
      u8 finalCount = 0;
      for ( unsigned int i = 0; i < count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = finalCount;
      size      = 2 + targCount * 2;
    }
    SelectAdd::SelectAdd(u8 count, BW::Unit **units)
        : always0x0A(0x0A)
    {
      u8 finalCount = 0;
      for ( unsigned int i = 0; i < count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = finalCount;
      size      = 2 + targCount * 2;
    }
    //----------------------------------------- SELECT CONSTRUCTOR -------------------------------------------
    Select::Select(u8 count, BWAPI::UnitImpl **units)
        : always0x09(0x09)
    {
      u8 finalCount = 0;
      for ( unsigned int i = 0; i < count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = finalCount;
      size      = 2 + targCount * 2;
    }
    Select::Select(u8 count, BW::Unit **units)
        : always0x09(0x09)
    {
      u8 finalCount = 0;
      for ( unsigned int i = 0; i < count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = finalCount;
      size      = 2 + targCount * 2;
    }
    Select::Select(std::vector<BW::Unit*> vUnits)
      : always0x09(0x09)
    {
      u8 finalCount = 0;
      for ( unsigned int i = 0; i < vUnits.size() && i < 12; ++i )
        if ( vUnits[i] )
          targets[finalCount++] = UnitTarget(vUnits[i]);

      targCount = finalCount;
      size      = 2 + targCount * 2;
    }
    Select::Select(std::vector<BWAPI::UnitImpl*> vUnits)
      : always0x09(0x09)
    {
      u8 finalCount = 0;
      for ( unsigned int i = 0; i < vUnits.size() && i < 12; ++i )
        if ( vUnits[i] )
          targets[finalCount++] = UnitTarget(vUnits[i]);

      targCount = finalCount;
      size      = 2 + targCount * 2;
    }
    //---------------------------------------- TRAIN UNIT CONSTRUCTOR ----------------------------------------
    TrainUnit::TrainUnit(int type)
        : always0x1f(0x1f)
        , type((u16)type)
    {
    }
    //--------------------------------------- TRAIN FIGHTER CONSTRUCTOR --------------------------------------
    TrainFighter::TrainFighter()
        : always0x27(0x27)
    {
    }
    //--------------------------------------------- MAKE BULDING ---------------------------------------------
    MakeBuilding::MakeBuilding(BW::TilePosition position, int type)
        : always0x0c(0x0c)
        , position(position)
        , type((u16)type)
        , raceDependant(0)
    {
      switch( BWAPI::UnitType(type).getRace() )
      {
      case BW::Race::Zerg:
        raceDependant = BW::OrderID::DroneStartBuild;
        break;
      case BW::Race::Terran:
        raceDependant = BW::OrderID::BuildTerran;
        break;
      case BW::Race::Protoss:
        raceDependant = BW::OrderID::BuildProtoss1;
        break;
      default:
        throw GeneralException("MakeBuilding::MakeBuilding - wrong race type of the worker");
        break;
      }
    }
    MakeBuilding::MakeBuilding(int tileX, int tileY, int type)
        : always0x0c(0x0c)
        , position(BW::TilePosition((u16)tileX, (u16)tileY))
        , type((u16)type)
        , raceDependant(0)
    {
      switch( BWAPI::UnitType(type).getRace() )
      {
      case BW::Race::Zerg:
        raceDependant = BW::OrderID::DroneStartBuild;
        break;
      case BW::Race::Terran:
        raceDependant = BW::OrderID::BuildTerran;
        break;
      case BW::Race::Protoss:
        raceDependant = BW::OrderID::BuildProtoss1;
        break;
      default:
        throw GeneralException("MakeBuilding::MakeBuilding - wrong race type of the worker");
        break;
      }
    }
    //---------------------------------------------- PLACE COP -----------------------------------------------
    PlaceCOP::PlaceCOP(BW::TilePosition position, int type)
        : always0x0c(0x0C)
        , always0x9B(BW::OrderID::CTFCOP2)
        , position(position)
        , type((u16)type)
    {
    }
    //--------------------------------------------- INVENT TECH ----------------------------------------------
    Invent::Invent(int tech)
        : always0x30(0x30)
        , tech((u8)tech)
    {
    }
    //----------------------------------------------- UPGRADE ------------------------------------------------
    Upgrade::Upgrade(int upgrade)
        : always0x32(0x32)
        , upgrade((u8)upgrade)
    {
    }
    //---------------------------------------------- MAKE ADDON ----------------------------------------------
    MakeAddon::MakeAddon(BW::TilePosition position, int type)
        : always0x0c(0x0c)
        , always0x24(BW::OrderID::PlaceAddon)
        , position(position)
        , type((u16)type)
    {
    }
    MakeAddon::MakeAddon(int tileX, int tileY, int type)
        : always0x0c(0x0c)
        , always0x24(BW::OrderID::PlaceAddon)
        , position(BW::TilePosition((u16)tileX, (u16)tileY))
        , type((u16)type)
    {
    }
    //---------------------------------------------- MAKE NYDUS ----------------------------------------------
    MakeNydusExit::MakeNydusExit(BW::TilePosition position)
        : always0x0c(0x0c)
        , always0x2E(BW::OrderID::BuildNydusExit)
        , position(position)
        , type(BW::UnitID::Zerg_NydusCanal)
    {
    }
    MakeNydusExit::MakeNydusExit(int tileX, int tileY)
        : always0x0c(0x0c)
        , always0x2E(BW::OrderID::BuildNydusExit)
        , position(BW::TilePosition((u16)tileX, (u16)tileY))
        , type(BW::UnitID::Zerg_NydusCanal)
    {
    }
    //------------------------------------------- MOVE CONSTRUCTOR -------------------------------------------
    ChangeSlot::ChangeSlot(Slot slot, u8 slotID)
        : slot(slot)
        , slotID(slotID)
        , always0x44(0x44)
    {
    }
    //--------------------------------------- CHANGE RACE CONSTRUCTOR ----------------------------------------
    ChangeRace::ChangeRace(u8 race, u8 slotID)
        : race(race)
        , slotID(slotID)
        , always0x41(0x41)
    {
    }
    //---------------------------------------- START GAME CONSTRUCTOR ----------------------------------------
    StartGame::StartGame()
        : always0x3c(0x3c)
    {
    }
    //------------------------------------------ PAUSE CONSTRUCTOR -------------------------------------------
    PauseGame::PauseGame()
        : always0x10(0x10)
    {
    }
    //------------------------------------------ RESUME CONSTRUCTOR ------------------------------------------
    ResumeGame::ResumeGame()
        : always0x11(0x11)
    {
    }
    //------------------------------------------ RESUME CONSTRUCTOR ------------------------------------------
    LeaveGame::LeaveGame(u8 type)
        : always0x57(0x57)
        , type(type)
    {
    }
    //-------------------------------------- MERGE DARK ARCHON CONSTRUCTOR -----------------------------------
    MergeDarkArchon::MergeDarkArchon()
        : always0x5A(0x5A)
    {
    }
    //-------------------------------------- MERGE ARCHON CONSTRUCTOR ----------------------------------------
    MergeArchon::MergeArchon()
        : always0x2A(0x2A)
    {
    }
    //-------------------------------------------- MINIMAP PING ----------------------------------------------
    MinimapPing::MinimapPing(BW::Position position)
        : always0x58(0x58)
        , position(position)
    {
    }
    //------------------------------------------ STIM CONSTRUCTOR --------------------------------------------
    UseStimPack::UseStimPack()
        : always0x36(0x36)
    {
    }
    //---------------------------------------- BUILDING MORPH CONSTRUCTOR ------------------------------------
    BuildingMorph::BuildingMorph(int type)
        : always0x35(0x35)
        , type((u16)type)
    {
    }
    //------------------------------------------ CANCEL ADDON ------------------------------------------------
    CancelAddon::CancelAddon()
        : always0x34(0x34)
    {
    }
    //------------------------------------------ CANCEL UPGRADE ----------------------------------------------
    CancelUpgrade::CancelUpgrade()
        : always0x33(0x33)
    {
    }
    //------------------------------------------ CANCEL RESEARCH ---------------------------------------------
    CancelResearch::CancelResearch()
        : always0x31(0x31)
    {
    }
    //------------------------------------------ CANCEL NUKE -------------------------------------------------
    CancelNuke::CancelNuke()
        : always0x2E(0x2E)
    {
    }
    //------------------------------------------------ LIFTOFF -----------------------------------------------
    Lift::Lift()
        : always0x2F(0x2F)
        , unknown(0x00)
    {
    }
    //------------------------------------------------- LAND -------------------------------------------------
    Land::Land(BW::TilePosition position, int type)
        : always0x0C(0x0C)
        , always0x47(BW::OrderID::BuildingLand)
        , position(position)
        , type((u16)type)
    {
    }
    //---------------------------------------------- BURROW --------------------------------------------------
    Burrow::Burrow()
        : always0x2C(0x2C)
        , unused(0x00)
    {
    }
    //----------------------------------------------- UNBURROW -----------------------------------------------
    Unburrow::Unburrow()
        : always0x2D(0x2D)
        , unused(0x00)
    {
    }
    //--------------------------------------------- HOLD POSITION --------------------------------------------
    HoldPosition::HoldPosition(bool queued)
        : always0x2B(0x2B)
        , type(queued ? 1 : 0)
    {
    }
    //------------------------------------------------ UNLOAD UNIT -------------------------------------------
    UnloadUnit::UnloadUnit(BWAPI::UnitImpl* unload)
        : always0x29(0x29)
        , target(unload)
    {
    }
    //---------------------------------------------- UNLOAD ALL ----------------------------------------------
    UnloadAll::UnloadAll()
        : always0x28(0x28)
        , unused(0x00)
    {
    }
    //------------------------------------------------- SIEGE ------------------------------------------------
    Siege::Siege()
        : always0x26(0x26)
        , unused(0x00)
    {
    }
    //------------------------------------------------ UNSIEGE -----------------------------------------------
    Unsiege::Unsiege()
        : always0x25(0x25)
        , unused(0x00)
    {
    }
    //----------------------------------------- UNIT MORPH CONSTRUCTOR ---------------------------------------
    UnitMorph::UnitMorph(int type)
        : always0x23(0x23)
        , type((u16)type)
    {
    }
    //------------------------------------------------ CLOAK -------------------------------------------------
    Cloak::Cloak()
        : always0x21(0x21)
        , unused(0x00)
    {
    }
    //------------------------------------------------ DECLOAK -----------------------------------------------
    Decloak::Decloak()
        : always0x22(0x22)
        , unused(0x00)
    {
    }
    //------------------------------------------ RETURN CARGO ------------------------------------------------
    ReturnCargo::ReturnCargo(bool queued)
        : always0x1E(0x1E)
        , type(queued ? 1 : 0)
    {
    }
    //------------------------------------------------ STOP --------------------------------------------------
    Stop::Stop(bool queued)
        : always0x1A(0x1A)
        , type(queued ? 1 : 0)
    {
    }
    //------------------------------------------ REAVER STOP -------------------------------------------------
    ReaverStop::ReaverStop()
        : always0x1C(0x1C)
    {
    }
    //------------------------------------------ CARRIER STOP ------------------------------------------------
    CarrierStop::CarrierStop()
        : always0x1B(0x1B)
    {
    }
    //------------------------------------------ CANCEL UNIT MORPH -------------------------------------------
    CancelUnitMorph::CancelUnitMorph()
        : always0x19(0x19)
    {
    }
    //------------------------------------------ CANCEL CONSTRUCTION -----------------------------------------
    CancelConstruction::CancelConstruction()
        : always0x18(0x18)
    {
    }
    //--------------------------------------------- CANCEL TRAIN ---------------------------------------------
    CancelTrain::CancelTrain(s8 slot)
        : always0x20(0x20)
        , slot(slot)
        , unknown(0)
    {
    }
    //------------------------------------------------ USE CHEAT ---------------------------------------------
    UseCheat::UseCheat(u32 flags)
      : always0x12(0x12)
      , flags(flags)
    {
    }
    //----------------------------------------------- RESTART GAME -------------------------------------------
    RestartGame::RestartGame()
      : always0x08(0x08)
    {
    }
    //------------------------------------------------ SET ALLIES --------------------------------------------
    SetAllies::SetAllies(u32 allies)
      : always0x0E(0x0E)
      , allies(allies)
    {
    }
    //------------------------------------------------ SET VISION --------------------------------------------
    SetVision::SetVision(u16 vision)
      : always0x0D(0x0D)
      , vision(vision)
    {
    }

  }
};
