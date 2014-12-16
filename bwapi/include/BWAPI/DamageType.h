#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace DamageTypes
  {
    /// Enumeration of damage types
    namespace Enum
    {
      /// Enumeration of damage types
      enum Enum
      {
        Independent,
        Explosive,
        Concussive,
        Normal,
        Ignore_Armor,
        None,
        Unknown,
        MAX
      };
    }
  }
  class DamageType : public Type<DamageType, DamageTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      DamageType(int id = DamageTypes::Enum::None);
  };
  /// Namespace containing damage types.
  ///
  /// [View on Liquipedia](http://wiki.teamliquid.net/starcraft/Damage_Type)<br>
  /// [View on Starcraft Campendium (Official Website)](http://classic.battle.net/scc/gs/damage.shtml)<br>
  /// [View on Starcraft Wikia](http://starcraft.wikia.com/wiki/Damage_types)<br>
  namespace DamageTypes
  {
    /// Retrieves the set of all the DamageTypes.
    ///
    /// @returns Set of DamageTypes.
    const DamageType::set& allDamageTypes();

    extern const DamageType Independent;
    extern const DamageType Explosive;
    extern const DamageType Concussive;
    extern const DamageType Normal;
    extern const DamageType Ignore_Armor;
    extern const DamageType None;
    extern const DamageType Unknown;
  }

  static_assert(sizeof(DamageType) == sizeof(int), "Expected type to resolve to primitive size.");
}
