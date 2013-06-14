#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
  // Forward Declarations
  class BulletInterface;
  typedef BulletInterface *Bullet;

  class Bulletset : public Vectorset<BWAPI::Bullet>
  {
  public:
    Bulletset(size_t initialSize = 16);
    Bulletset(const Bulletset &other);
    Bulletset(Bulletset &&other);
  };
}

