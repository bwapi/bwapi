#include "Command.h"
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Command::Command(const std::vector<UnitImpl*>& executors)
      : executors(executors)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Command::Command(UnitImpl* executor)
  {
    executors.push_back(executor);
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Command::~Command()
  {
  }
  //----------------------------------------------------------------------------------------------------------
};
