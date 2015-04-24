#pragma once

#include <Util/Types.h>

namespace BW
{
  struct grpFrame;
  struct grpHead;

  class Bitmap
  {
  public:
    Bitmap(int width, int height, void* pBuffer = nullptr);

    void resize(int width, int height);
    bool isValid() const;

    bool blitString(const char *pszStr, int x, int y, u8 bSize = 1);
    void blitBitmap(const Bitmap *pSrc, int x = 0, int y = 0);
    void BlitGrpFrame(const grpFrame *pGrp, int x = 0, int y = 0);
    void BlitGraphic(const grpHead *pGrp, int frame = 0, int x = 0, int y = 0);

    void plot(int x, int y, u8 color);
    void drawLine(int x1, int y1, int x2, int y2, u8 color);

    int width() const;
    int height() const;
    u8  *getData() const;

    void clear();
    
  private:
    void plotX(int x, int y, u8 col);

    void intersection(int& x1, int& y1, int x2, int y2) const;
    void clipping(int& x1, int& y1, int& x2, int& y2) const;

    u16 wid, ht;
    u8  *data;
  };



}


