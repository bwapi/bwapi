#include <BWAPI/Bitmap.h>

namespace BWAPI
{
  BitmapProxy::BitmapProxy(unsigned char *data, unsigned short width, unsigned short height, int x)
  :data(data)
  ,width(width)
  ,height(height)
  ,x(x)
  {}

  Color BitmapProxy::operator[](int y)
  {
    unsigned int offs = y * this->width + this->x;
    if ( offs < (unsigned int)(this->width * this->height) )
      return Color(this->data[offs]);
    return Color(0);
  }

  BitmapProxy Bitmap::operator[](int x)
  {
    return BitmapProxy(this->data, this->wid, this->ht, x);
  }

  unsigned short Bitmap::getWidth()
  {
    return this->wid;
  }

  unsigned short Bitmap::getHeight()
  {
    return this->ht;
  }
}

