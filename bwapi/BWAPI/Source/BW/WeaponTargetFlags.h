#pragma once

namespace BW
{
  /** Weapon target flags */
  namespace WeaponTargetFlags
  {
    enum Enum : u16
    {
      Air          = 1 << 0,
      Ground       = 1 << 1,
      Mechanical   = 1 << 2,
      Organic      = 1 << 3,
      NonBuilding  = 1 << 4,
      NonRobotic   = 1 << 5,
      Terrain      = 1 << 6,
      OrgOrMech    = 1 << 7,
      Own          = 1 << 8,
    };
  };
};

