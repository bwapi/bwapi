#include "ShapeText.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeText::ShapeText(int ctype, int x, int y, std::string text)
  :ctype(ctype)
  ,x(x)
  ,y(y)
  ,text(text)
  {
  }

  void ShapeText::draw()
  {
    drawText(x,y,text.c_str(),ctype);
  }
}