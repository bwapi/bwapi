#pragma once

#include "Shape.h"

#include <BW/Hook.h>
#include <math.h>

namespace BWAPI
{
  static void inline swap(int& a0, int& a1)
  {
    int tmpx = a0;
    a0 = a1;
    a1 = tmpx;
  }

  void drawLine(int x1, int y1, int x2, int y2, int color)
  {
    int Dx = x2 - x1;
    int Dy = y2 - y1;
    int steep = (abs(Dy) >= abs(Dx));
    if (steep)
    {
      swap(x1, y1);
      swap(x2, y2);
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

  static int inline rnd(float x)
  {
    return (x >= 0 ? ((int)( x + 0.5 )) : 0) ;
  }

  void drawTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color, bool isSolid)
  {
    if(isSolid)
    {
      if(ay > by) { swap(ax, bx); swap(ay, by); }
      if(ay > cy) { swap(ax, cx); swap(ay, cy); }
      if(by > cy) { swap(bx, cx); swap(by, cy); }

      float dx1, dx2, dx3;
      if (by - ay > 0) dx1 = (float)(bx - ax) / (by - ay); else dx1 = (float)bx - ax;
      if (cy - ay > 0) dx2 = (float)(cx - ax) / (cy - ay); else dx2 = 0;
      if (cy - by > 0) dx3 = (float)(cx - bx) / (cy - by); else dx3 = 0;

      float lx = (float)ax, rx = lx;
      int ly = ay, ry = ly;
      if(dx1 > dx2)
      {
        for(; ly <= by; ly++, ry++, lx += dx2, rx += dx1)
          BW::drawScanLine(rnd(lx), ly, rnd(rx - lx), color);
        rx = (float)bx; ry = by;
        for(; ly <= cy; ly++, ry++, lx += dx2, rx += dx3)
          BW::drawScanLine(rnd(lx), ly, rnd(rx - lx), color);
      }
      else
      {
        for(; ly <= by; ly++, ry++, lx += dx1, rx += dx2)
          BW::drawScanLine(rnd(lx), ly, rnd(rx - lx), color);
        lx = (float)bx; ly = by;
        for(; ly <= cy; ly++, ry++, lx += dx3, rx += dx2)
          BW::drawScanLine(rnd(lx), ly, rnd(rx - lx), color);
      }
    }
    else
    {
      drawLine(ax, ay, bx, by, color);
      drawLine(ax, ay, cx, cy, color);
      drawLine(bx, by, cx, cy, color);
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
      BW::drawScanLine((int)(x - radius), y, (int)(radius*2), color);

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
        BW::drawScanLine(x - xi, y - yi, xi*2 + 1, color);
        BW::drawScanLine(x - xi, y + yi, xi*2 + 1, color);
        BW::drawScanLine(x - yi, y - xi, yi*2 + 1, color);
        BW::drawScanLine(x - yi, y + xi, yi*2 + 1, color);
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

#define incx() x++, dxt += d2xt, t += dxt
#define incy() y--, dyt += d2yt, t += dyt

  void drawEllipse(int posx, int posy, int xrad, int yrad, int color, bool isSolid)
  {
    if (isSolid)
    {
      int x = 0, y = yrad;
      unsigned int width = 1;
      long a2 = (long)xrad * (int)xrad, b2 = (long)yrad * yrad;
      long crit1 = -(a2 / 4 + xrad % 2 + b2);
      long crit2 = -(b2 / 4 + yrad % 2 + a2);
      long crit3 = -(b2 / 4 + yrad % 2);
      long t = -a2 * y;
      long dxt = 2 * b2 * x, dyt = -2 * a2 * y;
      long d2xt = 2 * b2, d2yt = 2 * a2;

      while (y >= 0 && x <= xrad)
      {
        if (t + b2*x <= crit1 ||
          t + a2*y <= crit3)
        {
          incx();
          width += 2;
        }
        else if (t - a2*y > crit2)
        {
          BW::drawScanLine(posx - x, posy - y, width, color);
          if (y != 0)
	          BW::drawScanLine(posx - x, posy + y, width, color);
          incy();
        }
        else
        {
          BW::drawScanLine(posx - x, posy - y, width, color);
          if (y != 0)
	          BW::drawScanLine(posx - x, posy + y, width, color);
          incx();
          incy();
          width += 2;
        }
      }
      if (yrad == 0)
        BW::drawScanLine(posx - xrad, posy, 2*xrad + 1, color);
    }
    else
    {
      int x = xrad;
      int y = 0;
      int twoAsquare = 2 * xrad * xrad;
      int twoBsquare = 2 * yrad * yrad;
      int xchange = yrad * yrad * (1 - 2 * xrad);
      int ychange = xrad * xrad;
      int ellipseerror = 0;
      int stoppingX = twoBsquare * xrad;
      int stoppingY = 0;

      while (stoppingX >= stoppingY)
      {
        BW::drawDot(posx + x, posy + y, color);
        BW::drawDot(posx - x, posy + y, color);
        BW::drawDot(posx - x, posy - y, color);
        BW::drawDot(posx + x, posy - y, color);

        y++;
        stoppingY += twoAsquare;
        ellipseerror += ychange;
        ychange += twoAsquare;
        if ((2*  ellipseerror + xchange) > 0)
        {
          x--;
          stoppingX -= twoBsquare;
          ellipseerror += xchange;
          xchange += twoBsquare;
        }
      }
      x = 0;
      y = yrad;
      xchange = yrad * yrad;
      ychange = xrad * xrad * (1 - 2 * yrad);
      ellipseerror = 0;
      stoppingX = 0;
      stoppingY = twoAsquare * yrad;
      while (stoppingX <= stoppingY)
      {
        BW::drawDot(posx + x, posy + y, color);
        BW::drawDot(posx - x, posy + y, color);
        BW::drawDot(posx - x, posy - y, color);
        BW::drawDot(posx + x, posy - y, color);

        x++;
        stoppingX += twoBsquare;
        ellipseerror += xchange;
        xchange += twoBsquare;
        if ((2*  ellipseerror + ychange) > 0)
        {
          y--;
          stoppingY -= twoAsquare;
          ellipseerror += ychange;
          ychange += twoAsquare;
        }
      }
    }
  }
};
