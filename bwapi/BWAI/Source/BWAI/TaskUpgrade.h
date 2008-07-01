#pragma once
#include "Task.h"

#include <BW/UpgradeType.h>

namespace BWAI
{
  /** Represents task to upgrade given by the ai. */
  class TaskUpgrade : public Task
  {
    public :
      TaskUpgrade(BW::UpgradeType upgrade, u8 level, u16 priority);
      virtual ~TaskUpgrade();
      bool execute();
      TaskType::Enum getType();
      BW::UpgradeType getUpgradeType();
    private :  
      BW::UpgradeType upgradeType;
      u8 level;
  };
}