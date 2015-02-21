#pragma once
#include <list>

#include <BWAPI/UnitCommandType.h>
#include <BWAPI/UnitCommand.h>

namespace BWAPI
{
  /// Used for optimizing several commands of the same type, usually by grouping units together.
  class CommandOptimizer
  {
  public:
    /// Adds a command to the optimizer and performs pre-processing.
    /// Attempts to generalize commands of different types and reduce command byte size.
    bool add(UnitCommand command);

    /// Clears and discards all queued commands.
    void clear();

    /// Flushes commands from the optimizer and executes them. Groups commands together before execution.
    void flush();

    /// Initializes the optimizer. Used at the beginning and ending of games. Clears queued commands and resets optimizer settings.
    void init();

    // The optimization level
    int level = 0;

  private:
    std::list<UnitCommand> optimizerQueue[UnitCommandTypes::Enum::MAX];

  };

}
