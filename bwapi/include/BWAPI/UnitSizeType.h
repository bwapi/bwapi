#pragma once
#include <BWAPI/Type.h>

#define BWAPI_UNITSIZETYPEDEF(x) static const UnitSizeType x(Enum::x) /** ref x */

namespace BWAPI
{
  namespace UnitSizeTypes
  {
    /// Enumeration of unit size types
    namespace Enum
    {
      /// Enumeration of unit size types
      enum Enum
      {
        Independent = 0,
        Small,
        Medium,
        Large,
        None,
        Unknown,
        MAX
      };
    };
  };
  class UnitSizeType : public Type<UnitSizeType, UnitSizeTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UnitSizeType(int id = UnitSizeTypes::Enum::None);
  };
  /// Namespace containing unit size types
  namespace UnitSizeTypes
  {
    /// Retrieves the set of all UnitSizeTypes.
    ///
    /// @returns Set of all UnitSizeTypes.
    const UnitSizeType::const_set& allUnitSizeTypes();
    
#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const UnitSizeType x
    BWAPI_DECL(Independent);
    BWAPI_DECL(Small);
    BWAPI_DECL(Medium);
    BWAPI_DECL(Large);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
