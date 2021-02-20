#pragma once

#include "../Types.h"

namespace BW
{
  struct grpFrame;
  struct grpHead;

  class Bitmap
  {
  public:
    Bitmap(int width, int height, void* pBuffer = nullptr);

    bool isValid() const;

    bool blitString(const char *pszStr, int x, int y, u8 bSize = 1);

    void plot(int x, int y, u8 color);
    void drawLine(int x1, int y1, int x2, int y2, u8 color);

    int width() const;
    int height() const;
    u8  *getData() const;

    void clear();
    
  private:
    void plotX(int x, int y, u8 col);

    u16 wid, ht;
    u8  *data;
  };



}


