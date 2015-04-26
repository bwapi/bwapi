#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing unit command types.</summary>
  /// @see UnitCommandType
  namespace UnitCommandTypes
  {
    /// <summary>Enumeration of valid unit command types.</summary>
    /// @see UnitCommandType
    namespace Enum
    {
      /// <summary>Enumeration of valid unit command types.</summary>
      /// @see UnitCommandType
      enum Enum
      {
        Attack_Move = 0,
        Attack_Unit,
        Build,
        Build_Addon,
        Train,
        Morph,
        Research,
        Upgrade,
        Set_Rally_Position,
        Set_Rally_Unit,
        Move,
        Patrol,
        Hold_Position,
        Stop,
        Follow,
        Gather,
        Return_Cargo,
        Repair,
        Burrow,
        Unburrow,
        Cloak,
        Decloak,
        Siege,
        Unsiege,
        Lift,
        Land,
        Load,
        Unload,
        Unload_All,
        Unload_All_Position,
        Right_Click_Position,
        Right_Click_Unit,
        Halt_Construction,
        Cancel_Construction,
        Cancel_Addon,
        Cancel_Train,
        Cancel_Train_Slot,
        Cancel_Morph,
        Cancel_Research,
        Cancel_Upgrade,
        Use_Tech,
        Use_Tech_Position,
        Use_Tech_Unit,
        Place_COP,
        None,
        Unknown,
        MAX
      };
    };
  };
  /// <summary>A representation of a unit command in BWAPI.</summary> This is used by bots to
  /// notify BWAPI which commands to use. BWAPI filters commands accordingly and then converts
  /// them to Broodwar commands, which differ in complexity.
  ///
  /// @see UnitCommandTypes
  /// @ingroup TypeClasses
  class UnitCommandType : public Type<UnitCommandType, UnitCommandTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UnitCommandType(int id = UnitCommandTypes::Enum::None);
  };

  /// @ingroup Types
  namespace UnitCommandTypes
  {
    /// <summary>Retrieves the set of all valid UnitCommandTypes.</summary>
    ///
    /// @returns Set of UnitCommandTypes.
    const UnitCommandType::set& allUnitCommandTypes();
    
    extern const UnitCommandType Attack_Move;
    extern const UnitCommandType Attack_Unit;
    extern const UnitCommandType Build;
    extern const UnitCommandType Build_Addon;
    extern const UnitCommandType Train;
    extern const UnitCommandType Morph;
    extern const UnitCommandType Research;
    extern const UnitCommandType Upgrade;
    extern const UnitCommandType Set_Rally_Position;
    extern const UnitCommandType Set_Rally_Unit;
    extern const UnitCommandType Move;
    extern const UnitCommandType Patrol;
    extern const UnitCommandType Hold_Position;
    extern const UnitCommandType Stop;
    extern const UnitCommandType Follow;
    extern const UnitCommandType Gather;
    extern const UnitCommandType Return_Cargo;
    extern const UnitCommandType Repair;
    extern const UnitCommandType Burrow;
    extern const UnitCommandType Unburrow;
    extern const UnitCommandType Cloak;
    extern const UnitCommandType Decloak;
    extern const UnitCommandType Siege;
    extern const UnitCommandType Unsiege;
    extern const UnitCommandType Lift;
    extern const UnitCommandType Land;
    extern const UnitCommandType Load;
    extern const UnitCommandType Unload;
    extern const UnitCommandType Unload_All;
    extern const UnitCommandType Unload_All_Position;
    extern const UnitCommandType Right_Click_Position;
    extern const UnitCommandType Right_Click_Unit;
    extern const UnitCommandType Halt_Construction;
    extern const UnitCommandType Cancel_Construction;
    extern const UnitCommandType Cancel_Addon;
    extern const UnitCommandType Cancel_Train;
    extern const UnitCommandType Cancel_Train_Slot;
    extern const UnitCommandType Cancel_Morph;
    extern const UnitCommandType Cancel_Research;
    extern const UnitCommandType Cancel_Upgrade;
    extern const UnitCommandType Use_Tech;
    extern const UnitCommandType Use_Tech_Position;
    extern const UnitCommandType Use_Tech_Unit;
    extern const UnitCommandType Place_COP;
    extern const UnitCommandType None;
    extern const UnitCommandType Unknown;
  }

  static_assert(sizeof(UnitCommandType) == sizeof(int), "Expected type to resolve to primitive size.");
}
