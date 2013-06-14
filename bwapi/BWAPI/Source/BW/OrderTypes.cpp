#include "OrderTypes.h"
#include <BWAPI/Unit.h>
#include <BWAPI/Unitset.h>

#include <BWAPI/Order.h>
#include <BWAPI/Race.h>
#include <BW/Offsets.h>
#include <Util/Exceptions.h>

#include "../../../Debug.h"
namespace BW
{
  namespace Orders
  {

    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(BWAPI::Unit target, int OrderID, bool queued)
        : always0x15(0x15)
        , target((BWAPI::UnitImpl*)target)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const BW::Position& target, int OrderID, bool queued)
        : always0x15(0x15)
        , target(target)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    Attack::Attack(int x, int y, int OrderID, bool queued)
        : always0x15(0x15)
        , target(x,y)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const PositionUnitTarget& target, int OrderID, bool queued)
        : always0x15(0x15)
        , target(target)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(BWAPI::Unit target, bool queued)
        : always0x14(0x14)
        , target((BWAPI::UnitImpl*)target)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const BW::Position& target, bool queued)
        : always0x14(0x14)
        , target(target)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , type(queued ? 1 : 0)
    {
    }
    RightClick::RightClick(int x, int y, bool queued)
        : always0x14(0x14)
        , target(x,y)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const PositionUnitTarget& target, bool queued)
        : always0x14(0x14)
        , target(target)
        , always0xe4((u16)BWAPI::UnitTypes::None)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- SHIFT SELECT CONSTRUCTOR ---------------------------------------
    SelectAdd::SelectAdd(int count, BWAPI::UnitImpl **units)
        : always0x0A(0x0A)
    {
      unsigned int finalCount = 0;
      for ( unsigned int i = 0; i < (unsigned int)count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = (u8)finalCount;
      size      = 2 + targCount * 2;
    }
    SelectAdd::SelectAdd(int count, BW::CUnit **units)
        : always0x0A(0x0A)
    {
      unsigned int finalCount = 0;
      for ( unsigned int i = 0; i < (unsigned int)count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = (u8)finalCount;
      size      = 2 + targCount * 2;
    }
    //----------------------------------------- SELECT CONSTRUCTOR -------------------------------------------
    Select::Select(int count, BWAPI::UnitImpl **units)
        : always0x09(0x09)
    {
      unsigned int finalCount = 0;
      for ( unsigned int i = 0; i < (unsigned int)count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = (u8)finalCount;
      size      = 2 + targCount * 2;
    }
    Select::Select(int count, BW::CUnit **units)
        : always0x09(0x09)
    {
      unsigned int finalCount = 0;
      for ( unsigned int i = 0; i < (unsigned int)count && i < 12; ++i )
        if ( units[i] )
          targets[finalCount++] = UnitTarget(units[i]);

      targCount = (u8)finalCount;
      size      = 2 + targCount * 2;
    }
    Select::Select(const BWAPI::Unitset &unitset)
      : always0x09(0x09)
    {
      unsigned int finalCount = 0;
      for ( unsigned int i = 0; i < unitset.size() && i < 12; ++i )
        if ( unitset[i] != NULL )
          targets[finalCount++] = UnitTarget((BWAPI::UnitImpl*)unitset[i]);

      targCount = (u8)finalCount;
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
        , raceDependant(0)
        , position(position)
        , type((u16)type)
    {
      switch( BWAPI::UnitType(type).getRace() )
      {
      case BWAPI::Races::Enum::Zerg:
        raceDependant = BWAPI::Orders::Enum::DroneStartBuild;
        break;
      case BWAPI::Races::Enum::Terran:
        raceDependant = BWAPI::Orders::Enum::PlaceBuilding;
        break;
      case BWAPI::Races::Enum::Protoss:
        raceDependant = BWAPI::Orders::Enum::PlaceProtossBuilding;
        break;
      default:
        throw GeneralException("MakeBuilding::MakeBuilding - wrong race type of the worker");
        break;
      }
    }
    MakeBuilding::MakeBuilding(int tileX, int tileY, int type)
        : always0x0c(0x0c)
        , raceDependant(0)
        , position((u16)tileX, (u16)tileY)
        , type((u16)type)
    {
      switch( BWAPI::UnitType(type).getRace() )
      {
      case BWAPI::Races::Enum::Zerg:
        raceDependant = BWAPI::Orders::Enum::DroneStartBuild;
        break;
      case BWAPI::Races::Enum::Terran:
        raceDependant = BWAPI::Orders::Enum::PlaceBuilding;
        break;
      case BWAPI::Races::Enum::Protoss:
        raceDependant = BWAPI::Orders::Enum::PlaceProtossBuilding;
        break;
      default:
        throw GeneralException("MakeBuilding::MakeBuilding - wrong race type of the worker");
        break;
      }
    }
    //---------------------------------------------- PLACE COP -----------------------------------------------
    PlaceCOP::PlaceCOP(BW::TilePosition position, int type)
        : always0x0c(0x0C)
        , always0x9B((u8)BWAPI::Orders::CTFCOP2)
        , position(position)
        , type((u16)type)
    {
    }
    PlaceCOP::PlaceCOP(int x, int y, int type)
        : always0x0c(0x0C)
        , always0x9B((u8)BWAPI::Orders::CTFCOP2)
        , position((u16)x, (u16)y)
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
        , always0x24((u8)BWAPI::Orders::PlaceAddon)
        , position(position)
        , type((u16)type)
    {
    }
    MakeAddon::MakeAddon(int tileX, int tileY, int type)
        : always0x0c(0x0c)
        , always0x24((u8)BWAPI::Orders::PlaceAddon)
        , position((u16)tileX, (u16)tileY)
        , type((u16)type)
    {
    }
    //---------------------------------------------- MAKE NYDUS ----------------------------------------------
    MakeNydusExit::MakeNydusExit(BW::TilePosition position)
        : always0x0c(0x0c)
        , always0x2E((u8)BWAPI::Orders::BuildNydusExit)
        , position(position)
        , type((u16)BWAPI::UnitTypes::Zerg_Nydus_Canal)
    {
    }
    MakeNydusExit::MakeNydusExit(int tileX, int tileY)
        : always0x0c(0x0c)
        , always0x2E((u8)BWAPI::Orders::BuildNydusExit)
        , position((u16)tileX, (u16)tileY)
        , type((u16)BWAPI::UnitTypes::Zerg_Nydus_Canal)
    {
    }
    //------------------------------------------- MOVE CONSTRUCTOR -------------------------------------------
    ChangeSlot::ChangeSlot(int slotID, SlotType type)
    : always0x44(0x44)
    , slotID((u8)slotID)
    , slotType((u8)type)
    {
    }
    //--------------------------------------- CHANGE RACE CONSTRUCTOR ----------------------------------------
    RequestChangeRace::RequestChangeRace(int slot, int race)
    : always0x41(0x41)
    , slotID((u8)slot)
    , race((u8)race)
    {
    }
    UpdateSlot::UpdateSlot(int slot, int stormPlayerID, int owner, int newRace, int team)
      : always0x3E(0x3E)
      , bSlot((u8)slot)
      , bStormPlayerID((u8)stormPlayerID)
      , nType((u8)owner)
      , bNewRace((u8)newRace)
      , nTeam((u8)team)
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
    LeaveGame::LeaveGame(int type)
        : always0x57(0x57)
        , type((u8)type)
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
    MinimapPing::MinimapPing(int x, int y)
        : always0x58(0x58)
        , position((u16)x, (u16)y)
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
        , unknown(0)
    {
    }
    //------------------------------------------------- LAND -------------------------------------------------
    Land::Land(BW::TilePosition position, int type)
        : always0x0C(0x0C)
        , always0x47((u8)BWAPI::Orders::Enum::BuildingLand)
        , position(position)
        , type((u16)type)
    {
    }
    Land::Land(int x, int y, int type)
        : always0x0C(0x0C)
        , always0x47((u8)BWAPI::Orders::Enum::BuildingLand)
        , position((u16)x, (u16)y)
        , type((u16)type)
    {
    }
    //---------------------------------------------- BURROW --------------------------------------------------
    Burrow::Burrow()
        : always0x2C(0x2C)
        , unused(0)
    {
    }
    //----------------------------------------------- UNBURROW -----------------------------------------------
    Unburrow::Unburrow()
        : always0x2D(0x2D)
        , unused(0)
    {
    }
    //--------------------------------------------- HOLD POSITION --------------------------------------------
    HoldPosition::HoldPosition(bool queued)
        : always0x2B(0x2B)
        , type(queued ? 1 : 0)
    {
    }
    //------------------------------------------------ UNLOAD UNIT -------------------------------------------
    UnloadUnit::UnloadUnit(BWAPI::Unit unload)
        : always0x29(0x29)
        , target((BWAPI::UnitImpl*)unload)
    {
    }
    //---------------------------------------------- UNLOAD ALL ----------------------------------------------
    UnloadAll::UnloadAll()
        : always0x28(0x28)
        , unused(0)
    {
    }
    //------------------------------------------------- SIEGE ------------------------------------------------
    Siege::Siege()
        : always0x26(0x26)
        , unused(0)
    {
    }
    //------------------------------------------------ UNSIEGE -----------------------------------------------
    Unsiege::Unsiege()
        : always0x25(0x25)
        , unused(0)
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
        , unused(0)
    {
    }
    //------------------------------------------------ DECLOAK -----------------------------------------------
    Decloak::Decloak()
        : always0x22(0x22)
        , unused(0)
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
    CancelTrain::CancelTrain(int slot)
        : always0x20(0x20)
        , slot((s8)slot)
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
