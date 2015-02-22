#include "OrderTypes.h"
#include <BWAPI/Unit.h>
#include <BWAPI/Unitset.h>

#include <BWAPI/Order.h>
#include <BWAPI/Race.h>
#include <BW/Offsets.h>

#include "../../../Debug.h"
namespace BW
{
  namespace Orders
  {
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(BWAPI::Unit target, int OrderID, bool queued)
        : target(target)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const BW::Position& target, int OrderID, bool queued)
        : target(target)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    Attack::Attack(int x, int y, int OrderID, bool queued)
        : target(x,y)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const PositionUnitTarget& target, int OrderID, bool queued)
        : target(target)
        , order((u8)OrderID)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(BWAPI::Unit target, bool queued)
        : target(target)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const BW::Position& target, bool queued)
        : target(target)
        , type(queued ? 1 : 0)
    {
    }
    RightClick::RightClick(int x, int y, bool queued)
        : target(x,y)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(const PositionUnitTarget& target, bool queued)
        : target(target)
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- SHIFT SELECT CONSTRUCTOR ---------------------------------------
    SelectAdd::SelectAdd(int count, const BWAPI::Unit *units)
    {
      targCount = 0;
      for (int i = 0; i < count && i < MAX_SELECTION_COUNT; ++i)
      {
        if (units[i])
          targets[targCount++] = UnitTarget(units[i]);
      }
    }
    SelectAdd::SelectAdd(int count, const BW::CUnit **units)
    {
      targCount = 0;
      for (int i = 0; i < count && i < MAX_SELECTION_COUNT; ++i)
      {
        if (units[i])
          targets[targCount++] = UnitTarget(units[i]);
      }
    }
    size_t SelectAdd::size() const
    {
      return 2 + targCount * 2;
    }
    //----------------------------------------- SELECT CONSTRUCTOR -------------------------------------------
    Select::Select(int count, const BWAPI::Unit *units)
    {
      targCount = 0;
      for (int i = 0; i < count && i < MAX_SELECTION_COUNT; ++i)
      {
        if (units[i])
          targets[targCount++] = UnitTarget(units[i]);
      }
    }
    Select::Select(int count, const BW::CUnit **units)
    {
      targCount = 0;
      for (int i = 0; i < count && i < MAX_SELECTION_COUNT; ++i)
      {
        if (units[i])
          targets[targCount++] = UnitTarget(units[i]);
      }
    }
    Select::Select(const std::vector<BWAPI::Unit>& units)
      : Select(static_cast<int>(units.size()), units.data())
    {
    }
    size_t Select::size() const
    {
      return 2 + targCount * 2;
    }
    //---------------------------------------- TRAIN UNIT CONSTRUCTOR ----------------------------------------
    TrainUnit::TrainUnit(int type)
        : type((u16)type)
    {
    }
    //--------------------------------------------- MAKE BULDING ---------------------------------------------
    MakeBuilding::MakeBuilding(BW::TilePosition position, int type)
        : position(position)
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
        break;
      }
    }
    MakeBuilding::MakeBuilding(int tileX, int tileY, int type)
      : MakeBuilding(BW::TilePosition((u16)tileX, (u16)tileY), type)
    {
    }
    //---------------------------------------------- PLACE COP -----------------------------------------------
    PlaceCOP::PlaceCOP(BW::TilePosition position, int type)
        : position(position)
        , type((u16)type)
    {
    }
    PlaceCOP::PlaceCOP(int x, int y, int type)
        : position((u16)x, (u16)y)
        , type((u16)type)
    {
    }
    //--------------------------------------------- INVENT TECH ----------------------------------------------
    Invent::Invent(int tech)
        : tech((u8)tech)
    {
    }
    //----------------------------------------------- UPGRADE ------------------------------------------------
    Upgrade::Upgrade(int upgrade)
        : upgrade((u8)upgrade)
    {
    }
    //---------------------------------------------- MAKE ADDON ----------------------------------------------
    MakeAddon::MakeAddon(BW::TilePosition position, int type)
        : position(position)
        , type((u16)type)
    {
    }
    MakeAddon::MakeAddon(int tileX, int tileY, int type)
        : position((u16)tileX, (u16)tileY)
        , type((u16)type)
    {
    }
    //---------------------------------------------- MAKE NYDUS ----------------------------------------------
    MakeNydusExit::MakeNydusExit(BW::TilePosition position)
        : position(position)
    {
    }
    MakeNydusExit::MakeNydusExit(int tileX, int tileY)
        : position((u16)tileX, (u16)tileY)
    {
    }
    //------------------------------------------- MOVE CONSTRUCTOR -------------------------------------------
    ChangeSlot::ChangeSlot(int slotID, SlotType type)
    : slotID((u8)slotID)
    , slotType((u8)type)
    {
    }
    //--------------------------------------- CHANGE RACE CONSTRUCTOR ----------------------------------------
    RequestChangeRace::RequestChangeRace(int slot, int race)
    : slotID((u8)slot)
    , race((u8)race)
    {
    }
    UpdateSlot::UpdateSlot(int slot, int stormPlayerID, int owner, int newRace, int team)
      : bSlot((u8)slot)
      , bStormPlayerID((u8)stormPlayerID)
      , nType((u8)owner)
      , bNewRace((u8)newRace)
      , nTeam((u8)team)
    {
    }
    //------------------------------------------ RESUME CONSTRUCTOR ------------------------------------------
    LeaveGame::LeaveGame(int type)
        : type((u8)type)
    {
    }
    //-------------------------------------------- MINIMAP PING ----------------------------------------------
    MinimapPing::MinimapPing(BW::Position position)
        : position(position)
    {
    }
    MinimapPing::MinimapPing(int x, int y)
        : position((u16)x, (u16)y)
    {
    }
    //---------------------------------------- BUILDING MORPH CONSTRUCTOR ------------------------------------
    BuildingMorph::BuildingMorph(int type)
        : type((u16)type)
    {
    }
    //------------------------------------------------- LAND -------------------------------------------------
    Land::Land(BW::TilePosition position, int type)
        : position(position)
        , type((u16)type)
    {
    }
    Land::Land(int x, int y, int type)
        : position((u16)x, (u16)y)
        , type((u16)type)
    {
    }
    //--------------------------------------------- HOLD POSITION --------------------------------------------
    HoldPosition::HoldPosition(bool queued)
        : type(queued ? 1 : 0)
    {
    }
    //------------------------------------------------ UNLOAD UNIT -------------------------------------------
    UnloadUnit::UnloadUnit(BWAPI::Unit unload)
        : target(unload)
    {
    }
    //----------------------------------------- UNIT MORPH CONSTRUCTOR ---------------------------------------
    UnitMorph::UnitMorph(int type)
        : type((u16)type)
    {
    }
    //------------------------------------------ RETURN CARGO ------------------------------------------------
    ReturnCargo::ReturnCargo(bool queued)
        : type(queued ? 1 : 0)
    {
    }
    //------------------------------------------------ STOP --------------------------------------------------
    Stop::Stop(bool queued)
        : type(queued ? 1 : 0)
    {
    }
    //--------------------------------------------- CANCEL TRAIN ---------------------------------------------
    CancelTrain::CancelTrain(int slot)
        : slot((s8)slot)
    {
    }
    //------------------------------------------------ USE CHEAT ---------------------------------------------
    UseCheat::UseCheat(u32 flags)
      : flags(flags)
    {
    }
    //------------------------------------------------ SET ALLIES --------------------------------------------
    SetAllies::SetAllies(u32 allies)
      : allies(allies)
    {
    }
    //------------------------------------------------ SET VISION --------------------------------------------
    SetVision::SetVision(u16 vision)
      : vision(vision)
    {
    }

  }
};
