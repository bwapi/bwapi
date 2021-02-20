#pragma once

#include "../Types.h"

namespace BW
{
  // cppcheck-suppress noConstructor
  class FontChar
  {
  public:
    bool isValid() const;

    int width() const;
    int height() const;
    int x() const;
    int y() const;

    int colorMask(int index) const;
    int pixelOffset(int index) const;

  private:
    u8  w, h, _x, _y;
    u8  data[1];
  };

  class Font
  {
  public:
    bool  isValid() const;
    
    FontChar  *getChar(char c) const;

    int   getTextWidth(const char *pszStr) const;
    int   getTextHeight(const char *pszStr) const;

    int   getCharWidth(char c) const;

    int   maxWidth() const;
    int   maxHeight() const;

    static int getTextWidth(const char *pszStr, int size);
    static int getTextHeight(const char *pszStr, int size);
  private:
    Font() = delete;
    Font(const Font &other) = delete;
    Font(Font &&other) = delete;
    ~Font() = delete;

    u32       magic;
    u8        low, high;
    u8        Xmax, Ymax;
    FontChar  *chrs[1];
  };

}
