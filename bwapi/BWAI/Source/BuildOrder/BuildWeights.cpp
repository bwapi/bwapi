#include "BuildWeights.h"

#include <Util/Logger.h>

#include <BWAI/AI.h>
#include <BWAI/Globals.h>

#include "Root.h"

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  BuildWeights::BuildWeights(const std::string& factoryName, const std::list<std::pair<std::string,int> >& weights)
  {
    this->factory = BWAI::ai->unitNameToType[factoryName];
    for (std::list<std::pair<std::string,int> >::const_iterator i = weights.begin();
         i != weights.end();
         ++i)
      {
        BW::UnitType unitType = BWAI::ai->unitNameToType[(*i).first];
        if (unitType == BW::UnitID::None)
          {
            BWAI::ai->root->log->log("Unknown unit name in change weights: '%s'", (*i).first);
            continue;
          }
        this->weights.push_back(std::pair<BW::UnitType, int>(BWAI::ai->unitNameToType[(*i).first], (*i).second));
      }
  }
  //---------------------------------------------------------------------------
}