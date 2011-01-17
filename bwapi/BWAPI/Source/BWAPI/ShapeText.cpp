#include "ShapeText.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"

#include "../../Debug.h"

namespace BWAPI
{
  ShapeText::ShapeText(int ctype, int x, int y, std::string text, char size)
  :ctype(ctype)
  ,x(x)
  ,y(y)
  ,text(text)
  ,size(size)
  {
  }

  void ShapeText::draw()
  {
    drawText(x,y,text.c_str(),ctype,size);
  }
};
