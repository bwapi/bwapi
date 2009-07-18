#pragma once

#include <list>
#include <set>
#include "CommandPointer.h"
namespace BuildOrder { class Root; }
namespace BuildOrder { class Command; }
namespace BuildOrder { class RootBranch; }
namespace BuildOrder { class CommandCall; }

namespace BuildOrder
{
  class Executor
  {
      friend class CommandCall;
      friend class CommandRegisterException;
    public :
      Executor(RootBranch* start);
      void execute();
      bool finished();
      Branch* actualBranch();

    private :
      std::list<CommandPointer> callStack;
      std::set<std::string> registeredExceptions;
  };
}