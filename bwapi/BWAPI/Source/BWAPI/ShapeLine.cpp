#include "ShapeLine.h"
#include "DLLMain.h"
#include <math.h>
#include "GameImpl.h"
namespace BWAPI
{
  ShapeLine::ShapeLine(int ctype, int x1, int y1, int x2, int y2, int color)
      : ctype(ctype)
      , x1(x1)
      , y1(y1)
      , x2(x2)
      , y2(y2)
      , color(color)
  {
  }

  void ShapeLine::draw()
  {
    int x1 = this->x1;
    int y1 = this->y1;
    int x2 = this->x2;
    int y2 = this->y2;
    int Dx = x2 - x1;
    int Dy = y2 - y1;
    int steep = (abs(Dy) >= abs(Dx));
    if (steep)
    {
      int tmp = x1; x1 = y1; y1 = tmp;
      tmp = x2; x2 = y2; y2 = tmp;
      // recompute Dx, Dy after swap
      Dx = x2 - x1;
      Dy = y2 - y1;
    }
    int xstep = 1;
    if (Dx < 0)
    {
      xstep = -1;
      Dx = -Dx;
    }
    int ystep = 1;
    if (Dy < 0)
    {
      ystep = -1;
      Dy = -Dy;
    }
    int TwoDy = 2 * Dy;
    int TwoDyTwoDx = TwoDy - 2 * Dx; // 2*Dy - 2*Dx
    int E = TwoDy - Dx; //2*Dy - Dx
    int y = y1;
    int xDraw, yDraw;
    for (int x = x1; x != x2; x += xstep)
    {
      if (steep)
      {
        xDraw = y;
        yDraw = x;
      }
      else
      {
        xDraw = x;
        yDraw = y;
      }
      // plot
      drawDot(xDraw, yDraw, color, ctype);
      // next
      if (E > 0)
      {
        E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
        y = y + ystep;
      }
      else
      {
        E += TwoDy; //E += 2*Dy;
      }
    }
  }
}