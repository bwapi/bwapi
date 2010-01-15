#include "Drawing.h"

#include <BW\Hook.h>
#include <BW\Broodwar.h>

#include <BWAPI\Shape.h>

#include <vector>

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
    void pushLine(Position from, Position to, int color)
    {
      ShapeLine line;
      line.from = from;
      line.to = to;
      line.color = color;
      lineShapes.push_back(line);
    }
    //----------------------------------- PUSH TEXT ------------------------------------
    void pushText(Position pos, std::string str)
    {
      ShapeText text;
      text.pos = pos;
      text.text = str;
      textShapes.push_back(text);
    }
    //----------------------------------- PUSH MAP LINE --------------------------------
    void pushMapLine(Position from, Position to, int color)
    {
      ShapeLine line;
      line.from = from - Position(BW::getScreenPos());
      line.to = to - Position(BW::getScreenPos());
      line.color = color;
      lineShapes.push_back(line);
    }
    //----------------------------------- PUSH MAP TEXT --------------------------------
    void pushMapText(Position pos, std::string str)
    {
      ShapeText text;
      text.pos = pos - Position(BW::getScreenPos());
      text.text = str;
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
