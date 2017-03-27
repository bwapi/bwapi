#if 0
#include <BWAPI/BroodwarOutputDevice.h>

namespace BWAPI
{
  // Chars 0-31 don't exist as FontChars (BW::Font.getChar(c) == nullptr) and have no width.
  // Char 32, space, doesn't exist either, but has width font.maxWidth()/2.
  // The width of a tab is dependent on the current total width of the text in the buffer,
  // and must be handled separately.
  // The other chars have a printing width of font.getChar(c)->width()+1
  // This table was generated with the following code:
  //
  //  const BW::Font& font = *BW::BWDATA::FontBase[Text::Size::Default];
  //  for (int i = 0; i < 256; i++)
  //  {
  //    if (i == ' ')
  //      std::cout << font.maxWidth()/2;
  //    else if (font.getChar(char(i)))
  //      std::cout << font.getChar(char(i))->width()+1;
  //    else
  //      std::cout << "0";
  //    std::cout << ", ";
  //    if (i % 32 == 31)
  //      std::cout << std::endl;
  //  }
  const int BroodwarOutputDevice::DEFAULT_FONT_CHAR_PRINTING_WIDTHS[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 3, 5, 7, 7, 9, 9, 3, 4, 4, 7, 7, 4, 4, 3, 5, 7, 5, 7, 7, 7, 7, 7, 7, 7, 7, 3, 4, 6, 7, 7, 7,
    10,8, 7, 8, 8, 7, 7, 9, 7, 3, 6, 8, 6, 8, 7, 9, 7, 9, 7, 8, 7, 7, 8,11, 8, 9, 7, 4, 6, 4, 7, 7,
    4, 7, 7, 6, 7, 7, 5, 7, 6, 3, 4, 6, 3, 9, 6, 7, 7, 7, 6, 7, 5, 6, 7, 9, 7, 7, 6, 5, 3, 5, 6, 9,
    4, 7, 7, 6, 7, 7, 5, 7, 6, 3, 4, 6, 3, 9, 6, 7, 4, 7, 7, 6, 7, 7, 5, 7, 6, 8, 4, 6, 3, 9, 6, 7,
    7, 3, 7, 7, 7, 7, 3, 7, 5, 9, 6, 8, 7, 5, 9, 6, 6, 5, 5, 5, 4, 7, 7, 3, 4, 4, 5, 8, 9,10,10, 6,
    8, 8, 8, 8, 8, 8,11, 8, 7, 7, 7, 7, 4, 4, 5, 5, 9, 7, 9, 9, 9, 9, 9, 8, 9, 7, 7, 7, 7, 9, 7, 7,
    7, 7, 7, 7, 7, 7,10, 6, 7, 7, 7, 7, 4, 3, 5, 5, 7, 6, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 7, 7, 7,
  };
}
#endif // 0
