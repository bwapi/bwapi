#pragma once
#include "CommandTypes.h"
#include <vector>
namespace BWAPI
{
  class Unit;
  class Command
  {
    public :
      Command(const std::vector<Unit*> &executors);
      Command(Unit* executor);
      virtual ~Command();
      virtual BWAPI::CommandTypes::Enum getType() = 0;
      virtual void execute() = 0;
      bool failed;
    protected : 
      std::vector<Unit*> executors;
  };
}