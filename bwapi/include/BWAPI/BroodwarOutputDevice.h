#pragma once

#if 0
#include <BWAPI/Game.h>
#include <sstream>
#include <boost/circular_buffer.hpp>
#include <boost/iostreams/concepts.hpp>

namespace BWAPI
{
  ///<summary>Output device for writing text to the screen as a notification.</summary>
  /// Normally when you write stuff in Starcraft, the text wraps at word boundaries when it would
  /// otherwise exceed the screen width. When this happens any Text::Enum color in the message is lost and
  /// the next line will be some default color. To fix this color problem this device keeps track of
  /// the width of the text in the buffer and manually wraps when needed (not on word boundaries), and
  /// then colors the next line with the current color again before continuing to write.
  /// @since 4.2.0
  class BroodwarOutputDevice : public boost::iostreams::sink
  {
    static const int MAX_WIDTH = 620; //620 is the max width before starcraft wraps
    static const int DEFAULT_FONT_CHAR_PRINTING_WIDTHS[];
    static const int DEFAULT_FONT_MAX_WIDTH = 10;

    int bufferWidth = 0;
    std::stringbuf buffer;
    const Text::Enum defaultColor;
    boost::circular_buffer<Text::Enum> previousColors;

  public:
    explicit BroodwarOutputDevice(Text::Enum defaultColor = Text::Yellow)
      : defaultColor(Text::isColor(defaultColor) ? defaultColor : Text::Yellow)
      , previousColors(128) //arbitrary stack size
    {
      buffer.sputc(char(defaultColor));
      previousColors.push_back(defaultColor);
    }

    BroodwarOutputDevice(const BroodwarOutputDevice& o)
      : BroodwarOutputDevice(o.defaultColor)
    {}

    std::streamsize write(const char* s, std::streamsize n)
    {
      for (int j = 0; j < n; j++)
      {
        char c = s[j];

        //push/pop colors
        if (Text::isColor(Text::Enum(c)))
        {
          previousColors.push_back(Text::Enum(c));
        }
        else if (c == Text::Previous)
        {
          previousColors.pop_back();
          if (previousColors.empty()) //always keep a color on the stack
            previousColors.push_back(defaultColor);
          c = char(previousColors.back());
        }

        if (c == '\n' || bufferWidth + charPrintingWidth(c) > MAX_WIDTH)
        {
          BroodwarPtr->printf("%s", buffer.str().c_str());
          buffer.str("");
          buffer.sputc(char(previousColors.back()));
          bufferWidth = 0;
        }

        if (c != '\n')
        {
          buffer.sputc(c);
          bufferWidth += charPrintingWidth(c);
        }
      }
      return n;
    }

  private:
    int charPrintingWidth(unsigned char c) const
    {
      if (c == '\t')
        return DEFAULT_FONT_MAX_WIDTH * 2 - bufferWidth % (DEFAULT_FONT_MAX_WIDTH * 2);
      else
        return DEFAULT_FONT_CHAR_PRINTING_WIDTHS[c];
    }
  };
}

#endif // 0
