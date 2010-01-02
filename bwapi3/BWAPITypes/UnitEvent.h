#pragma once
/**
 *  AddEvent and RemoveEvent parameters
 */

#include "UnitId.h"

namespace BWAPI
{
  namespace UnitAddEventTypeIds
  {
    enum Enum
    {
      Created,
      Unfogged,
      Noticed,
      Unloaded
    };
  }
  typedef UnitAddEventTypeIds::Enum UnitAddEventTypeId;
  struct UnitAddEvent
  {
    UnitId unitId;
    UnitAddEventTypeId type;
  };

  namespace UnitRemoveEventTypeIds
  {
    enum Enum
    {
      Died,
      Fogged,
      Loaded
    };
  }
  typedef UnitRemoveEventTypeIds::Enum UnitRemoveEventTypeId;
  struct UnitRemoveEvent
  {
    UnitId unitId;
    UnitRemoveEventTypeId type;
  };
}
