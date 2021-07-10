#include <string>
#include <BWAPI/UnitCommandType.h>

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
    using namespace UnitCommandTypes;
    const UnitCommandType::set unitCommandTypeSet = { Attack_Move, Attack_Unit, Build, Build_Addon, Train, Morph, Research, Upgrade,
      Set_Rally_Position, Set_Rally_Unit, Move, Patrol, Hold_Position, Stop, Follow, Gather, Return_Cargo, Repair,
      Burrow, Unburrow, Cloak, Decloak, Siege, Unsiege, Lift, Land, Load, Unload, Unload_All, Unload_All_Position,
      Right_Click_Position, Right_Click_Unit, Halt_Construction, Cancel_Construction, Cancel_Addon,
      Cancel_Train, Cancel_Train_Slot, Cancel_Morph, Cancel_Research, Cancel_Upgrade, Use_Tech,
      Use_Tech_Position, Use_Tech_Unit, Place_COP, None, Unknown };
  }
  const UnitCommandType::set& UnitCommandTypes::allUnitCommandTypes()
  {
    return UnitCommandTypeSet::unitCommandTypeSet;
  }
}

