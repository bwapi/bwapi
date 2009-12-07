#include "Position.h"
#include "WalkPosition.h"
#include "BuildPosition.h"

namespace BW
{
  Position Position::Invalid(Util::Point<u16>::Invalid);
  //----------------------------- CONSTRUCTOR ----------------------------------
  Position::Position()
    : Point()
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  Position::Position(Util::Point<u16> point)
    : Point(point)
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  Position::Position(int x, int y)
    : Point(x, y)
  {
  }
  //------------------------------ CONVERSION ----------------------------------
  Position::Position(const WalkPosition &convertFrom)
    : Point(convertFrom.x*8, convertFrom.y*8)
  {
  }
  //------------------------------ CONVERSION ----------------------------------
  Position::Position(const BuildPosition &convertFrom)
    : Point(convertFrom.x*32, convertFrom.y*32)
  {
  }
  //------------------------------ ---------------------------------------------
};
