#include "CommandFight.h"

#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
#include <Util/Xml.h>
#include <BWAPI.h>
#include <BWAI/AI.h>
#include <BWAI/Globals.h>
#include "ConditionMinimalPopulation.h"
#include "Root.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandFight::CommandFight(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->dest_type = Util::Xml::getRequiredAttribute(xmlElement, "dest_type");
    this->position = Util::Xml::getOptionalAttribute(xmlElement, "position", "");

    const char * minimalPopulationAttribute = xmlElement->Attribute("minimal-population");
    if (minimalPopulationAttribute != NULL && this->condition == NULL)
    {
      this->condition = new ConditionMinimalPopulation(u16(Util::Strings::stringToInt(minimalPopulationAttribute))); 
      this->conditionRunType = ConditionRunType::WaitToApply;
    }
  }
  //----------------------------------------------------------------------------------------------------------
  bool CommandFight::executeInternal(Executor* executor)
  {
    //is it a valid destination type
    if (dest_type != "main" && dest_type != "expansion")
    {
      BWAI::ai->root->log->log("Unknown dest_type '%s'", this->dest_type.c_str());
      return true;
    }

    if (this->conditionApplies())
    {
      BWAI::ai->root->log->log("Command to fight '%s' called", this->dest_type.c_str());

      //Currently adding all fight units to the Fight task
      //TODO we'll define what type of units, how many, etc. to join the task according to the conditions in the build order xml
      u16 addedCount = 0;

      if (dest_type == "main")
      {
        BWAPI::Position position = BWAI::ai->getEnemyMain();

        for each (BWAI::Unit* i in BWAI::ai->units)
          if (!i->getType().isBuilding() &&
            !i->getType().isWorker() &&
            i->getType().canMove() &&
            i->getOwner() == BWAI::ai->player &&
            i->getTask() == NULL)
          {
            addedCount++;
            i->orderAttackLocation(position, BWAPI::Orders::AttackMove);
          }
      }

      BWAPI::Broodwar->print("%u units sent to fight '%s'", addedCount, this->dest_type.c_str());

      return true;
    }

    return false;
  }
  //----------------------------------------------------------------------------------------------------------
}