#pragma once
#include "SetContainer.h"

namespace BWAPI
{
  // Forward Declarations

  /// <summary>A container for a set of Bullet objects.</summary>
  class Bulletset : public SetContainer<Bullet, Bullet::Hash>
  {
  public:
  };
}

