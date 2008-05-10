#include "Unit.h"
#include "..//..//BWAPI//Source//Types.h"
namespace BWAI
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  Unit::Unit(BWAPI::Unit *unit)
  :BWAPI::Unit(*unit) /**< default autoconstructor usage */
  ,expansionAssingment(0)
  ,selected(false)
  {
  }
  //---------------------------------------------------------------------------
}