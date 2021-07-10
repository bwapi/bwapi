#include "Bitmap.h"
#include <storm.h>
#include <algorithm>

#include "Font.h"
#include "CImage.h"
#include "Offsets.h"

namespace BW
{
  Bitmap::Bitmap(int width, int height, void *pBuffer)
    : wid(static_cast<u16>(width))
    , ht(static_cast<u16>(height))
    , data(static_cast<u8*>(pBuffer != nullptr ? pBuffer : SMAlloc(width*height)))
  {}

  bool Bitmap::isValid() const
  {
    return this && this->data && this->wid > 0 && this->ht > 0;
  }

  u8 *Bitmap::getData() const
  {
    if ( this )
      return this->data;
    return nullptr;
  }

  int Bitmap::width() const
  {
    if ( this )
      return this->wid;
    return 0;
  }

  int Bitmap::height() const
  {
    if ( this )
      return this->ht;
    return 0;
  }

  BYTE gbColorTable[] = { 
     0,  0,  0,  1,  2,  3,  4,  5,  8,  0,  0,  0,  0,  0,  9, 10, 
    11, 12,  0,  0,  0, 13, 14, 15, 16, 17,  0, 18, 19, 20, 21, 23
  };

  BYTE gbFontColors[24][8] = {
    { 0xC0, 0x9B, 0x9A, 0x95, 0x43, 0x00, 0x00, 0x28 }, 
    { 0x56, 0xA7, 0x6D, 0x65, 0x5C, 0x00, 0x00, 0x8A }, 
    { 0x41, 0xFF, 0x53, 0x97, 0x47, 0x00, 0x00, 0x8A }, 
    { 0x40, 0x96, 0x49, 0x90, 0x42, 0x00, 0x00, 0x8A }, 
    { 0xA8, 0xAE, 0x17, 0x5E, 0xAA, 0x00, 0x00, 0x8A }, 
    { 0xB5, 0x75, 0xBA, 0xB9, 0xB7, 0x00, 0x00, 0x8A }, 
    { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 }, 
    { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 }, 
    { 0x8A, 0x6F, 0x17, 0x5E, 0xAA, 0x8A, 0x8A, 0x8A }, 
    { 0x28, 0xA5, 0xA2, 0x2D, 0xA0, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x9F, 0x9E, 0x9D, 0xB7, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0xA4, 0xA3, 0xA1, 0x0E, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x9C, 0x1C, 0x1A, 0x13, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x13, 0x12, 0x11, 0x57, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x54, 0x51, 0x4E, 0x4A, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x87, 0xA6, 0x81, 0x93, 0x8A, 0x8A, 0x8A }, 
    { 0xB5, 0xB9, 0xB8, 0xB7, 0xB6, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x88, 0x84, 0x81, 0x60, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x86, 0x72, 0x70, 0x69, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x33, 0x7C, 0x7A, 0xA0, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x4D, 0x26, 0x23, 0x22, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x9A, 0x97, 0x95, 0x91, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x88, 0x84, 0x81, 0x60, 0x8A, 0x8A, 0x8A }, 
    { 0x8A, 0x80, 0x34, 0x31, 0x2E, 0x8A, 0x8A, 0x8A }
  };

  bool Bitmap::blitString(const char *pszStr, int x, int y, u8 bSize)
  {
    // verify valid size index
    if ( bSize > 3 || !pszStr )
      return false;

    // localize pointer (TODO: get rid of this offset)
    Font *fnt = BWDATA::FontBase[bSize];
    if ( !fnt->isValid() )
      return false;

    // verify if drawing should be done
    if (  x + fnt->getTextWidth(pszStr)   < 0 ||
          y + fnt->getTextHeight(pszStr)  < 0 ||
          x >= this->width() ||
          y >= this->height() )
      return false;

    // Reference an unsigned character array
    const u8 *pbChars = (BYTE*)pszStr;

    u8 lastColor = 0, color   = 0;
    int  Xoffset   = x, Yoffset = y;

    // Iterate all characters in the message
    for ( int c = 0; pbChars[c]; ++c )
    {
      // Perform control character and whitespace functions
      if ( pbChars[c] <= ' ' )
      {
        switch ( pbChars[c] )
        {
        case 1:       // restore last colour
          color = lastColor;
          continue;
        case '\t':    // 9    tab
          Xoffset += fnt->getCharWidth( pbChars[c] );
          continue;
        case '\n':    // 10   newline
          Xoffset = x;
          Yoffset += fnt->maxHeight();
          continue;
        case 11:      // invisible
        case 20:
          color = (BYTE)~0;
          continue;
        case '\f':    // 12   formfeed
          break;
        case '\r':    // 13   carriage return
        case 26:
          continue;
        case 18:      // right align
          Xoffset += this->width() - fnt->getTextWidth(pszStr) - x;
          continue;
        case 19:      // center align
          Xoffset += (this->width() - fnt->getTextWidth(pszStr))/2 - x;
          continue;
        case ' ':     // space
          Xoffset += fnt->maxWidth() / 2;
          continue;
        default:      // colour code
          lastColor = color;
          color     = gbColorTable[ pbChars[c] ];
          continue;
        }
      }

      // Skip if the character is not supported by the font
      FontChar *fntChr = fnt->getChar( pbChars[c] );
      if ( !fntChr->isValid() )
        continue;

      // If the colour is not "invisible"
      if ( color != ~0 )
      {
        // begin drawing character process
        for ( int i = 0, pos = 0; pos < fntChr->height() * fntChr->width(); ++i, ++pos )
        {
          // font position
          pos += fntChr->pixelOffset(i);

          // x offset
          int newX = Xoffset + (fntChr->x() + pos % fntChr->width());
          if ( newX >= this->width() ) break;
          if ( newX < 0 ) continue;

          // y offset
          int newY = Yoffset + (fntChr->y() + pos / fntChr->width());
          if ( newY >= this->height() ) break;
          if ( newY < 0 ) continue;

          // blit offset
          int offset = newY * this->width() + newX;
          if ( offset >= this->width() * this->height() ) break;
          if ( offset < 0 ) continue;

          // Plot pixel
          this->data[offset] = gbFontColors[color][fntChr->colorMask(i)];
        }
      } // invis colour

      // Increment the X offset for the width of the character
      Xoffset += fntChr->width();
    }
    return true;
  }

  void Bitmap::plot(int x, int y, u8 color)
  {
    if ( x < 0 || y < 0 || x >= this->width() || y >= this->height() )
      return;
    this->plotX(x, y, color);
  }
  void Bitmap::plotX(int x, int y, u8 col)
  {
    this->data[ x + y*this->width() ] = col;
  }

  void Bitmap::drawLine(int x1, int y1, int x2, int y2, u8 color)
  {
    // If the line doesn't intersect this rect
    if (  std::max(x1,x2) < 0 ||
          std::max(y1,y2) < 0 ||
          std::min(x1,x2) >= this->width() ||
          std::min(y1,y2) >= this->height()  )
      return;

    // If the line is completely vertical or horizontal
    if ( x1 == x2 )
    {
      int ymin = std::clamp(std::min(y1,y2), 0, this->height()-1);
      int ymax = std::clamp(std::max(y1,y2), 0, this->height()-1);

      // Plot vertical line
      for ( int i = ymin; i < ymax; ++i )
        this->plotX(x1, i, color);
      return;
    }
    else if ( y1 == y2 )
    {
      int xmin = std::clamp(std::min(x1,x2), 0, this->width()-1);
      int xmax = std::clamp(std::max(x1,x2), 0, this->width()-1);

      // Plot horizontal line
      memset(&this->data[y1 * this->width() + xmin], color, xmax - xmin );
      return;
    }

    // If the line does intersect but needs to be clipped
    //if ( x1 < 0 || y1 < 0 || x2 >= this->width() || y2 >= this->height() )
      //this->clipping(x1,y1,x2,y2);

    // Line algorithm source: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification
    int dx = std::abs(x2 - x1),     dy = std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1,      sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    // @TODO: optimize
    while ( this->plot(x1, y1, color), x1 != x2 || y1 != y2 )
    {
      int e2 = 2*err;
      if ( e2 > -dy )
      {
        err -= dy;
        x1 += sx;
      }
      if ( e2 < dx )
      {
        err += dx;
        y1 += sy;
      }
    }
  }

  void Bitmap::clear()
  {
    if ( this->isValid() )
      memset(this->data, 0, this->width()*this->height());
  }
}


