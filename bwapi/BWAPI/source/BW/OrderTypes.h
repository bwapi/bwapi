#pragma once
#include "..\\Types.h"
#include "UnitTypes.h"
#include "UnitTarget.h"
namespace BWAPI { class Unit; }
//namespace BW { namespace UnitType { enum Enum; }  }
#pragma pack(1)
namespace BW
{
  namespace Orders
  {
    class RightClick
    {
      public :
        RightClick(u16 x, u16 y);
        RightClick(BWAPI::Unit *target);
      private :
        u8 always0x14;
        u16 x;
        u16 y;
        UnitTarget target;
        u8 always0xe4;
        u16 alwaysZero;
    };
   class SelectSingle
   {
     public :
      SelectSingle(BWAPI::Unit *select);
       u8 always0x09;
       u8 always0x01;
       UnitTarget target;
   };
   class TrainUnit
   {
     public :
      TrainUnit(BW::UnitType::Enum type); 
      u8 always0x1f;
      u8 unitID;
      u8 alwaysZero;
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