#include "ShapeBox.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"

#include "../../Debug.h"

namespace BWAPI
{
  ShapeBox::ShapeBox(int ctype, int left, int top, int right, int bottom, int color, bool isSolid)
      : ctype(ctype)
      , left(left)
      , top(top)
      , right(right)
      , bottom(bottom)
      , color(color)
      , isSolid(isSolid)
  {
  }

  void ShapeBox::draw()
  {
    int w = right - left;
    int h = bottom - top;
    if (this->isSolid)
    {
      drawBox(left, top, w, h, color, ctype);
    }
    else
    {
      drawBox(   left,      top, 1, h, color, ctype);
      drawBox(   left,      top, w, 1, color, ctype);
      drawBox(right-1,      top, 1, h, color, ctype);
      drawBox(   left, bottom-1, w, 1, color, ctype);
    }
  }
};
