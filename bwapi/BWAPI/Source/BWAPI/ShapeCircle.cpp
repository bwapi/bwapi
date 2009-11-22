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
    if (this->isSolid)
    {
      int f = 1 - radius;
      int ddF_x = 1;
      int ddF_y = -2 * radius;
      int xi = 0;
      int yi = radius;

      drawDot(x, y + radius, color, ctype);
      drawDot(x, y - radius, color, ctype);
      drawBox(x - radius, y, radius*2, 1, color, ctype);

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
        drawBox(x - xi, y - yi, xi*2 + 1, 1, color, ctype);
        drawBox(x - xi, y + yi, xi*2 + 1, 1, color, ctype);
        drawBox(x - yi, y - xi, yi*2 + 1, 1, color, ctype);
        drawBox(x - yi, y + xi, yi*2 + 1, 1, color, ctype);
      }
    }
    else
    {
      int f = 1 - radius;
      int ddF_x = 1;
      int ddF_y = -2 * radius;
      int xi = 0;
      int yi = radius;

      drawDot(x, y + radius, color, ctype);
      drawDot(x, y - radius, color, ctype);
      drawDot(x + radius, y, color, ctype);
      drawDot(x - radius, y, color, ctype);

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
        drawDot(x + xi, y + yi, color, ctype);
        drawDot(x - xi, y + yi, color, ctype);
        drawDot(x + xi, y - yi, color, ctype);
        drawDot(x - xi, y - yi, color, ctype);
        drawDot(x + yi, y + xi, color, ctype);
        drawDot(x - yi, y + xi, color, ctype);
        drawDot(x + yi, y - xi, color, ctype);
        drawDot(x - yi, y - xi, color, ctype);
      }
    }
  }
};
