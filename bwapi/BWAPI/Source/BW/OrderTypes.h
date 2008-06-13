#pragma once

#include <Util/Types.h>

#include "UnitType.h"
#include "TechType.h"
#include "PositionUnitTarget.h"
#include "Race.h"

namespace BWAPI { class Unit; }
#pragma pack(1)
namespace BW
{
  /** Set of classes that can pass to IssueCommand function. */
  namespace Orders
  {
    /** Right click command in bw, can target either location or target. */
    class RightClick
    {
      public :
        /** Right-click on position. */
        RightClick(const BW::Position& target);
        /** Right-click on unit. */
        RightClick(BWAPI::Unit *target);
        /** Right-click on general target. */
        RightClick(const PositionUnitTarget& target);
      private :
        /** 0x14 = Rightclick command-code in bw */
        u8 always0x14;
        BW::PositionUnitTarget target;
        u8 always0xe4;
        u16 alwaysZero;
    };
   /** Selection command in bw, single unit version. */
   class SelectSingle
   {
     public :
      SelectSingle(BWAPI::Unit *select);
       /** 0x09 = Select command-code in bw */
       u8 always0x09; 
       /** 0x01 = 1 unit selection */
       u8 always0x01;
       /** The unit to select bw index */
       UnitTarget target;
   };
   /** Train unit command in bw. */
   class TrainUnit
   {
     public :
      TrainUnit(BW::UnitType type); 
      /** 0x1f = Train Command-code in bw */
      u8 always0x1f;
      /** Type of unit to train */
      BW::UnitType type;
   };
   /** Make building. */
   class MakeBuilding
   {
     public :
      MakeBuilding(BW::TilePosition position, BW::UnitType type); 
      /** 0x0c = make building Command-code in bw */
      u8 always0x0c;
      /** 1e for terran 1f for protoss*/
      u8 always0x1e;
      BW::TilePosition position;
      /** Type of building to make */
      BW::UnitType type;
   };
   /** Invent tech. */
   class Invent
   {
     public :
      Invent(BW::TechType type); 
      /** 0x30 = invent tech Command-code in bw */
      u8 always0x30;
      /** Type of tech to invent */
      BW::TechType tech;
   };   
   class MakeAddon
   {
     public :
      MakeAddon(BW::TilePosition position, BW::UnitType type); 
      /** 0x0c = make building Command-code in bw */
      u8 always0x0c;
      /** 1e for terran 1f for protoss*/
      u8 always0x24;
      BW::TilePosition position;
      /** Type of building to make */
      BW::UnitType type;
   };   
   /** Change slot command in bw. */
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
        /** 0x44 = Change slot command-code in bw */
        u8 always0x44;
        /** Order of the slot to change (0 for the 1st slot) */
        u8 slotID;
        /** Target slot state. */
        Slot slot;
    };
   /** Change race command in bw. */
   class ChangeRace
    {
      public :
        ChangeRace(BW::Race::Enum slot, u8 slotID);
      private :
        /** 0x41 = Command code for change race in bw. */
        u8 always0x41;
        /** Order of the slot to change (0 for the 1st slot). */
        u8 slotID;
        /** Target slot race. */
        BW::Race::Enum race;
    };
   /** Starts game in the pre-game lobby. */
   class StartGame
    {
      public :
        StartGame();
      private :
        /** 0x3c = Command code for start game. */
        u8 always0x3c;
    };
  }
}
#pragma pack()