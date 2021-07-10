#include "Font.h"
#include "Offsets.h"

namespace BW
{
  bool Font::isValid() const
  {
    return this != nullptr;
  }

  FontChar *Font::getChar(char c) const
  {
    u8 ch = static_cast<u8>(c);
    if (  this->isValid() &&
          this->low <= ch && ch <= this->high &&
          this->chrs[ch - this->low] != reinterpret_cast<const FontChar*>(this) )
      return this->chrs[ch - this->low];
    return nullptr;
  }

  int Font::getCharWidth(char c) const
  {
    switch ( c )
    {
    case '\t':
      return this->maxWidth() * 2;
    case ' ':
      return this->maxWidth() / 2;
    default:
      return this->getChar(c)->width();
    }
  }
  // ----------------- GET WIDTH ---------------------
  int Font::getTextWidth(const char *pszStr) const
  {
    // verify valid size index
    if ( !pszStr || !this->isValid() )
      return 0;

    // Retrieve size
    int size = 0;
    for ( ; *pszStr; ++pszStr )
      size += this->getCharWidth(*pszStr);

    return size;
  }
  // ----------------- GET WIDTH ---------------------
  int Font::getTextHeight(const char *pszStr) const
  {
    // verify valid size index
    if ( !pszStr || !this->isValid() )
      return 0;

    int size = this->Ymax;
    for ( ; *pszStr; ++pszStr )
    {
      if ( *pszStr == '\n' )
        size += this->Ymax;
    }
    return size;
  }

  int Font::maxWidth() const
  {
    if ( this->isValid() )
      return this->Xmax;
    return 0;
  }
  int Font::maxHeight() const
  {
    if ( this->isValid() )
      return this->Ymax;
    return 0;
  }
  int Font::getTextWidth(const char *pszStr, int size)
  {
    if ( size < 0 || size > 3 )
      return 0;
    // TODO: Get rid of this offset
    return BW::BWDATA::FontBase[size]->getTextWidth(pszStr);
  }
  int Font::getTextHeight(const char *pszStr, int size)
  {
    if ( size < 0 || size > 3 )
      return 0;
    // TODO: Get rid of this offset
    return BW::BWDATA::FontBase[size]->getTextHeight(pszStr);
  }
  // ------------------------------------------------------------------------------ FontChar ------
  bool FontChar::isValid() const
  {
    return this != nullptr;
  }
  int FontChar::width() const
  {
    if ( this->isValid() )
      return this->w;
    return 0;
  }
  int FontChar::height() const
  {
    if ( this->isValid() )
      return this->h;
    return 0;
  }
  int FontChar::x() const
  {
    if ( this->isValid() )
      return this->_x;
    return 0;
  }
  int FontChar::y() const
  {
    if ( this->isValid() )
      return this->_y;
    return 0;
  }

  int FontChar::colorMask(int index) const
  {
    return this->data[index] & 7;
  }
  int FontChar::pixelOffset(int index) const
  {
    return this->data[index] >> 3;
  }
}
