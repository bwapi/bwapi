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
    ,x(target->getPosition().x)
    ,y(target->getPosition().y)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //-------------------------------- RIGHT CLICK COONSTRUCTOR -----------------------
    RightClick::RightClick(u16 x, u16 y)
    :always0x14(0x14)
    ,x(x)
    ,y(y)
    ,target(NULL)
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
    //---------------------------------------------------------------------------------
  }
}