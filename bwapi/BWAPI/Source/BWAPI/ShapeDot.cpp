#include "ShapeDot.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeDot::ShapeDot(int coordinateType, int x, int y, int color)
  :coordinateType(coordinateType)
  ,x(x)
  ,y(y)
  ,color(color)
  {
  }

  void ShapeDot::draw()
  {
    drawBox(x,y,1,1,color,coordinateType);
  }
}