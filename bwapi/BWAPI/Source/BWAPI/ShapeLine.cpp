#include "ShapeLine.h"
#include "DLLMain.h"
#include "Engine.h"
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
  }
};
