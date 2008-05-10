#include "OrderTypes.h"
#include "..\\BWAPI\\Unit.h"
#include "Offsets.h"
namespace BW
{
  namespace Orders
  {

    //-------------------------------- RIGHT CLICK COONSTRUCTOR -----------------------
    RightClick::RightClick(BWAPI::Unit *target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //-------------------------------- RIGHT CLICK COONSTRUCTOR -----------------------
    RightClick::RightClick(const BW::Position& target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //-------------------------------- RIGHT CLICK COONSTRUCTOR -----------------------
    RightClick::RightClick(const PositionUnitTarget& target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //------------------------------- SELECT SINGLE CONSTRUCTOR -----------------------
    SelectSingle::SelectSingle(BWAPI::Unit *select)
    :always0x09(0x09)
    ,always0x01(0x01)
    ,target(select)
    {
    }

    //--------------------------------- TRAIN UNIT CONSTRUCTOR ------------------------
    TrainUnit::TrainUnit(BW::UnitType::Enum unitID)
    :always0x1f(0x1f)
    ,unitID(unitID)
    ,alwaysZero(0x0)
    {
    }
    //--------------------------------- MAKE BULDING ----------------------------------
    MakeBuilding::MakeBuilding(u16 x, u16 y, BW::UnitType::Enum unitID)
    :always0x0c(0x0c)
    ,always0x1e(0x1e)
    ,tileX(x)
    ,tileY(y)
    ,unitID(unitID)
    ,alwaysZero(0x0)
    {
    }
    //---------------------------------- MOVE CONSTRUCTOR -----------------------------
    ChangeSlot::ChangeSlot(Slot slot, u8 slotID)
    :slot(slot)
    ,slotID(slotID)
    ,always0x44(0x44)
    {
    }
    //---------------------------------- MOVE CONSTRUCTOR -----------------------------
    ChangeRace::ChangeRace(Race race, u8 slotID)
    :race(race)
    ,slotID(slotID)
    ,always0x41(0x41)
    {
    }
    //------------------------------- START GAME CONSTRUCTOR --------------------------
    StartGame::StartGame()
    :always0x3c(0x3c)
    {
    }
    //---------------------------------------------------------------------------------
  }
}