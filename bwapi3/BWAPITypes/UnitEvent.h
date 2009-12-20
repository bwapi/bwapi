#pragma once
/**
 *  AddEvent and RemoveEvent parameters
 */

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
    int unitId;
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
    int unitId;
    UnitRemoveEventTypeId type;
  };
}
