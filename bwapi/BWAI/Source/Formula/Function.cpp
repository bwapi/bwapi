#include "Function.h"

#include <Util/Strings.h>
#include <Util/Dictionary.h>
#include <Util/Logger.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Player.h>
#include <BWAI/Globals.h>

namespace Formula
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Function::Function(const std::string& input, size_t& position)
  {
    std::string name = Util::Strings::readWord(input, position);
    needParameter = true;
    if (name == "Finished")
      this->type = FunctionType::Finished;
    else if (name == "All")
      this->type = FunctionType::All;
    else if (name == "InProgress")
      this->type = FunctionType::InProgress;
    else if (name == "ConfigValue")
      this->type = FunctionType::ConfigValue;
    else if (name == "ActiveMinerals")
    {
      this->type = FunctionType::ActiveMinerals;
      needParameter = false;
    }
    else if (name == "UsedSupply")
      this->type = FunctionType::UsedSupply;
    else if (name == "PlannedSupply")
      this->type = FunctionType::PlannedSupply;
    else if (name == "Factories")
      this->type = FunctionType::Factories;
    else 
      throw ParseException("Unknown Function '" + name + "'");
    if (needParameter)
    {
      if (input[position] != '(')
        throw ParseException("Parameter required for function '" + name + "'");
      position ++;
      size_t parameterStart = position;
      while (input[position] != ')' &&
             input[position] != 0)
        position++;
      if (input[position] == 0)
        throw ParseException("Not closed parameter - ')' expected in " + input);
      this->functionParameter = input.substr(parameterStart, position - parameterStart);
    }
    switch (this->type)
    {
      case FunctionType::Finished :
      case FunctionType::All :
      case FunctionType::InProgress :
        unitType = BWAPI::Broodwar.unitNameToType[this->functionParameter];
        if (unitType == BW::UnitID::None)
          throw ParseException("Unknown unit name " + this->functionParameter);
        break;
      case FunctionType::ConfigValue :
        sscanf(config->get(this->functionParameter).c_str(),"%f", &this->configValue);
      case FunctionType::UsedSupply :
      case FunctionType::PlannedSupply :
      case FunctionType::Factories :
        this->race = BW::Race::stringToRace(this->functionParameter);
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Function::~Function()
  {
  }
  //------------------------------------------------ EVALUATE ------------------------------------------------
  float Function::evaluate()
  {
    switch (this->type)
    {
      case FunctionType::Finished   : return (float) BWAPI::Broodwar.BWAPIPlayer->getCompletedUnits(this->unitType);
      case FunctionType::All        : return (float) BWAPI::Broodwar.BWAPIPlayer->getAllUnitsLocal(this->unitType);
      case FunctionType::InProgress : return (float) BWAPI::Broodwar.BWAPIPlayer->getIncompleteUnitsLocal(this->unitType);
      case FunctionType::ConfigValue: 
        return this->configValue;
      case FunctionType::ActiveMinerals :
        return (float) BWAI::ai->activeMinerals.size();
      case FunctionType::UsedSupply : return (float) BWAPI::Broodwar.BWAPIPlayer->getSuppliesUsedLocal(this->race)/2;
      case FunctionType::PlannedSupply : return (float) (BWAI::ai->plannedSupplyGain(race) + BWAPI::Broodwar.BWAPIPlayer->getSuppliesAvailableLocal(race))/2;
      case FunctionType::Factories : return (float) BWAPI::Broodwar.BWAPIPlayer->getCompletedUnits(BW::UnitID::Factories, race);
    }
    return 0;
  }
  //--------------------------------------------- DEBUG EVALUATE ---------------------------------------------
  void Function::debugEvaluate(std::string& view)
  {
    switch (this->type)
    {
      case FunctionType::Finished       : view += "Finished("; break;
      case FunctionType::All            : view += "All("; break;
      case FunctionType::InProgress     : view += "InProgress("; break;
      case FunctionType::ConfigValue    : view += "ConfigValue("; break;
      case FunctionType::ActiveMinerals : view += "ActiveMinerals("; break;
      case FunctionType::UsedSupply     : view += "UsedSupply("; break;
      case FunctionType::PlannedSupply  : view += "PlannedSupply("; break;
      case FunctionType::Factories      : view += "Factories("; break;
    }
    view += Util::Strings::ssprintf("%s)=%f", this->functionParameter.c_str(), this->evaluate());
  }
  //----------------------------------------------------------------------------------------------------------
}