#include <string>
#include <BWAPI/UnitCommandType.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  template <>
  const std::string Type<UnitCommandType, UnitCommandTypes::Enum::Unknown>::typeNames[UnitCommandTypes::Enum::MAX] =
  {
    "Attack_Move",
    "Attack_Unit",
    "Build",
    "Build_Addon",
    "Train",
    "Morph",
    "Research",
    "Upgrade",
    "Set_Rally_Position",
    "Set_Rally_Unit",
    "Move",
    "Patrol",
    "Hold_Position",
    "Stop",
    "Follow",
    "Gather",
    "Return_Cargo",
    "Repair",
    "Burrow",
    "Unburrow",
    "Cloak",
    "Decloak",
    "Siege",
    "Unsiege",
    "Lift",
    "Land",
    "Load",
    "Unload",
    "Unload_All",
    "Unload_All_Position",
    "Right_Click_Position",
    "Right_Click_Unit",
    "Halt_Construction",
    "Cancel_Construction",
    "Cancel_Addon",
    "Cancel_Train",
    "Cancel_Train_Slot",
    "Cancel_Morph",
    "Cancel_Research",
    "Cancel_Upgrade",
    "Use_Tech",
    "Use_Tech_Position",
    "Use_Tech_Unit",
    "Place_COP",
    "None",
    "Unknown"
  };
  namespace UnitCommandTypeSet
  {
    using namespace UnitCommandTypes::Enum;
    const UnitCommandType::set unitCommandTypeSet = { Attack_Move, Attack_Unit, Build, Build_Addon, Train, Morph, Research, Upgrade,
      Set_Rally_Position, Set_Rally_Unit, Move, Patrol, Hold_Position, Stop, Follow, Gather, Return_Cargo, Repair,
      Burrow, Unburrow, Cloak, Decloak, Siege, Unsiege, Lift, Land, Load, Unload, Unload_All, Unload_All_Position,
      Right_Click_Position, Right_Click_Unit, Halt_Construction, Cancel_Construction, Cancel_Addon,
      Cancel_Train, Cancel_Train_Slot, Cancel_Morph, Cancel_Research, Cancel_Upgrade, Use_Tech,
      Use_Tech_Position, Use_Tech_Unit, Place_COP, None, Unknown };
  }
  namespace UnitCommandTypes
  {
    BWAPI_TYPEDEF(UnitCommandType,Attack_Move);
    BWAPI_TYPEDEF(UnitCommandType,Attack_Unit);
    BWAPI_TYPEDEF(UnitCommandType,Build);
    BWAPI_TYPEDEF(UnitCommandType,Build_Addon);
    BWAPI_TYPEDEF(UnitCommandType,Train);
    BWAPI_TYPEDEF(UnitCommandType,Morph);
    BWAPI_TYPEDEF(UnitCommandType,Research);
    BWAPI_TYPEDEF(UnitCommandType,Upgrade);
    BWAPI_TYPEDEF(UnitCommandType,Set_Rally_Position);
    BWAPI_TYPEDEF(UnitCommandType,Set_Rally_Unit);
    BWAPI_TYPEDEF(UnitCommandType,Move);
    BWAPI_TYPEDEF(UnitCommandType,Patrol);
    BWAPI_TYPEDEF(UnitCommandType,Hold_Position);
    BWAPI_TYPEDEF(UnitCommandType,Stop);
    BWAPI_TYPEDEF(UnitCommandType,Follow);
    BWAPI_TYPEDEF(UnitCommandType,Gather);
    BWAPI_TYPEDEF(UnitCommandType,Return_Cargo);
    BWAPI_TYPEDEF(UnitCommandType,Repair);
    BWAPI_TYPEDEF(UnitCommandType,Burrow);
    BWAPI_TYPEDEF(UnitCommandType,Unburrow);
    BWAPI_TYPEDEF(UnitCommandType,Cloak);
    BWAPI_TYPEDEF(UnitCommandType,Decloak);
    BWAPI_TYPEDEF(UnitCommandType,Siege);
    BWAPI_TYPEDEF(UnitCommandType,Unsiege);
    BWAPI_TYPEDEF(UnitCommandType,Lift);
    BWAPI_TYPEDEF(UnitCommandType,Land);
    BWAPI_TYPEDEF(UnitCommandType,Load);
    BWAPI_TYPEDEF(UnitCommandType,Unload);
    BWAPI_TYPEDEF(UnitCommandType,Unload_All);
    BWAPI_TYPEDEF(UnitCommandType,Unload_All_Position);
    BWAPI_TYPEDEF(UnitCommandType,Right_Click_Position);
    BWAPI_TYPEDEF(UnitCommandType,Right_Click_Unit);
    BWAPI_TYPEDEF(UnitCommandType,Halt_Construction);
    BWAPI_TYPEDEF(UnitCommandType,Cancel_Construction);
    BWAPI_TYPEDEF(UnitCommandType,Cancel_Addon);
    BWAPI_TYPEDEF(UnitCommandType,Cancel_Train);
    BWAPI_TYPEDEF(UnitCommandType,Cancel_Train_Slot);
    BWAPI_TYPEDEF(UnitCommandType,Cancel_Morph);
    BWAPI_TYPEDEF(UnitCommandType,Cancel_Research);
    BWAPI_TYPEDEF(UnitCommandType,Cancel_Upgrade);
    BWAPI_TYPEDEF(UnitCommandType,Use_Tech);
    BWAPI_TYPEDEF(UnitCommandType,Use_Tech_Position);
    BWAPI_TYPEDEF(UnitCommandType,Use_Tech_Unit);
    BWAPI_TYPEDEF(UnitCommandType,Place_COP);
    BWAPI_TYPEDEF(UnitCommandType,None);
    BWAPI_TYPEDEF(UnitCommandType,Unknown);
  }
  UnitCommandType::UnitCommandType(int id) : Type( id )
  {}
  const UnitCommandType::set& UnitCommandTypes::allUnitCommandTypes()
  {
    return UnitCommandTypeSet::unitCommandTypeSet;
  }
}

