#include "ShapeBox.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeBox::ShapeBox(int coordinateType, int left, int top, int right, int bottom, int color, bool isSolid)
  :coordinateType(coordinateType)
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
      drawBox(left,top,right-left,bottom-top,color,coordinateType);
    }
    else
    {
      int lineWidth=1;
      drawBox(left,top,lineWidth,bottom-top,color,coordinateType);
      drawBox(left,top,right-left,lineWidth,color,coordinateType);
      drawBox(right-lineWidth,top,lineWidth,bottom-top,color,coordinateType);
      drawBox(left,bottom-lineWidth,right-left,lineWidth,color,coordinateType);
    }
  }
}