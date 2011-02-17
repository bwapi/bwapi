#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include "GameImpl.h"

#include <windows.h>

#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"

#include "../../Debug.h"

namespace BWAPI
{
  //-------------------------------------------------- IN SCREEN ---------------------------------------------
  bool GameImpl::inScreen(int ctype, int x, int y)
  {
    if ( noGUI )
      return false;
    int screen_x1 = x;
    int screen_y1 = y;
    switch ( ctype )
    {
    case BWAPI::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      break;
    case BWAPI::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      break;
    }
    if (screen_x1 < 0 || 
        screen_y1 < 0 ||
        screen_x1 > BW::BWDATA_GameScreenBuffer->wid || 
        screen_y1 > BW::BWDATA_GameScreenBuffer->ht)
      return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2)
  {
    if ( noGUI )
      return false;
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    switch ( ctype )
    {
    case BWAPI::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      {
        int maxW = Broodwar->mapWidth()*32;
        int maxH = Broodwar->mapHeight()*32;
        if ( (x1 < 0 && x2 < 0)         ||
             (y1 < 0 && y2 < 0)         ||
             (x1 >= maxW && x2 >= maxW) ||
             (y1 >= maxH && y2 >= maxH) )
          return false;

        screen_x1 -= *(BW::BWDATA_ScreenX);
        screen_y1 -= *(BW::BWDATA_ScreenY);
        screen_x2 -= *(BW::BWDATA_ScreenX);
        screen_y2 -= *(BW::BWDATA_ScreenY);
        break;
      }
    case BWAPI::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      break;
    }
    BW::rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    if ((screen_x1 < 0 && screen_x2 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0) ||
        (screen_x1 > scrLimit.Xmax  && screen_x2 > scrLimit.Xmax) ||
        (screen_y1 > scrLimit.Ymax && screen_y2 > scrLimit.Ymax))
      return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3)
  {
    if ( noGUI )
      return false;
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    int screen_x3 = x3;
    int screen_y3 = y3;
    switch ( ctype )
    {
    case BWAPI::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      {
        int maxW = Broodwar->mapWidth()*32;
        int maxH = Broodwar->mapHeight()*32;
        if ( (x1 < 0 && x2 < 0 && x3 < 0)             ||
             (y1 < 0 && y2 < 0 && y3 < 0)             ||
             (x1 >= maxW && x2 >= maxW && x3 >= maxW) ||
             (y1 >= maxH && y2 >= maxH && y3 >= maxH) )
          return false;
        screen_x1 -= *(BW::BWDATA_ScreenX);
        screen_y1 -= *(BW::BWDATA_ScreenY);
        screen_x2 -= *(BW::BWDATA_ScreenX);
        screen_y2 -= *(BW::BWDATA_ScreenY);
        screen_x3 -= *(BW::BWDATA_ScreenX);
        screen_y3 -= *(BW::BWDATA_ScreenY);
        break;
      }
    case BWAPI::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      screen_x3 += BW::BWDATA_Mouse->x;
      screen_y3 += BW::BWDATA_Mouse->y;
      break;
    }
    BW::rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
        (screen_x1 > scrLimit.Xmax && screen_x2 > scrLimit.Xmax && screen_x3 > scrLimit.Xmax) ||
        (screen_y1 > scrLimit.Ymax && screen_y2 > scrLimit.Ymax && screen_y3 > scrLimit.Ymax))
      return false;
    return true;
  }
  //-------------------------------------------------- DRAW TEXT ---------------------------------------------
  void  GameImpl::setTextSize(int size)
  {
    if ( size < 1 || size > 3 )
      size = 1;
    textSize = size;
  }
  void  GameImpl::drawText(int ctype, int x, int y, const char *format, ...)
  {
    if ( noGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    this->shapes.push_back(new ShapeText(ctype,x,y,std::string(buffer),(char)textSize));
    delete buffer;
  }
  void  GameImpl::drawTextMap(int x, int y, const char *format, ...)
  {
    if ( noGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Map,x,y,std::string(buffer),(char)textSize));
    delete buffer;
  }
  void  GameImpl::drawTextMouse(int x, int y, const char *format, ...)
  {
    if ( noGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Mouse,x,y,std::string(buffer),(char)textSize));
    delete buffer;
  }
  void  GameImpl::drawTextScreen(int x, int y, const char *format, ...)
  {
    if ( noGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Screen,x,y,std::string(buffer),(char)textSize));
    delete buffer;
  }
  //--------------------------------------------------- DRAW BOX ---------------------------------------------
  void  GameImpl::drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box */
    if (!inScreen(ctype,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(ctype, left, top, right, bottom, color, isSolid));
  }
  void  GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the map */
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color, isSolid));
  }
  void  GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the mouse */
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color, isSolid));
  }
  void  GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the screen */
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color, isSolid));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void  GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color, isSolid));
  }
  void  GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Map, ax, ay, bx, by, cx, cy, color, isSolid));
  }
  void  GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color, isSolid));
  }
  void  GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Screen, ax, ay, bx, by, cx, cy, color, isSolid));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void  GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(ctype, x, y, radius, color, isSolid));
  }
  void  GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Map, x, y, radius, color, isSolid));
  }
  void  GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Mouse, x, y, radius, color, isSolid));
  }
  void  GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Screen, x, y, radius, color, isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void  GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(ctype, x, y, xrad, yrad, color, isSolid));
  }
  void  GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Map, x, y, xrad, yrad, color, isSolid));
  }
  void  GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Mouse, x, y, xrad, yrad, color, isSolid));
  }
  void  GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Screen, x, y, xrad, yrad, color, isSolid));
  }
  //--------------------------------------------------- DRAW DOT ---------------------------------------------
  void  GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y)) return;
    this->shapes.push_back(new ShapeDot(ctype, x, y, color));
  }
  void  GameImpl::drawDotMap(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x,y)) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Map, x, y, color));
  }
  void  GameImpl::drawDotMouse(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x,y)) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Mouse, x, y, color));
  }
  void  GameImpl::drawDotScreen(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x,y)) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Screen, x, y, color));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void  GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2)) return;
    Position begin = Position(x1, y1);
    Position end   = Position(x2, y2);
    if ( ctype == BWAPI::CoordinateType::Map )
    {
      begin.makeValid();
      end.makeValid();
    }
    this->shapes.push_back(new ShapeLine(ctype, begin.x(), begin.y(), end.x(), end.y(), color));
  }
  void  GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2)) return;
    Position begin = Position(x1, y1).makeValid();
    Position end   = Position(x2, y2).makeValid();
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Map, begin.x(), begin.y(), end.x(), end.y(), color));
  }
  void  GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2)) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Mouse, x1, y1, x2, y2, color));
  }
  void  GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2)) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Screen, x1, y1, x2, y2, color));
  }
  //------------------------------------------------ SCREEN BUFFER -------------------------------------------
  void *GameImpl::getScreenBuffer()
  {
    return BW::BWDATA_GameScreenBuffer->data;
  }
  //--------------------------------------------------- SET GUI ----------------------------------------------
  void GameImpl::setGUI(bool enabled)
  {
    noGUI = !enabled;
    if ( enabled )
      setFrameSkip();
    else
      setFrameSkip(999999); // IT'S OVER NINE THOUSAND!!11111111
  }
};