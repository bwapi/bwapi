#include "ShapeBox.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeBox::ShapeBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, int color, bool isSolid)
  :ctype(ctype)
  ,left(left)
  ,top(top)
  ,right(right)
  ,bottom(bottom)
  ,color(color)
  ,isSolid(isSolid)
  {
  }

  void ShapeBox::draw()
  {
    if (this->isSolid)
    {
      drawBox(left,top,right-left,bottom-top,color,ctype);
    }
    else
    {
      int lineWidth=1;
      drawBox(left,top,lineWidth,bottom-top,color,ctype);
      drawBox(left,top,right-left,lineWidth,color,ctype);
      drawBox(right-lineWidth,top,lineWidth,bottom-top,color,ctype);
      drawBox(left,bottom-lineWidth,right-left,lineWidth,color,ctype);
    }
  }
}