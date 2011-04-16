#include "Graphics.h"
#include "BW/Offsets.h"

#include <BWAPI/CoordinateType.h>

void drawBox(int _x, int _y, int _w, int _h, int color, int ctype)
{
  RECT box = { _x, _y, _w, _h };
  switch ( ctype )
  {
  case BWAPI::CoordinateType::Map:
    box.left -= *(BW::BWDATA_ScreenX);
    box.top  -= *(BW::BWDATA_ScreenY);
    break;
  case BWAPI::CoordinateType::Mouse:
    box.left += BW::BWDATA_Mouse->x;
    box.top  += BW::BWDATA_Mouse->y;
    break;
  }

  int right  = box.left + box.right;
  int bottom = box.top  + box.bottom;
  if (right    <= 0   ||
      bottom   <= 0   ||
      box.left >= BW::BWDATA_GameScreenBuffer->wid - 1 ||
      box.top  >= BW::BWDATA_GameScreenBuffer->ht  - 1)
    return;
  if ( right > BW::BWDATA_GameScreenBuffer->wid - 1 )
    box.right = (BW::BWDATA_GameScreenBuffer->wid - 1) - box.left;
  if ( bottom > BW::BWDATA_GameScreenBuffer->ht - 1 )
    box.bottom = (BW::BWDATA_GameScreenBuffer->ht - 1) - box.top;
  if ( box.left < 0 )
  { 
    box.right += box.left; 
    box.left  =  0;
  }
  if ( box.top < 0 )
  {
    box.bottom  += box.top;
    box.top     =  0;
  }

  u8 *data    = BW::BWDATA_GameScreenBuffer->data;
  u16 scrWid  = BW::BWDATA_GameScreenBuffer->wid;

  if ( box.right == 1 )
  {
    for ( int iy = box.top; iy < box.top + box.bottom; iy++ )
      data[iy * scrWid + box.left] = (u8)color;
  }
  else
  {
    for ( int iy = box.top; iy < box.top + box.bottom; iy++ )
      memset(&data[iy * scrWid + box.left], (u8)color, box.right);
  }
}

void drawDot(int _x, int _y, int color, int ctype)
{
  POINT pt = { _x, _y };
  switch ( ctype )
  {
  case BWAPI::CoordinateType::Map:
    pt.x -= *(BW::BWDATA_ScreenX);
    pt.y -= *(BW::BWDATA_ScreenY);
    break;
  case BWAPI::CoordinateType::Mouse:
    pt.x += BW::BWDATA_Mouse->x;
    pt.y += BW::BWDATA_Mouse->y;
    break;
  }
  if ( pt.x + 1 <= 0 ||
       pt.y + 1 <= 0 ||
       pt.x >= BW::BWDATA_GameScreenBuffer->wid - 2 ||
       pt.y >= BW::BWDATA_GameScreenBuffer->ht - 2)
    return;

  u8 *data = BW::BWDATA_GameScreenBuffer->data;
  data[pt.y * BW::BWDATA_GameScreenBuffer->wid + pt.x] = (u8)color;
}

void drawLine(int _x1, int _y1, int _x2, int _y2, int color, int ctype)
{
  POINT ptStart = { _x1, _y1 };
  POINT ptEnd   = { _x2, _y2 };
  switch ( ctype )
  {
  case BWAPI::CoordinateType::Map:
    ptStart.x -= *(BW::BWDATA_ScreenX);
    ptEnd.x   -= *(BW::BWDATA_ScreenX);
    ptStart.y -= *(BW::BWDATA_ScreenY);
    ptEnd.y   -= *(BW::BWDATA_ScreenY);
    break;
  case BWAPI::CoordinateType::Mouse:
    ptStart.x += BW::BWDATA_Mouse->x;
    ptEnd.x   += BW::BWDATA_Mouse->x;
    ptStart.y += BW::BWDATA_Mouse->y;
    ptEnd.y   += BW::BWDATA_Mouse->y;
    break;
  }
  
  int x1 = ptStart.x;
  int y1 = ptStart.y;
  int x2 = ptEnd.x;
  int y2 = ptEnd.y;
  int Dx = x2 - x1;
  int Dy = y2 - y1;
  bool steep = abs(Dy) >= abs(Dx);
  if ( steep )
  {
    std::swap<int>(x1, y1);
    std::swap<int>(x2, y2);
    // recompute Dx, Dy after swap
    Dx = x2 - x1;
    Dy = y2 - y1;
  }
  int xstep = 1;
  if ( Dx < 0 )
  {
    xstep = -1;
    Dx = -Dx;
  }
  int ystep = 1;
  if ( Dy < 0 )
  {
    ystep = -1;
    Dy = -Dy;
  }
  int TwoDy = Dy << 1; // 2*Dy
  int TwoDyTwoDx = TwoDy - (Dx << 1); // 2*Dy - 2*Dx
  int E = TwoDy - Dx; //2*Dy - Dx
  int y = y1;
  int xDraw, yDraw;
  u8 *data    = BW::BWDATA_GameScreenBuffer->data;
  u16 scrWid  = BW::BWDATA_GameScreenBuffer->wid;
  u16 scrHgt  = BW::BWDATA_GameScreenBuffer->ht;
  for ( int x = x1; x != x2; x += xstep )
  {
    if (steep)
    {
      xDraw = y;
      yDraw = x;
    }
    else
    {
      xDraw = x;
      yDraw = y;
    }
    // plot
    if ( xDraw + 1 > 0 &&
         yDraw + 1 > 0 &&
         xDraw < scrWid - 2 &&
         yDraw < scrHgt - 2)
           data[yDraw * scrWid + xDraw] = (u8)color;
         
    // next
    if (E > 0)
    {
      E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
      y += ystep;
    }
    else
    {
      E += TwoDy; //E += 2*Dy;
    }
  }
}

void drawText(int _x, int _y, const char* ptext, int ctype, char size)
{
  POINT pt = { _x, _y };
  switch ( ctype )
  {
  case BWAPI::CoordinateType::Map:
    pt.x -= *(BW::BWDATA_ScreenX);
    pt.y -= *(BW::BWDATA_ScreenY);
    break;
  case BWAPI::CoordinateType::Mouse:
    pt.x += BW::BWDATA_Mouse->x;
    pt.y += BW::BWDATA_Mouse->y;
    break;
  }
  if (pt.x + BW::GetTextWidth(ptext, size)  < 0 || 
      pt.y + BW::GetTextHeight(ptext, size) < 0 || 
      pt.x > BW::BWDATA_GameScreenBuffer->wid   || 
      pt.y > BW::BWDATA_GameScreenBuffer->ht)
    return;

  BW::BlitText(ptext, BW::BWDATA_GameScreenBuffer, pt.x, pt.y, size);
}
