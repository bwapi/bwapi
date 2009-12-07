/* moved to BW 
TODO: look if it pays off

#include "Position.h"
#include "WalkPosition.h"
#include "BuildPosition.h"

namespace BWAPI
{
  Position Position::Invalid(Math::Point::Invalid);
  //----------------------------- CONSTRUCTOR ----------------------------------
  Position::Position()
    : Point()
  {
  }
  //----------------------------- CONSTRUCTOR ----------------------------------
  Position::Position(Math::Point point)
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
*/
