#pragma once
#include "SetContainer.h"

namespace BWAPI
{
  // Forward Declarations
  class BulletImpl;

  /// <summary>A container for a set of Bullet objects.</summary>
  class Bulletset : public SetContainer<BulletImpl*, std::hash<void*>>
  {
  public:
  };
}

