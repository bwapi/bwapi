#include "GameImpl.h"

#include "../Graphics.h"
#include <cassert>
#include <algorithm>
#include "../Convenience.h"

#include "../BW/Dialog.h"
#include <BWAPI/Color.h>
#include <BWAPI4/Client/Shape.h>

namespace BWAPI4
{
  //-------------------------------------------------- IN SCREEN ---------------------------------------------
  bool GameImpl::inScreen(CoordinateType::Enum ctype, int x, int y) const
  {
    if ( !data->hasGUI )
      return false;

    Position p(x,y);
    switch ( ctype )
    {
    case BWAPI4::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      p.x -= BW::BWDATA::ScreenX;
      p.y -= BW::BWDATA::ScreenY;
      break;
    case BWAPI4::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      p.x += BW::BWDATA::Mouse.x;
      p.y += BW::BWDATA::Mouse.y;
      break;
    }
    if (p.x < 0 || 
        p.y < 0 ||
        p.x > BW::BWDATA::GameScreenBuffer.width() || 
        p.y > BW::BWDATA::GameScreenBuffer.height())
      return false;
    return true;
  }

  bool GameImpl::inScreen(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2) const
  {
    if ( !data->hasGUI )
      return false;

    Position p1(x1,y1), p2(x2,y2);
    switch ( ctype )
    {
    case BWAPI4::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      {
        int maxW = BroodwarImpl.mapWidth()*32;
        int maxH = BroodwarImpl.mapHeight()*32;
        if ( (x1 < 0 && x2 < 0)         ||
             (y1 < 0 && y2 < 0)         ||
             (x1 >= maxW && x2 >= maxW) ||
             (y1 >= maxH && y2 >= maxH) )
          return false;

        p1.x -= BW::BWDATA::ScreenX;
        p1.y -= BW::BWDATA::ScreenY;
        p2.x -= BW::BWDATA::ScreenX;
        p2.y -= BW::BWDATA::ScreenY;
        break;
      }
    case BWAPI4::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      p1.x += BW::BWDATA::Mouse.x;
      p1.y += BW::BWDATA::Mouse.y;
      p2.x += BW::BWDATA::Mouse.x;
      p2.y += BW::BWDATA::Mouse.y;
      break;
    }
    rect scrLimit = { 0, 0, static_cast<s16>(BW::BWDATA::GameScreenBuffer.width()), static_cast<s16>(BW::BWDATA::GameScreenBuffer.height()) };
    if ((p1.x < 0 && p2.x < 0) ||
        (p1.y < 0 && p2.y < 0) ||
        (p1.x > scrLimit.right  && p2.x > scrLimit.right) ||
        (p1.y > scrLimit.bottom && p2.y > scrLimit.bottom))
      return false;
    return true;
  }

  bool GameImpl::inScreen(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int x3, int y3) const
  {
    if ( !data->hasGUI )
      return false;

    Position p1(x1,y1), p2(x2,y2), p3(x3,y3);
    switch ( ctype )
    {
    case BWAPI4::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      {
        int maxW = BroodwarImpl.mapWidth()*32;
        int maxH = BroodwarImpl.mapHeight()*32;
        if ( (x1 < 0 && x2 < 0 && x3 < 0)             ||
             (y1 < 0 && y2 < 0 && y3 < 0)             ||
             (x1 >= maxW && x2 >= maxW && x3 >= maxW) ||
             (y1 >= maxH && y2 >= maxH && y3 >= maxH) )
          return false;
        p1.x -= BW::BWDATA::ScreenX;
        p1.y -= BW::BWDATA::ScreenY;
        p2.x -= BW::BWDATA::ScreenX;
        p2.y -= BW::BWDATA::ScreenY;
        p3.x -= BW::BWDATA::ScreenX;
        p3.y -= BW::BWDATA::ScreenY;
        break;
      }
    case BWAPI4::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      p1.x += BW::BWDATA::Mouse.x;
      p1.y += BW::BWDATA::Mouse.y;
      p2.x += BW::BWDATA::Mouse.x;
      p2.y += BW::BWDATA::Mouse.y;
      p3.x += BW::BWDATA::Mouse.x;
      p3.y += BW::BWDATA::Mouse.y;
      break;
    }
    rect scrLimit = { 0, 0, static_cast<s16>(BW::BWDATA::GameScreenBuffer.width()), static_cast<s16>(BW::BWDATA::GameScreenBuffer.height()) };
    if ((p1.x < 0 && p2.x < 0 && p3.x < 0) ||
        (p1.y < 0 && p2.y < 0 && p3.y < 0) ||
        (p1.x > scrLimit.right && p2.x > scrLimit.right && p3.x > scrLimit.right) ||
        (p1.y > scrLimit.bottom && p2.y > scrLimit.bottom && p3.y > scrLimit.bottom))
      return false;
    return true;
  }
  void GameImpl::addShape(const BWAPIC4::Shape &s)
  {
    this->shapes.emplace_back(s);
  }
  //-------------------------------------------------- DRAW TEXT ---------------------------------------------
  void GameImpl::setTextSize(Text::Size::Enum size)
  {
    this->textSize = std::clamp(size, Text::Size::Small, Text::Size::Huge);
  }
  void GameImpl::vDrawText(CoordinateType::Enum ctype, int x, int y, const char *format, va_list arg)
  {
    if ( !data->hasGUI ) return;
    char buffer[2048];
    VSNPrintf(buffer, format, arg);
    BWAPIC4::Shape s(BWAPI::ShapeType::Text, ctype, x, y, 0, 0, 0, textSize, 0, false, buffer);
    addShape(s);
  }
  //--------------------------------------------------- DRAW BOX ---------------------------------------------
  void GameImpl::drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    if (!inScreen(ctype,left,top,right,bottom)) return;
    addShape(BWAPIC4::Shape(BWAPI::ShapeType::Box,ctype,left,top,right,bottom,0,0,color,isSolid));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void GameImpl::drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy)) return;
    addShape(BWAPIC4::Shape(BWAPI::ShapeType::Triangle,ctype,ax,ay,bx,by,cx,cy,color,isSolid));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void GameImpl::drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(BWAPIC4::Shape(BWAPI::ShapeType::Circle,ctype,x,y,0,0,radius,0,color,isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void GameImpl::drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(BWAPIC4::Shape(BWAPI::ShapeType::Ellipse,ctype,x,y,0,0,xrad,yrad,color,isSolid));
  }
  //--------------------------------------------------- DRAW DOT ---------------------------------------------
  void GameImpl::drawDot(CoordinateType::Enum ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y)) return;
    addShape(BWAPIC4::Shape(BWAPI::ShapeType::Dot,ctype,x,y,0,0,0,0,color,false));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void GameImpl::drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2)) return;
    addShape(BWAPIC4::Shape(BWAPI::ShapeType::Line,ctype,x1,y1,x2,y2,0,0,color,false));
  }
  //--------------------------------------------------- HAS GUI ----------------------------------------------
  bool GameImpl::isGUIEnabled() const
  {
    return data->hasGUI;
  }
  //--------------------------------------------------- SET GUI ----------------------------------------------
  void GameImpl::setGUI(bool enabled)
  {
    data->hasGUI = enabled;
    setFrameSkip(enabled ? -1 : 9999999);
  }
  // Fixed precision conversions
  // Uses * and / for compatibility with negative numbers
  #define fixed2Int( x ) ( ((x) + (1 << 15)) / (1 << 16) )
  #define int2Fixed( x ) ( (x) * (1 << 16) )
  #define positive( x ) ( (x) > 1 ? (x) : 1 )
  //--------------------------------------------- DRAW SHAPES ------------------------------------------------
  bool GameImpl::drawShapes()
  {
    for (BWAPIC4::Shape& shape : this->shapes)
    {
      BWAPI::ShapeType::Enum s = shape.type;
      int x1 = shape.x1;
      int y1 = shape.y1;
      int x2, y2, w, h;
      int radius, f, ddF_x, ddF_y, xi, yi;
      int xrad, yrad;
      CoordinateType::Enum ctype = shape.ctype;
      bool isSolid = shape.isSolid;
      BWAPI4::Color color = Color(shape.color);
      switch ( s )
      {
        case BWAPI::ShapeType::Text:
           bwDrawText(x1, y1, shape.text.c_str(), ctype, (char)shape.extra2);
           break;
        case BWAPI::ShapeType::Box:
          x2 = shape.x2;
          y2 = shape.y2;
          w = abs(x2 - x1);
          h = abs(y2 - y1);
          if (isSolid)
          {
            bwDrawBox(x1, y1, w, h, color, ctype);
          }
          else
          {
            bwDrawBox(  x1,   y1, 1, h, color, ctype);
            bwDrawBox(  x1,   y1, w, 1, color, ctype);
            bwDrawBox(x2-1,   y1, 1, h, color, ctype);
            bwDrawBox(  x1, y2-1, w, 1, color, ctype);
          }
          break;
        case BWAPI::ShapeType::Triangle:
        {
          x2 = shape.x2;
          y2 = shape.y2;
          int x3 = shape.extra1;
          int y3 = shape.extra2;
          if (isSolid)
          {
            int ly, ry, lx, rx;
            if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
            if (y1 > y3) { std::swap(x1, x3); std::swap(y1, y3); }
            if (y2 > y3) { std::swap(x2, x3); std::swap(y2, y3); }

            int dx1 = int2Fixed(x2 - x1);
            int dx2 = int2Fixed(x3 - x1);
            int dx3 = int2Fixed(x3 - x2);
            if (y2 - y1 > 0) dx1 /= (y2 - y1);
            if (y3 - y1 > 0) dx2 /= (y3 - y1);
            if (y3 - y2 > 0) dx3 /= (y3 - y2);

            rx = lx = int2Fixed(x1);
            ry = ly = y1;
            if (dx1 > dx2)
            {
              for (; ly <= y2; ly++, ry++, lx += dx2, rx += dx1)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
              rx = int2Fixed(x2); ry = y2;
              for (; ly <= y3; ly++, ry++, lx += dx2, rx += dx3)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
            }
            else
            {
              for (; ly <= y2; ly++, ry++, lx += dx1, rx += dx2)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
              lx = int2Fixed(x2); ly = y2;
              for (; ly <= y3; ly++, ry++, lx += dx3, rx += dx2)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
            }
          }
          else
          {
            bwDrawLine(x1, y1, x2, y2, color, ctype);
            bwDrawLine(x1, y1, x3, y3, color, ctype);
            bwDrawLine(x2, y2, x3, y3, color, ctype);
          }
          break;
        }
        case BWAPI::ShapeType::Circle:
          radius = shape.extra1;
          if (isSolid)
          {
            f = 1 - radius;
            ddF_x = 1;
            ddF_y = -2 * radius;
            xi = 0;
            yi = radius;

            bwDrawDot(x1, y1 + radius, color, ctype);
            bwDrawDot(x1, y1 - radius, color, ctype);
            bwDrawBox(x1 - radius, y1, radius*2, 1, color, ctype);

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
              bwDrawBox(x1 - xi, y1 - yi, xi*2 + 1, 1, color, ctype);
              bwDrawBox(x1 - xi, y1 + yi, xi*2 + 1, 1, color, ctype);
              bwDrawBox(x1 - yi, y1 - xi, yi*2 + 1, 1, color, ctype);
              bwDrawBox(x1 - yi, y1 + xi, yi*2 + 1, 1, color, ctype);
            }
          }
          else
          {
            f = 1 - radius;
            ddF_x = 1;
            ddF_y = -2 * radius;
            xi = 0;
            yi = radius;

            bwDrawDot(x1, y1 + radius, color, ctype);
            bwDrawDot(x1, y1 - radius, color, ctype);
            bwDrawDot(x1 + radius, y1, color, ctype);
            bwDrawDot(x1 - radius, y1, color, ctype);

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
              bwDrawDot(x1 + xi, y1 + yi, color, ctype);
              bwDrawDot(x1 - xi, y1 + yi, color, ctype);
              bwDrawDot(x1 + xi, y1 - yi, color, ctype);
              bwDrawDot(x1 - xi, y1 - yi, color, ctype);
              bwDrawDot(x1 + yi, y1 + xi, color, ctype);
              bwDrawDot(x1 - yi, y1 + xi, color, ctype);
              bwDrawDot(x1 + yi, y1 - xi, color, ctype);
              bwDrawDot(x1 - yi, y1 - xi, color, ctype);
            }
          }
          break;
        case BWAPI::ShapeType::Ellipse:
          xrad = shape.extra1;
          yrad = shape.extra2;
          if (xrad != 0 && yrad != 0)
          {
            if (isSolid)
            {
              x2 = 0, y2 = yrad;
              w = 1;
              int a2 = xrad * xrad;
              int b2 = yrad * yrad;
              int crit1 = -(a2 / 4 + xrad % 2 + b2);
              int crit2 = -(b2 / 4 + yrad % 2 + a2);
              int crit3 = -(b2 / 4 + yrad % 2);
              int t = - a2 * y2;
              int dxt = 2 * b2 * x2;
              int dyt = -2 * a2 * y2;
              int d2xt = 2 * b2;
              int d2yt = 2 * a2;

#define incx() x2++, dxt += d2xt, t += dxt
#define incy() y2--, dyt += d2yt, t += dyt

              while (y2 >= 0 && x2 <= xrad)
              {
                if (t + b2*x2 <= crit1 ||
                    t + a2*y2 <= crit3)
                {
                  incx();
                  w += 2;
                }
                else if (t - a2*y2 > crit2)
                {
                  bwDrawBox(x1 - x2, y1 - y2, w, 1, color, ctype);
                  if (y2 != 0)
                    bwDrawBox(x1 - x2, y1 + y2, w, 1, color, ctype);
                  incy();
                }
                else
                {
                  bwDrawBox(x1 - x2, y1 - y2, w, 1, color, ctype);
                  if (y2 != 0)
                    bwDrawBox(x1 - x2, y1 + y2, w, 1, color, ctype);
                  incx();
                  incy();
                  w += 2;
                }
              }
#undef incx
#undef incy
              if (yrad == 0)
                bwDrawBox(x1 - xrad, y1, 2*xrad + 1, 1, color, ctype);
            }
            else
            {
              x2 = xrad;
              y2 = 0;
              int twoAsquare = 2 * xrad * xrad;
              int twoBsquare = 2 * yrad * yrad;
              int xchange = yrad * yrad * (1 - 2 * xrad);
              int ychange = xrad * xrad;
              int ellipseerror = 0;
              int stoppingX = twoBsquare * xrad;
              int stoppingY = 0;

              while (stoppingX >= stoppingY)
              {
                bwDrawDot(x1 + x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 - y2, color, ctype);
                bwDrawDot(x1 + x2, y1 - y2, color, ctype);

                y2++;
                stoppingY += twoAsquare;
                ellipseerror += ychange;
                ychange += twoAsquare;
                if ((2 * ellipseerror + xchange) > 0)
                {
                  x2--;
                  stoppingX -= twoBsquare;
                  ellipseerror += xchange;
                  xchange += twoBsquare;
                }
              }
              x2 = 0;
              y2 = yrad;
              xchange = yrad * yrad;
              ychange = xrad * xrad * (1 - 2 * yrad);
              ellipseerror = 0;
              stoppingX = 0;
              stoppingY = twoAsquare * yrad;
              while (stoppingX <= stoppingY)
              {

                bwDrawDot(x1 + x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 - y2, color, ctype);
                bwDrawDot(x1 + x2, y1 - y2, color, ctype);

                x2++;
                stoppingX += twoBsquare;
                ellipseerror += xchange;
                xchange += twoBsquare;
                if ((2 * ellipseerror + ychange) > 0)
                {
                  y2--;
                  stoppingY -= twoAsquare;
                  ellipseerror += ychange;
                  ychange += twoAsquare;
                }
              }
            }
          }
          else if ( xrad == 0 && yrad == 0)
          {
            bwDrawDot(x1, y1, color, ctype);
          }
          else if ( xrad == 0)
          {
            bwDrawBox(x1, y1 - yrad, 1, yrad*2, color, ctype);
          }
          else //if ( yrad == 0)
          {
            bwDrawBox(x1 - xrad, y1, xrad*2, 1, color, ctype);
          }
          break;
        case BWAPI::ShapeType::Dot:
          bwDrawDot(x1, y1, color, ctype);
          break;
        case BWAPI::ShapeType::Line:
          x2 = shape.x2;
          y2 = shape.y2;
          if ( x1 == x2 && y1 == y2 )
            bwDrawDot(x1, y1, color, ctype);
          else if ( x1 == x2 )
            bwDrawBox(x1, y1 < y2 ? y1 : y2, 1, abs(y2 - y1), color, ctype);
          else if ( y1 == y2 )
            bwDrawBox(x1 < x2 ? x1 : x2, y1, abs(x2 - x1), 1, color, ctype);
          else
            bwDrawLine(x1, y1, x2, y2, color, ctype);
          break;
        default:
          break;
      }
    }
    return !shapes.empty();
  }
#undef fixed2Int
#undef int2Fixed
#undef positive
};
