#pragma once
#include "SetContainer.h"
#include <BWAPI/Race.h>

namespace BWAPI
{
  // Forward Declarations
  class PlayerImpl;
  typedef PlayerImpl *Player;
  class Unitset;

  /// <summary>A set containing Player objects.</summary>
  class Playerset : public SetContainer<BWAPI::Player, std::hash<void*>>
  {
  public:

  };
}

