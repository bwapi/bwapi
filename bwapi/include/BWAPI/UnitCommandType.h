#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace UnitCommandTypes
  {
    /// Enumeration of unit command types
    namespace Enum
    {
      /// Enumeration of unit command types
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
  class UnitCommandType : public Type<UnitCommandType, UnitCommandTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UnitCommandType(int id = UnitCommandTypes::Enum::None);
  };
  /// Namespace containing unit command types
  namespace UnitCommandTypes
  {
    /// Retrieves the set of all UnitCommandTypes.
    ///
    /// @returns Set of UnitCommandTypes.
    const UnitCommandType::const_set& allUnitCommandTypes();
    
#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const UnitCommandType x
    BWAPI_DECL(Attack_Move);
    BWAPI_DECL(Attack_Unit);
    BWAPI_DECL(Build);
    BWAPI_DECL(Build_Addon);
    BWAPI_DECL(Train);
    BWAPI_DECL(Morph);
    BWAPI_DECL(Research);
    BWAPI_DECL(Upgrade);
    BWAPI_DECL(Set_Rally_Position);
    BWAPI_DECL(Set_Rally_Unit);
    BWAPI_DECL(Move);
    BWAPI_DECL(Patrol);
    BWAPI_DECL(Hold_Position);
    BWAPI_DECL(Stop);
    BWAPI_DECL(Follow);
    BWAPI_DECL(Gather);
    BWAPI_DECL(Return_Cargo);
    BWAPI_DECL(Repair);
    BWAPI_DECL(Burrow);
    BWAPI_DECL(Unburrow);
    BWAPI_DECL(Cloak);
    BWAPI_DECL(Decloak);
    BWAPI_DECL(Siege);
    BWAPI_DECL(Unsiege);
    BWAPI_DECL(Lift);
    BWAPI_DECL(Land);
    BWAPI_DECL(Load);
    BWAPI_DECL(Unload);
    BWAPI_DECL(Unload_All);
    BWAPI_DECL(Unload_All_Position);
    BWAPI_DECL(Right_Click_Position);
    BWAPI_DECL(Right_Click_Unit);
    BWAPI_DECL(Halt_Construction);
    BWAPI_DECL(Cancel_Construction);
    BWAPI_DECL(Cancel_Addon);
    BWAPI_DECL(Cancel_Train);
    BWAPI_DECL(Cancel_Train_Slot);
    BWAPI_DECL(Cancel_Morph);
    BWAPI_DECL(Cancel_Research);
    BWAPI_DECL(Cancel_Upgrade);
    BWAPI_DECL(Use_Tech);
    BWAPI_DECL(Use_Tech_Position);
    BWAPI_DECL(Use_Tech_Unit);
    BWAPI_DECL(Place_COP);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
