#pragma once
#include <BWAPI/Force.h>
#include <BWAPI/SetContainer.h>

namespace BWAPI
{
  // Forward Declarations
  class Playerset;

  /// <summary>A container that holds a group of Forces.</summary>
  ///
  /// @see BWAPI::Force
  class Forceset : public SetContainer<BWAPI::Force, IDCompare>
  {
  public:
    /// @copydoc Force::getPlayers
    Playerset getPlayers() const;
  };
}

