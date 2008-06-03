#pragma once

#include <list>
//#include <pair>

#include <BW/UnitType.h>

namespace BuildOrder
{
  /** 
   * Specifies ratio of units that should be built in building.
   * It can provide more complex settings later (like at least 2 dropships then vessel etc)
   */
  class BuildWeights
  {
    public :
      BW::UnitType factory; /**< Factory whose building rate is affected. */
      std::list<std::pair<BW::UnitType, int> > weights;
      /**
       * Creates new BuildRatio specification.
       * @param factoryName The name of the fatory that should produce
       * @param weights List of pairs "name of unit" + "weight" for the specified factory
       */                                               
      BuildWeights(const std::string& factoryName, const std::list<std::pair<std::string,int> >& weights);
  };
}