#pragma once
#include "..\\Types.h"
#pragma pack(1)
namespace BW
{
  namespace Orders
  {
    class Move
    {
      public :
        Move(u16 x, u16 y);
      private :
        u8 always0x14;
        u16 x;
        u16 y;
        u16 alwaysZero;
        u8 always0xe4;
        u16 alwaysZero2;
    };
   class ChangeSlot
    {
      public :
       typedef enum Slot : u8
        {
          Computer = 0,
          Open = 1,
          Closed = 2
        };
        ChangeSlot(Slot slot, u8 slotID);
      private :
        u8 always0x44;
        u8 slotID;
        Slot slot;
    };
   class ChangeRace
    {
      public :
       typedef enum Race : u8
        {
          Zerg    = 0,
          Terran  = 1,
          Protoss = 2,
          Random  = 6
        };
        ChangeRace(Race slot, u8 slotID);
      private :
        u8 always0x41;
        u8 slotID;
        Race race;
    };
  }
}
#pragma pack()