#include "UnitModel.h"

#include "../BWAI/Unit.h"

namespace PathFinding
{
  //---------------------------------------- CONSTRUCTOR -------------------------------------------
  UnitModel::UnitModel(const BWAI::Unit*& original)
  :original(original)
  ,position(original->getPosition())
  {
  }
  //------------------------------------------------------------------------------------------------
}