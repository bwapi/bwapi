#pragma once

#include "Command.h"

#include <BW/UpgradeType.h>

namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of upgrade command. */
  class CommandUpgrade : public Command
  {
    public :
      /**
       * @param building Building thatupgrades
       * @param upgrade Upgrade to be invented
       */
      CommandUpgrade(UnitImpl* building, BW::UpgradeType upgrade);
      ~CommandUpgrade();
      virtual void execute();
      int getType();
      std::string describe();
    private :
      BW::UpgradeType upgrade;
  };
}