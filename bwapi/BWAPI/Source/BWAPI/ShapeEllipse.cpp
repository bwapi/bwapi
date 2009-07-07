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

  #define incx() x++, dxt += d2xt, t += dxt
	#define incy() y--, dyt += d2yt, t += dyt

  void ShapeEllipse::fillRect(int x, int y, int w, int h)
  {
    if (w != 0 && h != 0)
    {
      if (h < 0)
        drawBox(x, y - h, w, -h, color, ctype);
      else
        drawBox(x, y, w, h, color, ctype);
    }
  }
  void ShapeEllipse::draw()
  {
    if (this->isSolid)
    {
      int x = 0, y = this->yrad;
      int rx = x, ry = y;
      int width = 1;
      int height = 1;
      int a2 = this->xrad * this->xrad, b2 = this->yrad * this->yrad;
      int crit1 = -(a2/4 + this->xrad%2 + b2);
      int crit2 = -(b2/4 + this->yrad%2 + a2);
      int crit3 = -(b2/4 + this->yrad%2);
      int t = -a2 * y;
      int dxt = 2 * b2 * x, dyt = -2 * a2 * y;
      int d2xt = 2 * b2, d2yt = 2 * a2;
      
      if (this->yrad == 0)
      {
        fillRect(this->x - this->xrad, this->y, 2 * this->xrad + 1, 1);
        return;
      }
      
      while (y >= 0 && x <= this->xrad)
      {
        if (t+ b2 * x <= crit1 ||
            t+ a2 * y <= crit3)
        {
          if (height == 1);
          else if (ry * 2 + 1 > (height - 1) * 2)
          {
            fillRect(this->x - rx, this->y - ry, width, height - 1);
            fillRect(this->x - rx, this->y + ry + 1, width, 1 - height);
            ry -= height - 1;
            height = 1;
          } 
          else
          {
            fillRect(this->x - rx, this->y - ry, width, ry * 2 + 1);
            ry -= ry;
            height = 1;  
          }
          incx();
          rx++;
          width += 2;
        }
        else if (t - a2 * y > crit2)
        {
          incy();
          height++;
        }
        else
        {
          if (ry * 2 + 1 > height * 2)
          {
            fillRect(this->x - rx, this->y - ry, width, height);
            fillRect(this->x - rx, this->y + ry + 1, width, -height);
          }
          else
          {
            fillRect(this->x - rx, this->y - ry, width, ry * 2 + 1);
          }
          incx();
          incy();
          rx++;
          width += 2;
          ry -= height;
          height = 1;
        }
      }
      
      if (ry > height)
      {
        fillRect(this->x - rx, this->y - ry, width, height);
        fillRect(this->x - rx, this->y + ry + 1, width, -height);
      }
      else
      {
        fillRect(this->x - rx, this->y - ry, width, ry * 2 + 1);
      }
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
        
        y++;
        stoppingY += twoAsquare;
        ellipseerror += ychange;
        ychange += twoAsquare;
        if ((2 * ellipseerror + xchange) > 0)
        {
          x--;
          stoppingX -= twoBsquare;
          ellipseerror += xchange;
          xchange += twoBsquare;
        }
      }
      x = 0;
      y = this->yrad;
      xchange = this->yrad * this->yrad;
      ychange = this->xrad * this->xrad * (1 - 2 * this->yrad);
      ellipseerror = 0;
      stoppingX = 0;
      stoppingY = twoAsquare * this->yrad;
      while (stoppingX <= stoppingY)
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