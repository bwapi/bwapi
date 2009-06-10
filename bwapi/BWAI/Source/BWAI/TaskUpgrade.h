#pragma once
#include "Task.h"

#include <BWAPI.h>

namespace BWAI
{
  /** Represents task to upgrade given by the ai. */
  class TaskUpgrade : public Task
  {
    public :
      TaskUpgrade(BWAPI::UpgradeType upgrade, u8 level, u16 priority);
      virtual ~TaskUpgrade();
      bool execute();
      TaskType::Enum getType();
      BWAPI::UpgradeType getUpgradeType();
    private :  
      BWAPI::UpgradeType upgradeType;
      u8 level;
  };
}