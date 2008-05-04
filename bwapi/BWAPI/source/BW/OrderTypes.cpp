#include "OrderTypes.h"
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