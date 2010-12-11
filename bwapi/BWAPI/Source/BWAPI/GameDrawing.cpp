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

namespace BWAPI
{
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
    if (!inScreen(ctype,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the map */
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the mouse */
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the screen */
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color.getID(), isSolid));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void  GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Map, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Screen, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void  GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(ctype, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Map, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Mouse, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Screen, x, y, radius, color.getID(), isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void  GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(ctype, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Map, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Mouse, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Screen, x, y, xrad, yrad, color.getID(), isSolid));
  }
  //--------------------------------------------------- DRAW DOT ---------------------------------------------
  void  GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(ctype, x, y, color.getID()));
  }
  void  GameImpl::drawDotMap(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Map, x, y, color.getID()));
  }
  void  GameImpl::drawDotMouse(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Mouse, x, y, color.getID()));
  }
  void  GameImpl::drawDotScreen(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Screen, x, y, color.getID()));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void  GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2) || noGUI) return;
    this->shapes.push_back(new ShapeLine(ctype, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2) || noGUI) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Map, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2) || noGUI) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Mouse, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2) || noGUI) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Screen, x1, y1, x2, y2, color.getID()));
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
    {
      BW::BWDATA_ScreenLayers[0].buffers = 1;
      BW::BWDATA_ScreenLayers[1].buffers = 1;
      BW::BWDATA_ScreenLayers[2].buffers = 1;
      BW::BWDATA_ScreenLayers[5].buffers = 1;
    }
    else
    {
      BW::BWDATA_ScreenLayers[0].buffers = 0;
      BW::BWDATA_ScreenLayers[1].buffers = 0;
      BW::BWDATA_ScreenLayers[2].buffers = 0;
      BW::BWDATA_ScreenLayers[5].buffers = 0;
    }
  }
};