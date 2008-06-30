#pragma once

#include <list>
#include <BW/UnitType.h>
#include "BuildWeight.h"

class TiXmlElement;

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
      std::list<BuildWeight*> weights;
      /**
       * Creates new BuildRatio specification.
       * @param factoryName The name of the fatory that should produce
       * @param weights List of pairs "name of unit" + "weight" for the specified factory
       */                                               
      BuildWeights(TiXmlElement* element);
      ~BuildWeights();
      void loadTypes();
  };
}