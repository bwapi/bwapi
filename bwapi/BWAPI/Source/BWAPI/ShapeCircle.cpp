#include "ShapeCircle.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeCircle::ShapeCircle(int coordinateType, int x, int y, int radius, int color, bool isSolid)
  :coordinateType(coordinateType)
  ,x(x)
  ,y(y)
  ,radius(radius)
  ,color(color)
  ,isSolid(isSolid)
  {
  }

  void ShapeCircle::draw()
  {
    if (this->isSolid)
    {
    }
    else
    {
      int f = 1 - radius;
      int ddF_x = 1;
      int ddF_y = -2 * radius;
      int xi = 0;
      int yi = radius;
   
      drawDot(x, y + radius,color,coordinateType);
      drawDot(x, y - radius,color,coordinateType);
      drawDot(x + radius, y,color,coordinateType);
      drawDot(x - radius, y,color,coordinateType);
   
      while(xi < yi)
      {
        if(f >= 0) 
        {
          yi--;
          ddF_y += 2;
          f += ddF_y;
        }
        xi++;
        ddF_x += 2;
        f += ddF_x;    
        drawDot(x + xi, y + yi,color,coordinateType);
        drawDot(x - xi, y + yi,color,coordinateType);
        drawDot(x + xi, y - yi,color,coordinateType);
        drawDot(x - xi, y - yi,color,coordinateType);
        drawDot(x + yi, y + xi,color,coordinateType);
        drawDot(x - yi, y + xi,color,coordinateType);
        drawDot(x + yi, y - xi,color,coordinateType);
        drawDot(x - yi, y - xi,color,coordinateType);
      }
    }
  }
}