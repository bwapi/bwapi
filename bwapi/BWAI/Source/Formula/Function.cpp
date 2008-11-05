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
      case FunctionType::Finished   : return (float) BWAPI::Broodwar.BWAPIPlayer->getCompletedUnits(this->unitType.getID());
      case FunctionType::All        : return (float) BWAPI::Broodwar.BWAPIPlayer->getAllUnits(this->unitType.getID());
      case FunctionType::InProgress : return (float) BWAPI::Broodwar.BWAPIPlayer->getIncompleteUnits(this->unitType.getID());
      case FunctionType::ConfigValue: 
        return this->configValue;
      case FunctionType::ActiveMinerals :
        return (float) BWAI::ai->activeMinerals.size();
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
    }
    view += Util::Strings::ssprintf("%s)=%f", this->functionParameter.c_str(), this->evaluate());
  }
  //----------------------------------------------------------------------------------------------------------
}