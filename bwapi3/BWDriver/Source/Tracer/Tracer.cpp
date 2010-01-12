#include "Tracer.h"

#include <BW\Hook.h>
#include <BW\Broodwar.h>

#include <BWAPI\Shape.h>

#include <Util\Strings.h>

namespace BWAPI
{
  namespace Tracer
  {
//private:
    // state
    bool active = false;
    int wordWidth = 4;
    bool showUnits = true;

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
    void removeDrawings()
    {
      lineShapes.clear();
      textShapes.clear();
    }
    //----------------------------------- ----------------------------------------------
//public:
    //----------------------------------- DEASTIVATE -----------------------------------
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
      if(parsed[1] == "byte")
      {
        wordWidth = 1;
        return true;
      }
      if(parsed[1] == "word")
      {
        wordWidth = 2;
        return true;
      }
      if(parsed[1] == "dword")
      {
        wordWidth = 4;
        return true;
      }
      if(parsed[1] == "units")
      {
        if(parsed[2] == "show")
        {
          showUnits = true;
          return true;
        }
        if(parsed[2] == "hide")
        {
          showUnits = false;
          return true;
        }
        return false;
      }
      return false;
    }
    //----------------------------------- ON FRAME -------------------------------------
    void onMatchFrame()
    {
      if(!active)
        return;
      removeDrawings();

      // tracer code

      pushText(Position(10, 10), "Tracer on");

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
