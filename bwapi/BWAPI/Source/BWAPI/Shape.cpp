#pragma once

#include <BW/Hook.h>
#include <math.h>

namespace BWAPI
{
  void drawLine(int x1, int y1, int x2, int y2, int color)
  {
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
      BW::drawDot(xDraw, yDraw, color);
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

  void drawRectangle(int left, int top, int width, int height, int color, bool isSolid)
  {
    if (isSolid)
    {
      BW::drawBox(left, top, width, height, color);
    }
    else
    {
      int lineWidth = 1;
      int right = left + width;
      int bottom = top + height;
      BW::drawBox(left, top, lineWidth, height, color);
      BW::drawBox(left, top, width, lineWidth, color);
      BW::drawBox(right - lineWidth, top, lineWidth, height, color);
      BW::drawBox(left, bottom - lineWidth, width, lineWidth, color);
    }
  }

  void drawCircle(int x, int y, double radius, int color, bool isSolid)
  {
    if (isSolid)
    {
      int f = (int)(1 - radius);
      int ddF_x = 1;
      int ddF_y = (int)(-2 * radius);
      int xi = 0;
      int yi = (int)(radius);

      BW::drawDot(x, (int)(y + radius), color);
      BW::drawDot(x, (int)(y - radius), color);
      BW::drawBox((int)(x - radius), y, (int)(radius*2), 1, color);

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
        BW::drawBox(x - xi, y - yi, xi*2 + 1, 1, color);
        BW::drawBox(x - xi, y + yi, xi*2 + 1, 1, color);
        BW::drawBox(x - yi, y - xi, yi*2 + 1, 1, color);
        BW::drawBox(x - yi, y + xi, yi*2 + 1, 1, color);
      }
    }
    else
    {
      int f = (int)(1 - radius);
      int ddF_x = 1;
      int ddF_y = (int)(-2 * radius);
      int xi = 0;
      int yi = (int)(radius);

      BW::drawDot(x, (int)(y + radius), color);
      BW::drawDot(x, (int)(y - radius), color);
      BW::drawDot((int)(x + radius), y, color);
      BW::drawDot((int)(x - radius), y, color);

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
        BW::drawDot(x + xi, y + yi, color);
        BW::drawDot(x - xi, y + yi, color);
        BW::drawDot(x + xi, y - yi, color);
        BW::drawDot(x - xi, y - yi, color);
        BW::drawDot(x + yi, y + xi, color);
        BW::drawDot(x - yi, y + xi, color);
        BW::drawDot(x + yi, y - xi, color);
        BW::drawDot(x - yi, y - xi, color);
      }
    }
  }
};
