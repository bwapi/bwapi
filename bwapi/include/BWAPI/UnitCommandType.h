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
    }
  }
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
    constexpr explicit UnitCommandType(int id = UnitCommandTypes::Enum::None) : Type(id) {}
  };

  /// @ingroup Types
  namespace UnitCommandTypes
  {
    /// <summary>Retrieves the set of all valid UnitCommandTypes.</summary>
    ///
    /// @returns Set of UnitCommandTypes.
    const UnitCommandType::set& allUnitCommandTypes();
    
    constexpr UnitCommandType Attack_Move{Enum::Attack_Move};
    constexpr UnitCommandType Attack_Unit{Enum::Attack_Unit};
    constexpr UnitCommandType Build{Enum::Build};
    constexpr UnitCommandType Build_Addon{Enum::Build_Addon};
    constexpr UnitCommandType Train{Enum::Train};
    constexpr UnitCommandType Morph{Enum::Morph};
    constexpr UnitCommandType Research{Enum::Research};
    constexpr UnitCommandType Upgrade{Enum::Upgrade};
    constexpr UnitCommandType Set_Rally_Position{Enum::Set_Rally_Position};
    constexpr UnitCommandType Set_Rally_Unit{Enum::Set_Rally_Unit};
    constexpr UnitCommandType Move{Enum::Move};
    constexpr UnitCommandType Patrol{Enum::Patrol};
    constexpr UnitCommandType Hold_Position{Enum::Hold_Position};
    constexpr UnitCommandType Stop{Enum::Stop};
    constexpr UnitCommandType Follow{Enum::Follow};
    constexpr UnitCommandType Gather{Enum::Gather};
    constexpr UnitCommandType Return_Cargo{Enum::Return_Cargo};
    constexpr UnitCommandType Repair{Enum::Repair};
    constexpr UnitCommandType Burrow{Enum::Burrow};
    constexpr UnitCommandType Unburrow{Enum::Unburrow};
    constexpr UnitCommandType Cloak{Enum::Cloak};
    constexpr UnitCommandType Decloak{Enum::Decloak};
    constexpr UnitCommandType Siege{Enum::Siege};
    constexpr UnitCommandType Unsiege{Enum::Unsiege};
    constexpr UnitCommandType Lift{Enum::Lift};
    constexpr UnitCommandType Land{Enum::Land};
    constexpr UnitCommandType Load{Enum::Load};
    constexpr UnitCommandType Unload{Enum::Unload};
    constexpr UnitCommandType Unload_All{Enum::Unload_All};
    constexpr UnitCommandType Unload_All_Position{Enum::Unload_All_Position};
    constexpr UnitCommandType Right_Click_Position{Enum::Right_Click_Position};
    constexpr UnitCommandType Right_Click_Unit{Enum::Right_Click_Unit};
    constexpr UnitCommandType Halt_Construction{Enum::Halt_Construction};
    constexpr UnitCommandType Cancel_Construction{Enum::Cancel_Construction};
    constexpr UnitCommandType Cancel_Addon{Enum::Cancel_Addon};
    constexpr UnitCommandType Cancel_Train{Enum::Cancel_Train};
    constexpr UnitCommandType Cancel_Train_Slot{Enum::Cancel_Train_Slot};
    constexpr UnitCommandType Cancel_Morph{Enum::Cancel_Morph};
    constexpr UnitCommandType Cancel_Research{Enum::Cancel_Research};
    constexpr UnitCommandType Cancel_Upgrade{Enum::Cancel_Upgrade};
    constexpr UnitCommandType Use_Tech{Enum::Use_Tech};
    constexpr UnitCommandType Use_Tech_Position{Enum::Use_Tech_Position};
    constexpr UnitCommandType Use_Tech_Unit{Enum::Use_Tech_Unit};
    constexpr UnitCommandType Place_COP{Enum::Place_COP};
    constexpr UnitCommandType None{Enum::None};
    constexpr UnitCommandType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(UnitCommandType) == sizeof(int), "Expected type to resolve to primitive size.");
} // namespace BWAPI

namespace std {
  template<>
  struct hash<BWAPI::UnitCommandType> {
    auto operator()(BWAPI::UnitCommandType id) const {
      return BWAPI::UnitCommandType::Hash{}(id);
    }
  };
} // namespace std
