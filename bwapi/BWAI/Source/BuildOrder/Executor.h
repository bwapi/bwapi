#pragma once

#include <list>
#include "CommandPointer.h"
namespace BuildOrder{ class Root; }
namespace BuildOrder{ class Command; }
namespace BuildOrder{ class RootBranch; }
namespace BuildOrder{ class CommandCall; }

namespace BuildOrder
{
  class Executor
  {
    friend class CommandCall;
    public :
      Executor(RootBranch* start);
      void execute();
      bool finished();
      Branch* actualBranch();
    private :
      std::list<CommandPointer> callStack;
  };
}