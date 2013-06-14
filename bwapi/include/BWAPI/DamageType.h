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
  /// Namespace containing damage types
  namespace DamageTypes
  {
    /// Retrieves the set of all the DamageTypes.
    ///
    /// @returns Set of DamageTypes.
    const DamageType::const_set& allDamageTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const DamageType x
    BWAPI_DECL(Independent);
    BWAPI_DECL(Explosive);
    BWAPI_DECL(Concussive);
    BWAPI_DECL(Normal);
    BWAPI_DECL(Ignore_Armor);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
