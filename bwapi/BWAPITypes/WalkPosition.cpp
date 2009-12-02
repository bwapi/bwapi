#include "WalkPosition.h"
#include "Position.h"
#include "BuildPosition.h"

namespace BWAPI
{
  WalkPosition WalkPosition::Invalid(Math::Point::Invalid);
  //----------------------------- CONSTRUCTOR ----------------------------------
  WalkPosition::WalkPosition()
    : Point()
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  WalkPosition::WalkPosition(Math::Point point)
    : Point(point)
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  WalkPosition::WalkPosition(int x, int y)
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
