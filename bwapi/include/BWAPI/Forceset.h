#pragma once
#include "SetContainer.h"

namespace BWAPI
{
  // Forward Declarations
  class ForceInterface;
  typedef ForceInterface *Force;
  class Playerset;

  /// <summary>A container that holds a group of Forces.</summary>
  ///
  /// @see BWAPI::Force
  class Forceset : public SetContainer<BWAPI::Force, std::hash<void*>>
  {
  public:

    /// @copydoc ForceInterface::getPlayers
    Playerset getPlayers() const;
  };
}

