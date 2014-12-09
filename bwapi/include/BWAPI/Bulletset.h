#pragma once
#include "SetContainer.h"

namespace BWAPI
{
  // Forward Declarations
  class BulletInterface;
  typedef BulletInterface *Bullet;

  class Bulletset : public SetContainer<Bullet, std::hash<void*>>
  {
  public:
  };
}

