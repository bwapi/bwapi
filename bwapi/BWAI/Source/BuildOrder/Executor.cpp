#include "Executor.h"

#include <BWAI/Globals.h>
#include <Util/Logger.h>
#include "Root.h"
#include "RootBranch.h"
#include "Command.h"
#include "CommandPointer.h"
#include "CommandCall.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Executor::Executor(RootBranch* start)
  {
    this->callStack.push_back(CommandPointer(start));
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
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
    for each (const std::string& i in this->registeredExceptions)
      {
        for each (const CommandPointer& j in this->callStack)
          if (j.branch->getName() == i)
          // I must check if I don't call exception from inside of the exception body (or function called from 
          // inside it's body) to avoid unexpected behaviour
            goto end;
        {
          CommandCall test(i);
          test.execute(this);
        }
        end:;
      }
    if ((*last.position)->execute(this))
    {
      ++last.position;
      goto again;
    }
  }
  //------------------------------------------------ FINISHED ------------------------------------------------
  bool Executor::finished()
  {
    return this->callStack.empty();
  }
  //--------------------------------------------- ACTUAL BRANCH ----------------------------------------------
  Branch* Executor::actualBranch()
  {
    if (this->finished())
      return NULL;
    return this->callStack.front().branch;
  }
  //----------------------------------------------------------------------------------------------------------
}