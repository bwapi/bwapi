#include "BuildPosition.h"
#include "Position.h"
#include "WalkPosition.h"

namespace BW
{
  BuildPosition BuildPosition::Invalid(Util::Point<u16>::Invalid);
  //----------------------------- CONSTRUCTOR ----------------------------------
  BuildPosition::BuildPosition()
    : Point()
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  BuildPosition::BuildPosition(Util::Point<u16> point)
    : Point(point)
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  BuildPosition::BuildPosition(u16 x, u16 y)
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
