#pragma once

#include "../Types.h"

#include <BWAPI/Position.h>

#include <BWAPI/UnitType.h>
#include <BWAPI/Order.h>

#include "PositionUnitTarget.h"
#include "Constants.h"

namespace BWAPI
{ 
  class UnitType;
}
#pragma pack(1)
namespace BW
{
  // Set of classes that can pass to IssueCommand function. 
  namespace Orders
  {
    template<int Cmd>
    struct BWCommand
    {
      u8 commandType = Cmd;
    };

    // Attack Location command in bw, can target either location or target. 
    class Attack : private BWCommand<0x15>
    {
    public:
      // Attack Location on position. 
      Attack(const BW::Position& target, BWAPI::Order order, bool queued = false);
      Attack(int x, int y, BWAPI::Order order, bool queued = false);
      // Attack Location on unit. 
      Attack(BWAPI4::Unit target, BWAPI::Order order, bool queued = false);
      // Attack Location on general target. 
      Attack(const PositionUnitTarget& target, BWAPI::Order order, bool queued = false);
    private:
      BW::PositionUnitTarget target;
      u16 always0xe4 = BWAPI::UnitTypes::Enum::None;
      u8 order;
      u8 type;
    };

    // Right click command in bw, can target either location or target. 
    class RightClick : private BWCommand<0x14>
    {
    public:
      // Right-click on position. 
      RightClick(const BW::Position& target, bool queued = false);
      RightClick(int x, int y, bool queued = false);
      // Right-click on unit. 
      RightClick(BWAPI4::Unit target, bool queued = false);
      // Right-click on general target. 
      RightClick(const PositionUnitTarget& target, bool queued = false);
    private:
      BW::PositionUnitTarget target;
      u16 always0xe4 = BWAPI::UnitTypes::Enum::None;
      u8  type;
    };

    // Selection addition in bw 
    class SelectAdd : private BWCommand<0x0A>
    {
    public:
      SelectAdd(int count, const BWAPI4::Unit *units);
      SelectAdd(int count, const BW::CUnit **units);

      size_t size() const;
    private:
      u8         targCount;
      UnitTarget targets[BW::MAX_SELECTION_COUNT];
    };

    // Selection command in bw 
    class Select : private BWCommand<0x09>
    {
    public:
      Select(int count, const BWAPI4::Unit *units);
      Select(int count, const BW::CUnit **units);
      Select(const std::vector<BWAPI4::Unit>& units);

      size_t size() const;
    private:
      u8         targCount;
      UnitTarget targets[BW::MAX_SELECTION_COUNT];
    };

    // Train unit command in bw. 
    class TrainUnit : private BWCommand<0x1F>
    {
    public:
      TrainUnit(BWAPI::UnitType type);
    private:
      // Type of unit to train 
      u16 type;
    };

    // Train unit command in bw. 
    class TrainFighter : private BWCommand<0x27>
    {
    };

    // Make building. 
    class MakeBuilding : private BWCommand<0x0c>
    {
    public:
      MakeBuilding(BW::TilePosition position, BWAPI::UnitType type);
      MakeBuilding(int tileX, int tileY, BWAPI::UnitType type);

    private:
      // Specifies race of builder: zerg = 0x19, terran = 1e, toss = 1f 
      u8 raceDependant = 0;
      BW::TilePosition position;
      // Type of building to make 
      u16 type;
    };

    // Place COP 
    class PlaceCOP : private BWCommand<0x0c>
    {
    public:
      PlaceCOP(BW::TilePosition position, BWAPI::UnitType type);
      PlaceCOP(int x, int y, BWAPI::UnitType type);
    
    private:
      u8 always0x9B = BWAPI::Orders::Enum::CTFCOP2;
      BW::TilePosition position;
      // Type of building to make 
      u16 type;
    };

    // Invent tech. 
    class Invent : private BWCommand<0x30>
    {
    public:
      Invent(int type);
    private:
      // Type of tech to invent 
      u8 tech;
    };

    class Upgrade : private BWCommand<0x32>
    {
    public:
      Upgrade(int upgrade);
    private:
      // Type of upgrade 
      u8 upgrade;
    };

    class MakeAddon : private BWCommand<0x0c>
    {
    public:
      MakeAddon(BW::TilePosition position, BWAPI::UnitType type);
      MakeAddon(int tileX, int tileY, BWAPI::UnitType type);
    private:
      // 1e for terran 1f for protoss
      u8 always0x24 = BWAPI::Orders::Enum::PlaceAddon;
      BW::TilePosition position;
      // Type of building to make 
      u16 type;
    };

    class MakeNydusExit : private BWCommand<0x0c>
    {
    public:
      MakeNydusExit(BW::TilePosition position);
      MakeNydusExit(int tileX, int tileY);
    private:
      // 1e for terran 1f for protoss
      u8 always0x2E = BWAPI::Orders::Enum::BuildNydusExit;
      BW::TilePosition position;
      // Type of building to make 
      u16 type = BWAPI::UnitTypes::Enum::Zerg_Nydus_Canal;
    };

    // Change slot command in bw. 
    class ChangeSlot : private BWCommand<0x44>
    {
    public:
      typedef enum __SlotType
      {
        Computer = 0,
        Open,
        Closed
      } SlotType;
      ChangeSlot(int slotID, SlotType type);
    private:
      // Order of the slot to change (0 for the 1st slot) 
      u8 slotID;
      // Target slot state. 
      u8 slotType;
    };

    // Change race command in bw. 
    class RequestChangeRace : private BWCommand<0x41>
    {
    public:
      RequestChangeRace(int slot, int race);
    private:
      // Order of the slot to change (0 for the 1st slot). 
      u8 slotID;
      // Target slot race. 
      u8 race;
    };

    // Lobby slot alteration (sent by host only). 
    class UpdateSlot : private BWCommand<0x3e>
    {
    public:
      UpdateSlot(int slot, int stormPlayerID, int owner, int newRace, int team);
    private:
      // Order of the slot to change (0 for the 1st slot). 
      u8 bSlot;
      // Storm ID of the player to map the slot to. 
      u8 bStormPlayerID;
      // Player type. 
      u8 nType;
      // Player's race. 
      u8 bNewRace;
      // Player's force. 
      u8 nTeam;
    };

    // Starts game in the pre-game lobby. 
    class StartGame : private BWCommand<0x3c>
    {
    };

    // Pauses the game. 
    class PauseGame : private BWCommand<0x10>
    {
    };

    class ResumeGame : private BWCommand<0x11>
    {
    };

    class LeaveGame : private BWCommand<0x57>
    {
    public:
      LeaveGame(int type);
    private:
      u8 type;
    };

    class MergeDarkArchon : private BWCommand<0x5a>
    {
    };

    class MergeArchon : private BWCommand<0x2a>
    {
    };

    class MinimapPing : private BWCommand<0x58>
    {
    public:
      MinimapPing(BW::Position position);
      MinimapPing(int x, int y);
    private:
      BW::Position position;
    };

    class UseStimPack : private BWCommand<0x36>
    {
    };

    class BuildingMorph : private BWCommand<0x35>
    {
    public:
      BuildingMorph(BWAPI::UnitType type);
    private:
      // Type of unit to train 
      u16 type;
    };

    class CancelAddon : private BWCommand<0x34>
    {
    };

    class CancelUpgrade : private BWCommand<0x33>
    {
    };

    class CancelResearch : private BWCommand<0x31>
    {
    };

    class CancelNuke : private BWCommand<0x2e>
    {
    };

    class Lift : private BWCommand<0x2f>
    {
    private:
      u32 unknown = 0;
    };

    class Land : private BWCommand<0x0c>
    {
    public:
      Land(BW::TilePosition position, BWAPI::UnitType type);
      Land(int x, int y, BWAPI::UnitType type);
    private:
      u8 always0x47 = BWAPI::Orders::Enum::BuildingLand;
      BW::TilePosition position;
      u16 type;
    };

    class Burrow : private BWCommand<0x2c>
    {
    private:
      u8 unused = 0;
    };

    class Unburrow : private BWCommand<0x2d>
    {
    private:
      u8 unused = 0;
    };

    class HoldPosition : private BWCommand<0x2b>
    {
    public:
      HoldPosition(bool queued = false);
    private:
      u8 type;
    };

    class UnloadUnit : private BWCommand<0x29>
    {
    public:
      UnloadUnit(BWAPI4::Unit unload);
    private:
      // The unit to unload bw index 
      UnitTarget target;
    };

    class UnloadAll : private BWCommand<0x28>
    {
    private:
      u8 unused = 0;
    };

    class Siege : private BWCommand<0x26>
    {
    private:
      u8 unused = 0;
    };

    class Unsiege : private BWCommand<0x25>
    {
    private:
      u8 unused = 0;
    };

    class UnitMorph : private BWCommand<0x23>
    {
    public:
      UnitMorph(BWAPI::UnitType type);
    private:
      // Type of unit to train 
      u16 type;
    };

    class Cloak : private BWCommand<0x21>
    {
    private:
      u8 unused = 0;
    };

    class Decloak : private BWCommand<0x22>
    {
    private:
      u8 unused = 0;
    };

    class ReturnCargo : private BWCommand<0x1e>
    {
    public:
      ReturnCargo(bool queued = false);
    private:
      u8 type;
    };

    class Stop : private BWCommand<0x1a>
    {
    public:
      Stop(bool queued = false);
    private:
      u8 type;
    };

    class ReaverStop : private BWCommand<0x1c>
    {
    };

    class CarrierStop : private BWCommand<0x1b>
    {
    };

    class CancelUnitMorph : private BWCommand<0x19>
    {
    };

    class CancelConstruction : private BWCommand<0x18>
    {
    };

    class CancelTrain : private BWCommand<0x20>
    {
    public:
      CancelTrain(int slot = -2);
    private:
      s8 slot;
      u8 unknown = 0;
    };

    class UseCheat : private BWCommand<0x12>
    {
    public:
      UseCheat(u32 flags);
    private:
      u32 flags;
    };

    class RestartGame : private BWCommand<0x08>
    {
    };

    class SetAllies : private BWCommand<0x0e>
    {
    public:
      SetAllies(u32 allies);
    private:
      u32 allies;
    };

    class SetVision : private BWCommand<0x0d>
    {
    public:
      SetVision(u16 vision);
    private:
      u16 vision;
    };
  } // namespace orders
}; // namespace BW
#pragma pack()
