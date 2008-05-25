#pragma once

#include <list>
//#include <pair>

#include "../../../BWAPI/Source/BW/UnitType.h"

namespace BuildOrder
{
  class BuildWeights
  {
    public :
      BW::UnitType factory;
      std::list<std::pair<BW::UnitType, int> > weights;
      BuildWeights(const std::string& factoryName, const std::list<std::pair<std::string,int> >& weights);
  };
}