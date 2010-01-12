#pragma once

/**
 *  In lobby, the slot states you can choose from
 */ 

namespace BW
{
  namespace SlotStateIDs
  {
    enum Enum : u8
    {
      Computer = 0,
      Open = 1,
      Closed = 2
    };
  }
  typedef SlotStateIDs::Enum SlotStateID;
}
