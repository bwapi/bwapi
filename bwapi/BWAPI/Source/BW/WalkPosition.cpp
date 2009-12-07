#include "WalkPosition.h"
#include "Position.h"
#include "BuildPosition.h"

namespace BW
{
  WalkPosition WalkPosition::Invalid(Util::Point<u16>::Invalid);
  //----------------------------- CONSTRUCTOR ----------------------------------
  WalkPosition::WalkPosition()
    : Point()
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  WalkPosition::WalkPosition(Util::Point<u16> point)
    : Point(point)
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  WalkPosition::WalkPosition(u16 x, u16 y)
    : Point(x, y)
  {
  }
  //------------------------------ CONVERSION ----------------------------------
  WalkPosition::WalkPosition(const Position &convertFrom)
    : Point(convertFrom.x/8, convertFrom.y/8)
  {
  }
  //------------------------------ CONVERSION ----------------------------------
  WalkPosition::WalkPosition(const BuildPosition &convertFrom)
    : Point(convertFrom.x*4, convertFrom.y*4)
  {
  }
  //------------------------------ ---------------------------------------------
};
