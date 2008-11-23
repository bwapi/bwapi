#include "OrderTypes.h"
#include "..\\BWAPI\\Unit.h"
#include "Offsets.h"
#include <Util/Exceptions.h>
namespace BW
{
  namespace Orders
  {

    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(BWAPI::Unit *target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const BW::Position& target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const PositionUnitTarget& target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //-------------------------------------- SELECT SINGLE CONSTRUCTOR ---------------------------------------
    SelectSingle::SelectSingle(BWAPI::Unit *select)
    :always0x09(0x09)
    ,always0x01(0x01)
    ,target(select)
    {
    }
    //---------------------------------------- TRAIN UNIT CONSTRUCTOR ----------------------------------------
    TrainUnit::TrainUnit(BW::UnitType type)
    :always0x1f(0x1f)
    ,type(type)
    {
    }
    //--------------------------------------------- MAKE BULDING ---------------------------------------------
    MakeBuilding::MakeBuilding(BW::TilePosition position, BW::UnitType type)
    :always0x0c(0x0c)
    ,position(position)
    ,type(type)
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
    :always0x30(0x30)
    ,tech(tech)
    {
    }
    //----------------------------------------------- UPGRADE ------------------------------------------------
    Upgrade::Upgrade(BW::UpgradeType upgrade)
    :always0x32(0x32)
    ,upgrade(upgrade)
    {
    }    
    //---------------------------------------------- MAKE ADDON ----------------------------------------------
    MakeAddon::MakeAddon(BW::TilePosition position, BW::UnitType type)
    :always0x0c(0x0c)
    ,always0x24(0x24)
    ,position(position)
    ,type(type)
    {
    }    
    //------------------------------------------- MOVE CONSTRUCTOR -------------------------------------------
    ChangeSlot::ChangeSlot(Slot slot, u8 slotID)
    :slot(slot)
    ,slotID(slotID)
    ,always0x44(0x44)
    {
    }
    //--------------------------------------- CHANGE RACE CONSTRUCTOR ----------------------------------------
    ChangeRace::ChangeRace(BW::Race::Enum race, u8 slotID)
    :race(race)
    ,slotID(slotID)
    ,always0x41(0x41)
    {
    }
    //---------------------------------------- START GAME CONSTRUCTOR ----------------------------------------
    StartGame::StartGame()
    :always0x3c(0x3c)
    {
    }
    //------------------------------------------ PAUSE CONSTRUCTOR -------------------------------------------
    PauseGame::PauseGame()
    :always0x10(0x10)
    {
    }
    //------------------------------------------ RESUME CONSTRUCTOR ------------------------------------------
    ResumeGame::ResumeGame()
    :always0x11(0x11)
    {
    }
    //-------------------------------------- MERGE DARK ARCHON CONSTRUCTOR -----------------------------------
    MergeDarkArchon::MergeDarkArchon()
    :always0x5A(0x5A)
    {
    }
    //-------------------------------------- MERGE ARCHON CONSTRUCTOR -----------------------------------
    MergeArchon::MergeArchon()
    :always0x2A(0x2A)
    {
    }
    //-------------------------------------------- MINIMAP PING -----------------------------------------------
    MinimapPing::MinimapPing(BW::Position position)
    :always0x58(0x58)
    ,position(position)
    {
    }
    //------------------------------------------ STIM CONSTRUCTOR --------------------------------------------
    UseStimPack::UseStimPack()
    :always0x36(0x36)
    {
    }
    //---------------------------------------- BUILDING MORPH CONSTRUCTOR ------------------------------------
    BuildingMorph::BuildingMorph(BW::UnitType type)
    :always0x35(0x35)
    ,type(type)
    {
    }
    //------------------------------------------ CANCEL ADDON ------------------------------------------------
    CancelAddon::CancelAddon()
    :always0x34(0x34)
    {
    }
    //------------------------------------------ CANCEL UPGRADE ------------------------------------------------
    CancelUpgrade::CancelUpgrade()
    :always0x33(0x33)
    {
    }
    //------------------------------------------ CANCEL RESEARCH ------------------------------------------------
    CancelResearch::CancelResearch()
    :always0x31(0x31)
    {
    }
    //------------------------------------------ CANCEL NUKE ------------------------------------------------
    CancelNuke::CancelNuke()
    :always0x2E(0x2E)
    {
    }
    //------------------------------------------ LIFTOFF ------------------------------------------------
    Lift::Lift()
    :always0x2F(0x2F)
    ,unknown(0x00)
    {
    }
    //------------------------------------------ BURROW ------------------------------------------------
    Burrow::Burrow()
    :always0x2C(0x2C)
    ,unused(0x00)
    {
    }
    //------------------------------------------ UNBURROW ------------------------------------------------
    Unburrow::Unburrow()
    :always0x2D(0x2D)
    ,unused(0x00)
    {
    }
    //------------------------------------------ HOLD POSITION ------------------------------------------------
    HoldPosition::HoldPosition(u8 type)
    :always0x2B(0x2B)
    ,type(0x00)
    {
    }

    //------------------------------------------------       -------------------------------------------------
  }
}