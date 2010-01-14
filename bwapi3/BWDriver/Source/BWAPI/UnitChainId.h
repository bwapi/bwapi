#pragma once
/**
 *  In Broodwar memory, there are several chains of units.
 *  These are Ids we assign to each one. ChainId is used in
 *  UnitMirror to identify to which chain the unit slot corresponds
 */

namespace BWAPI
{
  namespace UnitChainIds
  {
    enum Enum
    {
      Unused = 0,
      Hidden,
      Visible,
      ScannerSweep
    };
  }
  typedef UnitChainIds::Enum UnitChainId;
}
