#pragma once
#include "SetContainer.h"
#include <BWAPI/Race.h>

namespace BWAPI
{
  // Forward Declarations
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class Unitset;

  /// <summary>A set containing Player objects.</summary>
  class Playerset : public SetContainer<BWAPI::Player, std::hash<void*>>
  {
  public:

  };
}

