#pragma once

#include <list>
#include <BWAPI.h>
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
      BWAPI::UnitType factory; /**< Factory whose building rate is affected. */
      std::list<BuildWeight*> weights;
      /**
       * Creates new BuildRatio specification according to it's xml representation
       * @param element XML representation of this class
       */                                               
      BuildWeights(TiXmlElement* element);
      ~BuildWeights();
      void loadTypes();
  };
}