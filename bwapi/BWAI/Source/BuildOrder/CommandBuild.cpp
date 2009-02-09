#include "CommandBuild.h"

#include <tinyxml.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
#include <Util/Xml.h>
#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BWAI/AI.h>
#include <BWAI/BuildingPositionSet.h>
#include <BWAI/TaskBuild.h>
#include <BWAI/Globals.h>
#include <BWAI/MapStartingPosition.h>
#include <BWAPI/Player.h>
#include <BWAPI/Globals.h>
#include "ConditionMinimalPopulation.h"
#include "Root.h"

namespace BWAI { class BuildingPosition; }

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandBuild::CommandBuild(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
    this->place = Util::Xml::getOptionalAttribute(xmlElement, "place", "");
    
    const char * minimalPopulationAttribute = xmlElement->Attribute("minimal-population");
    if (minimalPopulationAttribute != NULL && this->condition == NULL)
    {
      this->condition = new ConditionMinimalPopulation(u16(Util::Strings::stringToInt(minimalPopulationAttribute))); 
      this->conditionRunType = ConditionRunType::WaitToApply;
    }
  }
  //----------------------------------------------------------------------------------------------------------
  bool CommandBuild::executeInternal(Executor* executor)
  {
    BW::UnitType toBuild = BWAPI::Broodwar.unitNameToType[this->name];
    if (BWAI::ai->buildTaskUnitsPlanned[(u16)toBuild.getID()] < BWAPI::Broodwar.BWAPIPlayer->getAllUnits(toBuild.getID()))
    {
      BWAI::ai->buildTaskUnitsPlanned[(u16)toBuild.getID()]++;
      return true;
    }
    else
    {
      if (BWAI::ai->player->canAfford(toBuild, BWAI::ai->reserved) &&
          BWAI::ai->player->canBuild(toBuild)) 
      {
        BWAI::BuildingPositionSet* alternatives;
        if (!this->place.empty())
          alternatives = BWAI::ai->getPositionsCalled(this->place); 
        else
          alternatives = NULL;
       
        if (!toBuild.isAddon())
        {
          if (alternatives == NULL)
          {
            BWAI::ai->root->log->log("Position '%s' not found", this->place.c_str());
            return true;
          }
          if (alternatives->positions.empty())
          {
            BWAI::ai->root->log->log("Position '%s' is empty", this->place.c_str());
            return true;
          }
          BWAI::Unit* scvToUse = NULL;
          BWAI::BuildingPosition* position = BWAI::ai->getFreeBuildingSpot(this->place, scvToUse); 
          BWAI::ai->root->log->log("Command build '%s' called", this->name.c_str());
          BWAI::ai->plannedBuildings.push_back(new BWAI::TaskBuild(toBuild, position, scvToUse, alternatives, priority));
          BWAI::ai->buildTaskUnitsPlanned[(u16)toBuild.getID()]++;
        }
        else if (toBuild.isAddon())
        {
          BWAI::Unit* executor = NULL;
          BWAI::BuildingPosition* position = NULL;
          BW::TilePosition spot = BW::TilePosition::Invalid;

          if (alternatives != NULL)
          {
            BWAI::BuildingPosition* position = alternatives->positions.front();
            
            if (BWAPI::Broodwar.unitsOnTile[position->position.x - 2][position->position.y].empty())
            {
              BWAI::ai->root->log->log("Building for the addon not found", Util::LogLevel::Commmon);
              return false;
            }
            
           if (!BWAPI::Broodwar.unitsOnTile[position->position.x - 2][position->position.y].front()->isReady())
            {
              BWAI::ai->root->log->logCommon("Building for the addon not ready");
              return false;
            }
            
            executor = BWAI::Unit::BWAPIUnitToBWAIUnit(BWAPI::Broodwar.unitsOnTile[position->position.x - 2][position->position.y].front());
            if (!executor->getType().isBuilding())
            {
              BWAI::ai->root->log->log("Executor chosen is not building ???? but %s", executor->getName().c_str());
              return false;
            }
            BWAI::ai->plannedBuildings.push_back(new BWAI::TaskBuild(toBuild, position, executor, alternatives, priority));
            BWAI::ai->buildTaskUnitsPlanned[(u16)toBuild.getID()]++;
            BWAI::ai->root->log->log("Command to build addon '%s' called at '%s'", this->name.c_str(), this->place.c_str());
          }
          else
          {
            BW::UnitType builderType = toBuild.whereToBuild();
            for each (BWAI::Unit* i in BWAI::ai->units)
              if (i->isReady() &&
                  i->getType() == builderType &&
                  i->getRawDataLocal()->childInfoUnion.childUnit1 == NULL &&
                  (i->getTask() == NULL || i->getTask()->getType() != BWAI::TaskType::Build))
              {
                executor = i;
                spot = executor->getTilePosition();
                spot.x += 4;
                spot.y += 1;
                break;
              }
           if (executor == NULL)
             return false;
           BWAI::ai->plannedBuildings.push_back(new BWAI::TaskBuild(toBuild, executor, spot, priority));
           BWAI::ai->buildTaskUnitsPlanned[(u16)toBuild.getID()]++;
           BWAI::ai->root->log->log("Command to build addon '%s' called (using %s)", toBuild.getName(), executor->getType().getName());
          }
        }
        return true;
      }
      return false;
    }
  }
  //----------------------------------------------------------------------------------------------------------
}