#include "Drawing.h"

#include <BW\Hook.h>
#include <BW\Broodwar.h>

#include <BWAPI\Shape.h>

#include <vector>
#include <stdarg.h>

namespace BWAPI
{
  namespace Tracer
  {
  //private:
    //----------------------------------- ----------------------------------------------
    // drawing
    struct ShapeLine
    {
      BWAPI::Position from;
      BWAPI::Position to;
      int color;
    };
    struct ShapeText
    {
      BWAPI::Position pos;
      std::string text;
    };
    std::vector<ShapeLine> lineShapes;
    std::vector<ShapeText> textShapes;

    //----------------------------------- ----------------------------------------------
  //public:
    //----------------------------------- PUSH LINE ------------------------------------
    void pushLineScreen(Position from, Position to, int color)
    {
      ShapeLine line;
      line.from = from;
      line.to = to;
      line.color = color;
      lineShapes.push_back(line);
    }
    //----------------------------------- PUSH MAP LINE --------------------------------
    void pushLineMap(Position from, Position to, int color)
    {
      ShapeLine line;
      line.from = from - Position(BW::getScreenPos());
      line.to = to - Position(BW::getScreenPos());
      line.color = color;
      lineShapes.push_back(line);
    }
    //----------------------------------- PUSH TEXT ------------------------------------
    char printbuffer[1000];
    void pushTextScreen(Position pos, const char* str, ...)
    {
      va_list ap;
      va_start(ap, str);
      vsnprintf_s(printbuffer, 1000, 1000, str, ap);
      va_end(ap);
      ShapeText text;
      text.pos = pos;
      text.text = printbuffer;
      textShapes.push_back(text);
    }
    //----------------------------------- PUSH MAP TEXT --------------------------------
    void pushTextMap(Position pos, const char* str, ...)
    {
      va_list ap;
      va_start(ap, str);
      vsnprintf_s(printbuffer, 1000, 1000, str, ap);
      va_end(ap);
      ShapeText text;
      text.pos = pos - Position(BW::getScreenPos());
      text.text = printbuffer;
      textShapes.push_back(text);
    }
    //----------------------------------- REMOVE DRAWINGS ------------------------------
    void removeShapes()
    {
      lineShapes.clear();
      textShapes.clear();
    }
    //----------------------------------- ON DRAW --------------------------------------
    void drawShapes()
    {
      for each(const Tracer::ShapeText& text in Tracer::textShapes)
      {
        BW::drawText(text.pos.x, text.pos.y, text.text.c_str());
      }
      for each(const Tracer::ShapeLine& line in Tracer::lineShapes)
      {
        drawLine(line.from.x, line.from.y, line.to.x, line.to.y, line.color);
      }
    }
    //----------------------------------- ----------------------------------------------
  }
}
