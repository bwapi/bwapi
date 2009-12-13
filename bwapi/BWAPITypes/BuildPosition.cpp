#include "Position.h"
#include "WalkPosition.h"
#include "BuildPosition.h"

namespace BWAPI
{
  BuildPosition BuildPosition::Invalid(Util::Point<int>::Invalid);
  //----------------------------- CONSTRUCTOR ----------------------------------
  BuildPosition::BuildPosition()
    : Point()
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  BuildPosition::BuildPosition(int x, int y)
    : Point(x, y)
  {
  }
  //------------------------------ CONVERSION ----------------------------------
  BuildPosition::BuildPosition(const Position &convertFrom)
    : Point(convertFrom.x/32, convertFrom.y/32)
  {
  }
  //------------------------------ CONVERSION ----------------------------------
  BuildPosition::BuildPosition(const WalkPosition &convertFrom)
    : Point(convertFrom.x/8, convertFrom.y/8)
  {
  }
  //------------------------------ ---------------------------------------------
};
