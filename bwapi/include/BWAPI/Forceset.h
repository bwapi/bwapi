#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
  // Forward Declarations
  class ForceInterface;
  typedef ForceInterface *Force;
  class Playerset;

  /// A container that holds a group of Forces.
  ///
  /// @see BWAPI::Force, BWAPI::Vectorset
  class Forceset : public Vectorset<BWAPI::Force>
  {
  public:
    Forceset(size_t initialSize = 5);
    Forceset(const Forceset &other);
    Forceset(Forceset &&other);

    /// @copydoc ForceInterface::getPlayers
    Playerset getPlayers() const;
  };
}

