#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// The Color object is used in drawing routines to specify the color to use.
  ///
  /// @note Starcraft uses a 256 color palette for rendering. Thus, the colors available are
  /// limited to this palette.
  class Color : public Type<Color, 255>
  {
    public:
      /// A constructor that uses the color at the specified palette index.
      ///
      /// @param id 
      ///   The index of the color in the 256-color palette.
      Color(int id = 0);

      /// A constructor that uses the color index in the palette that is closest to the given rgb
      /// values. On its first call, the colors in the palette will be sorted for fast indexing.
      ///
      /// @note This function computes the distance of the RGB values and may not be accurate.
      ///
      /// @param red
      ///   The amount of red.
      /// @param green
      ///   The amount of green.
      /// @param blue
      ///   The amount of blue.
      Color(int red, int green, int blue);

      /// Retrieves the red component of the color.
      ///
      /// @returns integer containing the value of the red component.
      int red() const;

      /// Retrieves the green component of the color.
      ///
      /// @returns integer containing the value of the green component.
      int green() const;

      /// Retrieves the blue component of the color.
      ///
      /// @returns integer containing the value of the blue component.
      int blue() const;
  };

  /// Namespace containing known colors.
  namespace Colors
  {
    /// The default color for Player 1.
    extern const Color Red;

    /// The default color for Player 2.
    extern const Color Blue;

    /// The default color for Player 3.
    extern const Color Teal;

    /// The default color for Player 4.
    extern const Color Purple;

    /// The default color for Player 5.
    extern const Color Orange;

    /// The default color for Player 6.
    extern const Color Brown;

    /// The default color for Player 7.
    extern const Color White;

    /// The default color for Player 8.
    extern const Color Yellow;

    /// The alternate color for Player 7 on Ice tilesets.
    extern const Color Green;

    /// The default color for Neutral (Player 12).
    extern const Color Cyan;
    
    /// The color black
    extern const Color Black;

    /// The color grey
    extern const Color Grey;
  }
  
  /// Namespace containing text formatting codes. Such codes are used in calls to Game::drawText,
  /// Game::printf, and Broodwar::operator<<
  namespace Text
  {
    // Enumeration of text formatting codes
    enum Enum
    {
      /// Uses the previous color that was specified before the current one.
      Previous    = 1,
      
      /// Uses the default blueish color. This color is used in standard game messages.
      Default      = 2,
      
      /// A solid yellow. This yellow is used in notifications and is also the default color when
      /// printing text to Broodwar.
      Yellow      = 3,
      
      /// A bright white. This is used for timers.
      White      = 4,
      
      /// A dark grey. This color code will override all color formatting that follows.
      Grey      = 5,
      
      /// A deep red. This color code is used for error messages.
      Red        = 6,
      
      /// A solid green. This color is used for sent messages and resource counters.
      Green      = 7,
      
      /// A type of red. This color is used to color the name of the red player.
      BrightRed    = 8,
      
      /// This code hides all text and formatting that follows.
      Invisible    = 11,
      
      /// A deep blue. This color is used to color the name of the blue player.
      Blue      = 14,
      
      /// A teal color. This color is used to color the name of the teal player.
      Teal      = 15,
      
      /// A deep purple. This color is used to color the name of the purple player.
      Purple      = 16,
      
      /// A solid orange. This color is used to color the name of the orange player.
      Orange      = 17,
      
      /// An alignment directive that aligns the text to the right side of the screen.
      Align_Right    = 18,
      
      /// An alignment directive that aligns the text to the center of the screen.
      Align_Center  = 19,
      
      /// @copydoc Invisible
      Invisible2    = 20,
      
      /// A dark brown. This color is used to color the name of the brown player.
      Brown      = 21,
      
      /// A dirty white. This color is used to color the name of the white player.
      PlayerWhite    = 22,
      
      /// A deep yellow. This color is used to color the name of the yellow player.
      PlayerYellow  = 23,
      
      /// A dark green. This color is used to color the name of the green player.
      DarkGreen    = 24,
      
      /// A bright yellow.
      LightYellow    = 25,
      
      /// A cyan color. Similar to Default.
      Cyan      = 26,
      
      /// A tan color.
      Tan        = 27,
      
      /// A dark blueish color.
      GreyBlue    = 28,
      
      /// A type of Green.
      GreyGreen    = 29,
      
      /// A different type of Cyan.
      GreyCyan    = 30,
      
      /// A bright blue color.
      Turquoise    = 31
    };

    /// Standard output stream operator for text formatting codes. This is used to correctly
    /// format the codes for output.
    ///
    /// @param out
    ///   Reference to destination output stream.
    /// @param t
    ///   Reference to the Text formatting code to insert into the output stream.
    ///
    /// @returns Reference to the \p out parameter that was passed in.
    std::ostream &operator << (std::ostream &out, const Text::Enum &t);
  }
}
