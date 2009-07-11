#include "ShapeTriangle.h"
#include "ShapeLine.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
  ShapeTriangle::ShapeTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, int color, bool isSolid)
  :ctype(ctype)
  ,ax(ax)
  ,ay(ay)
  ,bx(bx)
  ,by(by)
  ,cx(cx)
  ,cy(cy)
  ,color(color)
  ,isSolid(isSolid)
  {
  }

  void ShapeTriangle::swap(int &a0, int &a1)
  {
    int tmpx = a0;
    a0 = a1;
    a1 = tmpx;
  }
  
  int ShapeTriangle::rnd(float x)
  {
    return (x >= 0 ? ((int)( x + 0.5 )) : 0) ;
  }

  void ShapeTriangle::draw()
  {
    if(isSolid)
    {
      if(ay > by) { swap(ax,bx); swap(ay,by); }
      if(ay > cy) { swap(ax,cx); swap(ay,cy); }
      if(by > cy) { swap(bx,cx); swap(by,cy); }
    
      float dx1, dx2, dx3;
      if (by-ay > 0) dx1=(float)(bx-ax)/(by-ay); else dx1=(float)bx - ax;
	    if (cy-ay > 0) dx2=(float)(cx-ax)/(cy-ay); else dx2=0;
	    if (cy-by > 0) dx3=(float)(cx-bx)/(cy-by); else dx3=0;
  	  
	    float lx = (float)ax, rx = lx;
	    int ly = ay, ry = ly;
	    if(dx1 > dx2) {
		    for(;ly<=by;ly++,ry++,lx+=dx2,rx+=dx1)
		      drawBox(rnd(lx),ly,rnd(rx-lx),1,color,ctype);
		    rx = (float)bx; ry = by;
		    for(;ly<=cy;ly++,ry++,lx+=dx2,rx+=dx3)
			    drawBox(rnd(lx),ly,rnd(rx-lx),1,color,ctype);
	    } else {
		    for(;ly<=by;ly++,ry++,lx+=dx1,rx+=dx2)
			    drawBox(rnd(lx),ly,rnd(rx-lx),1,color,ctype);
		    lx = (float)bx; ly = by;
		    for(;ly<=cy;ly++,ry++,lx+=dx3,rx+=dx2)
			    drawBox(rnd(lx),ly,rnd(rx-lx),1,color,ctype);
	    }
	  }
	  else
	  {
	    new ShapeLine(ctype,ax,ay,bx,by,color);
	    new ShapeLine(ctype,ax,ay,cx,cy,color);
	    new ShapeLine(ctype,bx,by,cx,cy,color);
	  }
  }
}