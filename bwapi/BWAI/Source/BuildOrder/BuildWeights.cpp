#include "BuildWeights.h"

#include <BWAI/AI.h>
#include <BWAI/Globals.h>

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  BuildWeights::BuildWeights(const std::string& factoryName, const std::list<std::pair<std::string,int> >& weights)
  {
    this->factory = BWAI::ai->unitNameToType[factoryName];
    for (std::list<std::pair<std::string,int> >::const_iterator i = weights.begin();
         i != weights.end();
         ++i)
      this->weights.push_back(std::pair<BW::UnitType, int>(BWAI::ai->unitNameToType[(*i).first], (*i).second));
  }
  //---------------------------------------------------------------------------
}