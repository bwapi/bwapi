#include "Command.h"
#include <BW/Offsets.h>
#include <BW/OrderID.h>
#include <BW/UnitID.h>
#include <BW/PositionUnitID.h>

#include <Util/Exceptions.h>
namespace BW
{
  namespace Command
  {

    //--------------------------------------- ATTACK LOCATION COONSTRUCTOR -----------------------------------
    Attack::Attack(BW::UnitID unit, BW::OrderID OrderId)
      : always0xe4(0xe4)
      , order(OrderId)
      , type(0x00)
    {
      target.unitId = unit;
    }
    //--------------------------------------- ATTACK LOCATION COONSTRUCTOR -----------------------------------
    Attack::Attack(const BW::Position& pos, BW::OrderID OrderId)
      : always0xe4(0xe4)
      , order(OrderId)
      , type(0x00)
    {
      target.position = pos;
    }
    //--------------------------------------- ATTACK LOCATION COONSTRUCTOR -----------------------------------
    Attack::Attack(const PositionUnitID& target, BW::OrderID OrderId)
      : target(target)
      , always0xe4(0xe4)
      , order(OrderId)
      , type(0x00)
    {
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(BW::UnitID unitId)
      : always0xe4(0xe4)
      , alwaysZero(0x0)
    {
      target.unitId = unitId;
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const BW::Position& pos)
      : target(target)
      , always0xe4(0xe4)
      , alwaysZero(0x0)
    {
      target.position = pos;
    }
    //--------------------------------------- RIGHT CLICK COONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const PositionUnitID& target)
      : target(target)
      , always0xe4(0xe4)
      , alwaysZero(0x0)
    {
    }
    //-------------------------------------- SELECT SINGLE CONSTRUCTOR ---------------------------------------
    SelectSingle::SelectSingle(BW::UnitID select)
      : always0x01(0x01)
      , target(select)
    {
    }
    //---------------------------------------- TRAIN UNIT CONSTRUCTOR ----------------------------------------
    TrainUnit::TrainUnit(BW::UnitType type)
      : type(type)
    {
    }
    //--------------------------------------- TRAIN FIGHTER CONSTRUCTOR --------------------------------------
    TrainFighter::TrainFighter()
    {
    }
    //--------------------------------------------- MAKE BULDING ---------------------------------------------
    MakeBuilding::MakeBuilding(BW::BuildPosition position, BW::UnitType type)
      : position(position)
      , type(type)
    {
      switch (type.getRace())
      {
        case BW::RaceIDs::Zerg    : raceDependant = 0x19; break;
        case BW::RaceIDs::Terran  : raceDependant = 0x1e; break;
        case BW::RaceIDs::Protoss : raceDependant = 0x1f; break;
        default : throw GeneralException("MakeBuilding::MakeBuilding - faulty race type of the worker");
      }
    }
    //--------------------------------------------- INVENT TECH ----------------------------------------------
    Invent::Invent(BW::TechType tech)
      : tech(tech)
    {
    }
    //----------------------------------------------- UPGRADE ------------------------------------------------
    Upgrade::Upgrade(BW::UpgradeType upgrade)
      : upgrade(upgrade)
    {
    }
    //---------------------------------------------- MAKE ADDON ----------------------------------------------
    MakeAddon::MakeAddon(BW::BuildPosition position, BW::UnitType type)
      : always0x24(0x24)
      , position(position)
      , type(type)
    {
    }
    //------------------------------------------- MOVE CONSTRUCTOR -------------------------------------------
    ChangeSlot::ChangeSlot(u8 slot, u8 slotState)
      : slotId(slot)
      , slotStateId(slotState)
    {
    }
    //--------------------------------------- CHANGE RACE CONSTRUCTOR ----------------------------------------
    ChangeRace::ChangeRace(u8 slot, u8 race)
      : slotId(slot)
      , raceId(race)
    {
    }
    //---------------------------------------- START GAME CONSTRUCTOR ----------------------------------------
    StartGame::StartGame()
    {
    }
    //------------------------------------------ PAUSE CONSTRUCTOR -------------------------------------------
    PauseGame::PauseGame()
    {
    }
    //------------------------------------------ RESUME CONSTRUCTOR ------------------------------------------
    ResumeGame::ResumeGame()
    {
    }
    //------------------------------------------ RESUME CONSTRUCTOR ------------------------------------------
    LeaveGame::LeaveGame(u8 type)
      : type(type)
    {
    }
    //-------------------------------------- MERGE DARK ARCHON CONSTRUCTOR -----------------------------------
    MergeDarkArchon::MergeDarkArchon()
    {
    }
    //-------------------------------------- MERGE ARCHON CONSTRUCTOR ----------------------------------------
    MergeArchon::MergeArchon()
    {
    }
    //-------------------------------------------- MINIMAP PING ----------------------------------------------
    MinimapPing::MinimapPing(BW::Position position)
      : position(position)
    {
    }
    //------------------------------------------ STIM CONSTRUCTOR --------------------------------------------
    UseStimPack::UseStimPack()
    {
    }
    //---------------------------------------- BUILDING MORPH CONSTRUCTOR ------------------------------------
    BuildingMorph::BuildingMorph(BW::UnitType type)
      : type(type)
    {
    }
    //------------------------------------------ CANCEL ADDON ------------------------------------------------
    CancelAddon::CancelAddon()
    {
    }
    //------------------------------------------ CANCEL UPGRADE ----------------------------------------------
    CancelUpgrade::CancelUpgrade()
    {
    }
    //------------------------------------------ CANCEL RESEARCH ---------------------------------------------
    CancelResearch::CancelResearch()
    {
    }
    //------------------------------------------ CANCEL NUKE -------------------------------------------------
    CancelNuke::CancelNuke()
    {
    }
    //------------------------------------------------ LIFTOFF -----------------------------------------------
    Lift::Lift()
      : unknown(0x00)
    {
    }
    //------------------------------------------------- LAND -------------------------------------------------
    Land::Land(BW::BuildPosition position, BW::UnitType type)
      : always0x47(0x47)
      , position(position)
      , type(type)
    {
    }
    //---------------------------------------------- BURROW --------------------------------------------------
    Burrow::Burrow()
      : unused(0x00)
    {
    }
    //----------------------------------------------- UNBURROW -----------------------------------------------
    Unburrow::Unburrow()
      : unused(0x00)
    {
    }
    //--------------------------------------------- HOLD POSITION --------------------------------------------
    HoldPosition::HoldPosition(u8 type)
      : type(type)
    {
    }
    //------------------------------------------------ UNLOAD UNIT -------------------------------------------
    UnloadUnit::UnloadUnit(BW::UnitID unload)
      : target(unload)
    {
    }
    //---------------------------------------------- UNLOAD ALL ----------------------------------------------
    UnloadAll::UnloadAll()
      : unused(0x00)
    {
    }
    //------------------------------------------------- SIEGE ------------------------------------------------
    Siege::Siege()
      : unused(0x00)
    {
    }
    //------------------------------------------------ UNSIEGE -----------------------------------------------
    Unsiege::Unsiege()
      : unused(0x00)
    {
    }
    //----------------------------------------- UNIT MORPH CONSTRUCTOR ---------------------------------------
    UnitMorph::UnitMorph(BW::UnitType type)
      : type(type)
    {
    }
    //------------------------------------------------ CLOAK -------------------------------------------------
    Cloak::Cloak()
      : unused(0x00)
    {
    }
    //------------------------------------------------ DECLOAK -----------------------------------------------
    Decloak::Decloak()
      : unused(0x00)
    {
    }
    //------------------------------------------ RETURN CARGO ------------------------------------------------
    ReturnCargo::ReturnCargo(u8 type)
      : type(type)
    {
    }
    //------------------------------------------------ STOP --------------------------------------------------
    Stop::Stop(u8 type)
      : type(type)
    {
    }
    //------------------------------------------ REAVER STOP -------------------------------------------------
    ReaverStop::ReaverStop()
    {
    }
    //------------------------------------------ CARRIER STOP ------------------------------------------------
    CarrierStop::CarrierStop()
    {
    }
    //------------------------------------------ CANCEL UNIT MORPH -------------------------------------------
    CancelUnitMorph::CancelUnitMorph()
    {
    }
    //------------------------------------------ CANCEL CONSTRUCTION -----------------------------------------
    CancelConstruction::CancelConstruction()
    {
    }
    //--------------------------------------------- CANCEL TRAIN ---------------------------------------------
    CancelTrain::CancelTrain(u8 slot)
      : slot(slot)
      , unknown(0)
    {
    }
    //------------------------------------------- CANCEL TRAIN LAST ------------------------------------------
    CancelTrainLast::CancelTrainLast()
      : always0xFE(0xFE)
      , unknown(0)
    {
    }
    //----------------------------------------------- SEND TEXT ----------------------------------------------
    SendText::SendText(u8 playerID, const char* msg)
      : playerID(playerID)
      , msg(msg)
    {
    }
    //------------------------------------------------ USE CHEAT ---------------------------------------------
    UseCheat::UseCheat(u32 flags)
      : flags(flags)
    {
    }
    //--------------------------------------------------------------------------------------------------------
  }
};
