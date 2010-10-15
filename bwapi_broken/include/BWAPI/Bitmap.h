#include <BWAPI/Color.h>
#pragma once

namespace BWAPI
{
  class BitmapProxy
  {
  public:
    BitmapProxy(unsigned char *data, unsigned short width, unsigned short height, int x);
    Color operator[](int y);
  private:
    unsigned char *data;
    unsigned short width;
    unsigned short height;
    int x;
  };

  class Bitmap
  {
    public:
      BitmapProxy operator[](int x);
      unsigned short getWidth();
      unsigned short getHeight();

    private:
      unsigned short wid;
      unsigned short ht;
      unsigned char  *data;
  };
}
