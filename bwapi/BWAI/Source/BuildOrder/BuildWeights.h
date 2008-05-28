#pragma once

#include <list>
//#include <pair>

#include "../../../BWAPI/Source/BW/UnitType.h"

namespace BuildOrder
{
  /** 
   * Specifies ratio of units that should be built in building.
   * It can provide more complex settings later (like at least 2 dropships then vessel etc)
   */
  class BuildWeights
  {
    public :
      BW::UnitType factory;
      std::list<std::pair<BW::UnitType, int> > weights;
      BuildWeights(const std::string& factoryName, const std::list<std::pair<std::string,int> >& weights);
  };
}