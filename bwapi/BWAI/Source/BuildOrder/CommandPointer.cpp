#include "CommandPointer.h"
#include "Branch.h"
#include "RootBranch.h"

namespace BuildOrder
{
  //------------------------------------------------ FINISHED ------------------------------------------------
  bool CommandPointer::finished()
  {
    return this->position == this->branch->commands.end();
  }
  //----------------------------------------------------------------------------------------------------------
  CommandPointer::CommandPointer(Branch* branch)
  {
    this->branch = branch;
    this->position = branch->commands.begin();
  }
  //----------------------------------------------------------------------------------------------------------
}