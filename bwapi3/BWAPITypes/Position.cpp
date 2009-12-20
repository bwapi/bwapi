#include "Position.h"
#include "WalkPosition.h"
#include "BuildPosition.h"

namespace BWAPI
{
  Position Position::Invalid(Util::Point<int>::Invalid);
  //----------------------------- CONSTRUCTOR ----------------------------------
  Position::Position()
    : Point()
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
