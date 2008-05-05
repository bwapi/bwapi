#include "OrderTypes.h"
#include "..\\BWAPI\\Unit.h"
#include "Offsets.h"
namespace BW
{
  namespace Orders
  {
    //---------------------------------- MOVE CONSTRUCTOR -----------------------------
    Move::Move(u16 x, u16 y)
    :x(x)
    ,y(y)
    ,always0x14(0x14)
    ,alwaysZero(0x0)
    ,always0xe4(0xe4)
    ,alwaysZero2(0x0)
    {
    }
    //---------------------------------- MOVE CONSTRUCTOR -----------------------------
    MoveTarget::MoveTarget(BWAPI::Unit *target)
    :x(target->getPosition().x)
    ,y(target->getPosition().y)
    ,always0x14(0x14)
    ,alwaysZero(0x0)
    ,always0xe4(0xe4)
    ,unitOrder((int)((int)target->getOriginalRawData() - (int)BW::UnitNodeTable)/336 | 1 << 11)
    {
    }
    //------------------------------------- TRAIN UNIT --------------------------------
    SelectSingle::SelectSingle(BWAPI::Unit *select)
    :always0x09(0x09)
    ,always0x01(0x01)
    ,unitID((int)((int)select->getOriginalRawData() - (int)BW::UnitNodeTable)/336 | 1 << 11)
    {
    }
    //------------------------------------- TRAIN UNIT --------------------------------
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