#include "Tracer.h"

#include <BW\Hook.h>

#include <BWAPI\Shape.h>

namespace BWAPI
{
  namespace Tracer
  {
//private:
    // state
    bool active = false;

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
    //----------------------------------- ----------------------------------------------
//public:
    //----------------------------------- IS ACTIVE ------------------------------------
    void deactivate()
    {
      active = false;
    }
    //----------------------------------- COMMAND --------------------------------------
    bool command(const std::vector<std::string>& parsed)
    {
      if(parsed[1] == "on")
      {
        active = true;
        return true;
      }
      if(parsed[1] == "off")
      {
        active = false;
        return true;
      }
      return false;
    }
    //----------------------------------- ON FRAME -------------------------------------
    void onMatchFrame()
    {
      if(!active)
        return;
      pushText(Position(10, 10), "tracer is on");
    }
    //----------------------------------- ON DRAW --------------------------------------
    void onDraw()
    {
      if(!active)
        return;
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
