#pragma once

#include "GameAction.h"

namespace ReplayTool
{
  class AbstractReplayReader;

  class UpgradeAction : public GameAction
  {
  public:
    UpgradeAction(PlayerID player);
    UpgradeAction(PlayerID player, BWAPI::UpgradeType upgradeType);

    void read(AbstractReplayReader &reader);
    std::string toString() const;
    const BWAPI::UpgradeType& getUpgrade() const { return upgradeType; }

    IMPLEMENT_FROM(UpgradeAction);

  private:
    BWAPI::UpgradeType upgradeType;
  };
}