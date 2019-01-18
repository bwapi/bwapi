#pragma once
#include <BWAPI/Bullet.h>
#include <BWAPI/SetContainer.h>

namespace BWAPI
{
  // Forward Declarations

  /// <summary>A container for a set of Bullet objects.</summary>
  class Bulletset : public SetContainer<BWAPI::Bullet, IDCompare>
  {
  public:
  };
}

