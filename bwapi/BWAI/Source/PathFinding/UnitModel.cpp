#include "UnitModel.h"

#include <BWAI/Unit.h>

namespace PathFinding
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitModel::UnitModel(const BWAI::Unit* original)
      : original(original)
      , position(original->getPosition())
      , walkabilityPosition(original->getPosition())
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitModel::UnitModel(const UnitModel& model)
      : original(model.original)
      , position(model.position)
      , walkabilityPosition(model.walkabilityPosition)
  {
  }
  //----------------------------------------------------------------------------------------------------------
}