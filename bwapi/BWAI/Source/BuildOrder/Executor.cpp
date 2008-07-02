#include "Executor.h"

#include "Root.h"
#include "RootBranch.h"
#include "Command.h"
#include "CommandPointer.h"

namespace BuildOrder
{
  //------------------------------------------------- CONSTRUCTOR --------------------------------------------
  Executor::Executor(RootBranch* start)
  {
    this->callStack.push_back(CommandPointer(start));
  }
  //--------------------------------------------------- EXECUTE ----------------------------------------------
  void Executor::execute()
  {
    again:
    if (this->finished())
      return;
    CommandPointer& last = callStack.back();
    if (last.finished())
    {
      callStack.erase(--callStack.end());
      goto again;
    }
    if ((*last.position)->execute(this))
    {
      ++last.position;
      goto again;
    }
  }
  //-------------------------------------------------- FINISHED ----------------------------------------------
  bool Executor::finished()
  {
    return this->callStack.empty();
  }
  //------------------------------------------------ ACTUAL BRANCH -------------------------------------------
  Branch* Executor::actualBranch()
  {
    if (this->finished())
      return NULL;
    return this->callStack.front().branch;
  }
  //----------------------------------------------------------------------------------------------------------
}