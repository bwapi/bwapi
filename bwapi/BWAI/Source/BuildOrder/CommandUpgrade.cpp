#include "CommandUpgrade.h"

#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
#include <Util/Xml.h>

#include <BW/UpgradeType.h>

#include <BWAI/Globals.h>
#include <BWAI/TaskUpgrade.h>

#include <BWAPI/Player.h>
#include <BWAPI/Globals.h>

#include "ConditionMinimalPopulation.h"
#include "Root.h"

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  CommandUpgrade::CommandUpgrade(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
    this->level = (u8) Util::Xml::getOptionalU16Attribute(xmlElement, "level", 1);
  }
  //---------------------------------------------------------------------------
  bool CommandUpgrade::execute()
  {
    BW::UpgradeType toUpgrade = BWAPI::Broodwar.upgradeNameToType[this->name];
    if (!toUpgrade.isValid())
    {
      BWAI::ai->root->log->log("Unknown upgrade name '%s'", this->name.c_str());
      return true;
    }
    if (BWAI::ai->player->canAfford(toUpgrade, level, BWAI::ai->reserved) &&
        this->conditionApplies()) 
    {
      BWAI::ai->root->log->log("Command to upgrade '%s' called", this->name.c_str());
      BWAI::ai->plannedUpgrades.push_back(new BWAI::TaskUpgrade(toUpgrade, level));        
      return true;
    }
    return false;
  }
  //---------------------------------------------------------------------------
}