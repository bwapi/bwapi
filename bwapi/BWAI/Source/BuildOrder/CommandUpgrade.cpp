#include "CommandUpgrade.h"

#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
#include <Util/Xml.h>
#include <BWAI/Globals.h>
#include <BWAI/TaskUpgrade.h>
#include <BWAI/Player.h>
#include <BWAPI.h>
#include "ConditionMinimalPopulation.h"
#include "Root.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUpgrade::CommandUpgrade(TiXmlElement* xmlElement)
      : Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
    this->level = (u8) Util::Xml::getOptionalU16Attribute(xmlElement, "level", 1);
  }
  //----------------------------------------------------------------------------------------------------------
  bool CommandUpgrade::executeInternal(Executor* executor)
  {
    BWAPI::UpgradeType toUpgrade = BWAPI::UpgradeTypes::getUpgradeType(this->name);
    if (toUpgrade == BWAPI::UpgradeTypes::None || toUpgrade == BWAPI::UpgradeTypes::Unknown)
    {
      BWAI::ai->root->log->log("Unknown upgrade name '%s'", this->name.c_str());
      return true;
    }
    if (BWAI::ai->player->canAfford(toUpgrade, level) &&
        this->conditionApplies())
    {
      BWAI::ai->root->log->log("Command to upgrade '%s' called", this->name.c_str());
      BWAI::ai->plannedUpgrades.push_back(new BWAI::TaskUpgrade(toUpgrade, level, priority));
      return true;
    }
    return false;
  }
  //----------------------------------------------------------------------------------------------------------
}