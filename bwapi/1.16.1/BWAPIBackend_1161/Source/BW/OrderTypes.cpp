#include "OrderTypes.h"

#include <BWAPI/Order.h>
#include <BWAPI/Race.h>

namespace BW
{
  namespace Orders
  {
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(BWAPI4::Unit target, BWAPI::Order OrderID, bool queued)
        : target(target)
        , order(static_cast<u8>(OrderID))
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const BW::Position& target, BWAPI::Order OrderID, bool queued)
        : target(target)
        , order(static_cast<u8>(OrderID))
        , type(queued ? 1 : 0)
    {
    }
    Attack::Attack(int x, int y, BWAPI::Order OrderID, bool queued)
        : target(x,y)
        , order(static_cast<u8>(OrderID))
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- ATTACK LOCATION CONSTRUCTOR -----------------------------------
    Attack::Attack(const PositionUnitTarget& target, BWAPI::Order OrderID, bool queued)
        : target(target)
        , order(static_cast<u8>(OrderID))
        , type(queued ? 1 : 0)
    {
    }
    //--------------------------------------- RIGHT CLICK CONSTRUCTOR ---------------------------------------
    RightClick::RightClick(BWAPI4::Unit target, bool queued)
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
    SelectAdd::SelectAdd(int count, const BWAPI4::Unit *units)
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
    Select::Select(int count, const BWAPI4::Unit *units)
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
    Select::Select(const std::vector<BWAPI4::Unit>& units)
      : Select(static_cast<int>(units.size()), units.data())
    {
    }
    size_t Select::size() const
    {
      return 2 + targCount * 2;
    }
    //---------------------------------------- TRAIN UNIT CONSTRUCTOR ----------------------------------------
    TrainUnit::TrainUnit(BWAPI::UnitType type)
      : type(static_cast<u16>(type))
    {
    }
    //--------------------------------------------- MAKE BULDING ---------------------------------------------
    MakeBuilding::MakeBuilding(BW::TilePosition position, BWAPI::UnitType type)
        : position(position)
        , type(static_cast<u16>(type))
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
    MakeBuilding::MakeBuilding(int tileX, int tileY, BWAPI::UnitType type)
      : MakeBuilding(BW::TilePosition(static_cast<s16>(tileX), static_cast<s16>(tileY)), type)
    {
    }
    //---------------------------------------------- PLACE COP -----------------------------------------------
    PlaceCOP::PlaceCOP(BW::TilePosition position, BWAPI::UnitType type)
        : position(position)
        , type(static_cast<u16>(type))
    {
    }
    PlaceCOP::PlaceCOP(int x, int y, BWAPI::UnitType type)
      : PlaceCOP(BW::TilePosition(static_cast<s16>(x), static_cast<s16>(y)), type)
    {
    }
    //--------------------------------------------- INVENT TECH ----------------------------------------------
    Invent::Invent(int tech)
      : tech(static_cast<u8>(tech))
    {
    }
    //----------------------------------------------- UPGRADE ------------------------------------------------
    Upgrade::Upgrade(int upgrade)
      : upgrade(static_cast<u8>(upgrade))
    {
    }
    //---------------------------------------------- MAKE ADDON ----------------------------------------------
    MakeAddon::MakeAddon(BW::TilePosition position, BWAPI::UnitType type)
        : position(position)
        , type(static_cast<u16>(type))
    {
    }
    MakeAddon::MakeAddon(int tileX, int tileY, BWAPI::UnitType type)
      : MakeAddon(BW::TilePosition(static_cast<s16>(tileX), static_cast<s16>(tileY)), type)
    {
    }
    //---------------------------------------------- MAKE NYDUS ----------------------------------------------
    MakeNydusExit::MakeNydusExit(BW::TilePosition position)
        : position(position)
    {
    }
    MakeNydusExit::MakeNydusExit(int tileX, int tileY)
      : position(static_cast<s16>(tileX), static_cast<s16>(tileY))
    {
    }
    //------------------------------------------- MOVE CONSTRUCTOR -------------------------------------------
    ChangeSlot::ChangeSlot(int slotID, SlotType type)
      : slotID(static_cast<u8>(slotID))
      , slotType(static_cast<u8>(type))
    {
    }
    //--------------------------------------- CHANGE RACE CONSTRUCTOR ----------------------------------------
    RequestChangeRace::RequestChangeRace(int slot, int race)
      : slotID(static_cast<u8>(slot))
      , race(static_cast<u8>(race))
    {
    }
    UpdateSlot::UpdateSlot(int slot, int stormPlayerID, int owner, int newRace, int team)
      : bSlot(static_cast<u8>(slot))
      , bStormPlayerID(static_cast<u8>(stormPlayerID))
      , nType(static_cast<u8>(owner))
      , bNewRace(static_cast<u8>(newRace))
      , nTeam(static_cast<u8>(team))
    {
    }
    //------------------------------------------ RESUME CONSTRUCTOR ------------------------------------------
    LeaveGame::LeaveGame(int type)
      : type(static_cast<u8>(type))
    {
    }
    //-------------------------------------------- MINIMAP PING ----------------------------------------------
    MinimapPing::MinimapPing(BW::Position position)
        : position(position)
    {
    }
    MinimapPing::MinimapPing(int x, int y)
      : position(static_cast<s16>(x), static_cast<s16>(y))
    {
    }
    //---------------------------------------- BUILDING MORPH CONSTRUCTOR ------------------------------------
    BuildingMorph::BuildingMorph(BWAPI::UnitType type)
      : type(static_cast<u16>(type))
    {
    }
    //------------------------------------------------- LAND -------------------------------------------------
    Land::Land(BW::TilePosition position, BWAPI::UnitType type)
        : position(position)
        , type(static_cast<u16>(type))
    {
    }
    Land::Land(int x, int y, BWAPI::UnitType type)
      : Land(BW::TilePosition(static_cast<s16>(x), static_cast<s16>(y)), type)
    {
    }
    //--------------------------------------------- HOLD POSITION --------------------------------------------
    HoldPosition::HoldPosition(bool queued)
        : type(queued ? 1 : 0)
    {
    }
    //------------------------------------------------ UNLOAD UNIT -------------------------------------------
    UnloadUnit::UnloadUnit(BWAPI4::Unit unload)
        : target(unload)
    {
    }
    //----------------------------------------- UNIT MORPH CONSTRUCTOR ---------------------------------------
    UnitMorph::UnitMorph(BWAPI::UnitType type)
      : type(static_cast<u16>(type))
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
