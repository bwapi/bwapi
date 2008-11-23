#pragma once

#include <Util/Types.h>

#include "UnitType.h"
#include "TechType.h"
#include "UpgradeType.h"
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
      /** Specifies race of builder: zerg = 0x19, terran = 1e, toss = 1f */
      u8 raceDependant;
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
   class Upgrade
   {
     public :
      Upgrade(BW::UpgradeType upgrade); 
      /** 0x32 = upgrade Command-code in bw */
      u8 always0x32;
      /** Type of upgrade */
      BW::UpgradeType upgrade;
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
   /** Pauses the game. */
   class PauseGame
    {
      public :
        PauseGame();
      private :
        /** 0x10 = Command code for pause game. */
        u8 always0x10;
    };
    class ResumeGame
    {
      public :
        ResumeGame();
      private :
        /** 0x11 = Command code for unpause game. */
        u8 always0x11;
    };

    class MergeDarkArchon
    {
      public :
        MergeDarkArchon();
      private :
        /** 0x5A = Command code for Merge Dark Archon. */
        u8 always0x5A;
    };

    class MergeArchon
    {
      public :
        MergeArchon();
      private :
        /** 0x2A = Command code for Merge Archon. */
        u8 always0x2A;
    };

    class MinimapPing
    {
      public :
        MinimapPing(BW::Position position);
      private :
        /** 0x58 = Command code for Minimap Ping. */
        u8 always0x58;
        BW::Position position;
    };

    class UseStimPack
    {
      public :
        UseStimPack();
      private :
        /** 0x36 = Command code for Stim */
        u8 always0x36;
    };

   class BuildingMorph
   {
     public :
      BuildingMorph(BW::UnitType type); 
      /** 0x35 = Building Morph for zerg */
      u8 always0x35;
      /** Type of unit to train */
      BW::UnitType type;
   };

    class CancelAddon
    {
      public :
        CancelAddon();
      private :
        u8 always0x34;
    };

    class CancelUpgrade
    {
      public :
        CancelUpgrade();
      private :
        u8 always0x33;
    };

    class CancelResearch
    {
      public :
        CancelResearch();
      private :
        u8 always0x31;
    };

    class CancelNuke
    {
      public :
        CancelNuke();
      private :
        u8 always0x2E;
    };

    class Lift
    {
      public :
        Lift();
      private :
        u8 always0x2F;
        u32 unknown;
    };

    class Burrow
    {
      public :
        Burrow();
      private :
        u8 always0x2C;
        u8 unused;
    };

    class Unburrow
    {
      public :
        Unburrow();
      private :
        u8 always0x2D;
        u8 unused;
    };

    class HoldPosition
    {
      public :
        HoldPosition(u8 type);
      private :
        u8 always0x2B;
        u8 type;
    };


  }
}
#pragma pack()