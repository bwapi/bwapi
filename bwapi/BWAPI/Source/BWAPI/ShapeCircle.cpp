#include "ShapeCircle.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeCircle::ShapeCircle(int ctype, int x, int y, int radius, int color, bool isSolid)
      : ctype(ctype)
      , x(x)
      , y(y)
      , radius(radius)
      , color(color)
      , isSolid(isSolid)
  {
  }

  void ShapeCircle::draw()
  {
  }
};
