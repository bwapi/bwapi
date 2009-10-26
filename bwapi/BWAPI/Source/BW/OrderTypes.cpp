#include "OrderTypes.h"
#include <BWAPI/Unit.h>
#include <BW/Offsets.h>
#include <Util/Exceptions.h>
namespace BW
{
  namespace Orders
  {

    //--------------------------------------- ATTACK LOCATION COONSTRUCTOR -----------------------------------
    Attack::Attack(BWAPI::UnitImpl* target, u8 OrderID)
        : always0x15(0x15)
        , target(target)
        , always0xe4(0xe4)
        , order(OrderID)
        , type(0x00)
    {
    }
    //--------------------------------------- ATTACK LOCATION COONSTRUCTOR -----------------------------------
    Attack::Attack(const BW::Position& target, u8 OrderID)
        : always0x15(0x15)
        , target(target)
        , always0xe4(0xe4)
        , order(OrderID)
        , type(0x00)
    {
    }
    //--------------------------------------- ATTACK LOCATION COONSTRUCTOR -----------------------------------
    Attack::Attack(const PositionUnitTarget& target, u8 OrderID)
        : always0x15(0x15)
        , target(target)
        , always0xe4(0xe4)
        , order(OrderID)
        , type(0x00)
    {
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(BWAPI::UnitImpl* target)
        : always0x14(0x14)
        , target(target)
        , always0xe4(0xe4)
        , alwaysZero(0x0)
    {
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const BW::Position& target)
        : always0x14(0x14)
        , target(target)
        , always0xe4(0xe4)
        , alwaysZero(0x0)
    {
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const PositionUnitTarget& target)
        : always0x14(0x14)
        , target(target)
        , always0xe4(0xe4)
        , alwaysZero(0x0)
    {
    }
    //-------------------------------------- SELECT SINGLE CONSTRUCTOR ---------------------------------------
    SelectSingle::SelectSingle(BWAPI::UnitImpl* select)
        : always0x09(0x09)
        , always0x01(0x01)
        , target(select)
    {
    }
    //---------------------------------------- TRAIN UNIT CONSTRUCTOR ----------------------------------------
    TrainUnit::TrainUnit(BW::UnitType type)
        : always0x1f(0x1f)
        , type(type)
    {
    }
    //--------------------------------------- TRAIN FIGHTER CONSTRUCTOR --------------------------------------
    TrainFighter::TrainFighter()
        : always0x27(0x27)
    {
    }
    //--------------------------------------------- MAKE BULDING ---------------------------------------------
    MakeBuilding::MakeBuilding(BW::TilePosition position, BW::UnitType type)
        : always0x0c(0x0c)
        , position(position)
        , type(type)
    {
      switch (type.getRace())
      {
        case BW::Race::Zerg : raceDependant = 0x19; break;
        case BW::Race::Terran : raceDependant = 0x1e; break;
        case BW::Race::Protoss : raceDependant = 0x1f; break;
        default : throw GeneralException("MakeBuilding::MakeBuilding - wrong race type of the worker");
      }
    }
    //--------------------------------------------- INVENT TECH ----------------------------------------------
    Invent::Invent(BW::TechType tech)
        : always0x30(0x30)
        , tech(tech)
    {
    }
    //----------------------------------------------- UPGRADE ------------------------------------------------
    Upgrade::Upgrade(BW::UpgradeType upgrade)
        : always0x32(0x32)
        , upgrade(upgrade)
    {
    }
    //---------------------------------------------- MAKE ADDON ----------------------------------------------
    MakeAddon::MakeAddon(BW::TilePosition position, BW::UnitType type)
        : always0x0c(0x0c)
        , always0x24(0x24)
        , position(position)
        , type(type)
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
    ChangeRace::ChangeRace(BW::Race::Enum race, u8 slotID)
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
    BuildingMorph::BuildingMorph(BW::UnitType type)
        : always0x35(0x35)
        , type(type)
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
    Land::Land(BW::TilePosition position, BW::UnitType type)
        : always0x0C(0x0C)
        , always0x47(0x47)
        , position(position)
        , type(type)
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
    HoldPosition::HoldPosition(u8 type)
        : always0x2B(0x2B)
        , type(type)
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
    UnitMorph::UnitMorph(BW::UnitType type)
        : always0x23(0x23)
        , type(type)
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
    ReturnCargo::ReturnCargo(u8 type)
        : always0x1E(0x1E)
        , type(type)
    {
    }
    //------------------------------------------------ STOP --------------------------------------------------
    Stop::Stop(u8 type)
        : always0x1A(0x1A)
        , type(type)
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
    CancelTrain::CancelTrain(u8 slot)
        : always0x20(0x20)
        , slot(slot)
        , unknown(0)
    {
    }
    //------------------------------------------- CANCEL TRAIN LAST ------------------------------------------
    CancelTrainLast::CancelTrainLast()
        : always0x20(0x20)
        , always0xFE(0xFE)
        , unknown(0)
    {
    }
    //--------------------------------------------------------------------------------------------------------
  }
}