#include "ShapeEllipse.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeEllipse::ShapeEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, int color, bool isSolid)
  :ctype(ctype)
  ,x(x)
  ,y(y)
  ,xrad(xrad)
  ,yrad(yrad)
  ,color(color)
  ,isSolid(isSolid)
  {
  }

  void ShapeEllipse::draw()
  {
    if (this->isSolid)
    {
      
    }
    else
    {
      int x = this->xrad;
      int y = 0;
      int twoAsquare = 2 * this->xrad * this->xrad;
      int twoBsquare = 2 * this->yrad * this->yrad;
      int xchange = this->yrad * this->yrad * (1 - 2 * this->xrad);
      int ychange = this->xrad * this->xrad;
      int ellipseerror = 0;
      int stoppingX = twoBsquare * this->xrad;
      int stoppingY = 0;
      
      while (stoppingX >= stoppingY)
      {
        drawDot(this->x + x, this->y + y,color,ctype);
        drawDot(this->x - x, this->y + y,color,ctype);
        drawDot(this->x - x, this->y - y,color,ctype);
        drawDot(this->x + x, this->y - y,color,ctype);
        
        x++;
        stoppingX += twoBsquare;
        ellipseerror += xchange;
        xchange += twoBsquare;
        if((2 * ellipseerror + ychange) > 0)
        {
          y--;
          stoppingY -= twoAsquare;
          ellipseerror += ychange;
          ychange += twoAsquare;
        }
      }
    }
  }
}