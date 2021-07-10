#pragma once
#include "SetContainer.h"

namespace BWAPI
{
  // Forward Declarations
  class BulletInterface;
  typedef BulletInterface *Bullet;

  /// <summary>A container for a set of Bullet objects.</summary>
  class Bulletset : public SetContainer<Bullet, std::hash<void*>>
  {
  public:
  };
}

