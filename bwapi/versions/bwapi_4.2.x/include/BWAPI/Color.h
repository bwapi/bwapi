#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>The Color object is used in drawing routines to specify the color to use.</summary>
  ///
  /// @note Starcraft uses a 256 color palette for rendering. Thus, the colors available are
  /// limited to this palette.
  ///
  /// @see Colors
  /// @ingroup TypeClasses
  class Color : public Type<Color, 255>
  {
    public:
      /// <summary>A constructor that uses the color at the specified palette index.</summary>
      ///
      /// <param name="id">
      ///   The index of the color in the 256-color palette.
      /// </param>
      constexpr Color(int id = 0) : Type(id) {}

      /// <summary>A constructor that uses the color index in the palette that is closest to the
      /// given rgb values.</summary> On its first call, the colors in the palette will be sorted
      /// for fast indexing.
      ///
      /// @note This function computes the distance of the RGB values and may not be accurate.
      ///
      /// <param name="red">
      ///   The amount of red.
      /// </param>
      /// <param name="green">
      ///   The amount of green.
      /// </param>
      /// <param name="blue">
      ///   The amount of blue.
      /// </param>
      Color(int red, int green, int blue);

      /// <summary>Retrieves the red component of the color.</summary>
      ///
      /// @returns integer containing the value of the red component.
      int red() const;

      /// <summary>Retrieves the green component of the color.</summary>
      ///
      /// @returns integer containing the value of the green component.
      int green() const;

      /// <summary>Retrieves the blue component of the color.</summary>
      ///
      /// @returns integer containing the value of the blue component.
      int blue() const;
  };

  /// <summary>Namespace containing known colors.</summary>
  /// @see Color
  /// @ingroup Types
  namespace Colors
  {
    /// <summary>The default color for Player 1.</summary>
    constexpr Color Red{ 111 };

    /// <summary>The default color for Player 2.</summary>
    constexpr Color Blue{ 165 };

    /// <summary>The default color for Player 3.</summary>
    constexpr Color Teal{ 159 };

    /// <summary>The default color for Player 4.</summary>
    constexpr Color Purple{ 164 };

    /// <summary>The default color for Player 5.</summary>
    constexpr Color Orange{ 179 };

    /// <summary>The default color for Player 6.</summary>
    constexpr Color Brown{ 19 };

    /// <summary>A bright white. Note that this is lighter than Player 7's white.</summary>
    constexpr Color White{ 255 };

    /// <summary>The default color for Player 8.</summary>
    constexpr Color Yellow{ 135 };

    /// <summary>The alternate color for Player 7 on Ice tilesets.</summary>
    constexpr Color Green{ 117 };

    /// <summary>The default color for Neutral (Player 12).</summary>
    constexpr Color Cyan{ 128 };
    
    /// <summary>The color black</summary>
    constexpr Color Black{ 0 };

    /// <summary>The color grey</summary>
    constexpr Color Grey{ 74 };
  }
  
  /// <summary>Namespace containing text formatting codes.</summary> Such codes are used in calls
  /// to Game::drawText, Game::printf, and Broodwar::operator<<
  namespace Text
  {
    /// <summary>Enumeration of text formatting codes</summary>
    enum Enum
    {
      /// <summary>Uses the previous color that was specified before the current one.</summary>
      Previous    = 1,
      
      /// <summary>Uses the default blueish color. This color is used in standard game messages.</summary>
      Default      = 2,
      
      /// <summary>A solid yellow. This yellow is used in notifications and is also the default
      /// color when printing text to Broodwar.</summary>
      Yellow      = 3,
      
      /// <summary>A bright white. This is used for timers.</summary>
      White      = 4,
      
      /// <summary>A dark grey. This color code will override all color formatting that follows.</summary>
      Grey      = 5,
      
      /// <summary>A deep red. This color code is used for error messages.</summary>
      Red        = 6,
      
      /// <summary>A solid green. This color is used for sent messages and resource counters.</summary>
      Green      = 7,
      
      /// <summary>A type of red. This color is used to color the name of the red player.</summary>
      BrightRed    = 8,
      
      /// <summary>This code hides all text and formatting that follows.</summary>
      Invisible    = 11,
      
      /// <summary>A deep blue. This color is used to color the name of the blue player.</summary>
      Blue      = 14,
      
      /// <summary>A teal color. This color is used to color the name of the teal player.</summary>
      Teal      = 15,
      
      /// <summary>A deep purple. This color is used to color the name of the purple player.</summary>
      Purple      = 16,
      
      /// <summary>A solid orange. This color is used to color the name of the orange player.</summary>
      Orange      = 17,
      
      /// <summary>An alignment directive that aligns the text to the right side of the screen.</summary>
      Align_Right    = 18,
      
      /// <summary>An alignment directive that aligns the text to the center of the screen.</summary>
      Align_Center  = 19,
      
      /// @copydoc Invisible
      Invisible2    = 20,
      
      /// <summary>A dark brown. This color is used to color the name of the brown player.</summary>
      Brown      = 21,
      
      /// <summary>A dirty white. This color is used to color the name of the white player.</summary>
      PlayerWhite    = 22,
      
      /// <summary>A deep yellow. This color is used to color the name of the yellow player.</summary>
      PlayerYellow  = 23,
      
      /// <summary>A dark green. This color is used to color the name of the green player.</summary>
      DarkGreen    = 24,
      
      /// <summary>A bright yellow.</summary>
      LightYellow    = 25,
      
      /// <summary>A cyan color. Similar to Default.</summary>
      Cyan      = 26,
      
      /// <summary>A tan color.</summary>
      Tan        = 27,
      
      /// <summary>A dark blueish color.</summary>
      GreyBlue    = 28,
      
      /// <summary>A type of Green.</summary>
      GreyGreen    = 29,
      
      /// <summary>A different type of Cyan.</summary>
      GreyCyan    = 30,
      
      /// <summary>A bright blue color.</summary>
      Turquoise    = 31
    };

    /// <summary>Checks if the given character is a color-changing control code.</summary>
    /// @returns true if \p c is a regular color, not Text::Previous, Text::Invisible* or Text::Align*
    /// @since 4.2.0
    inline bool isColor(Text::Enum c)
    {
      return (2 <= c && c <= 8) || (14 <= c && c <= 17) || (21 <= c && c <= 31);
    }

    /// <summary>Namespace containing text sizes.</summary>
    namespace Size
    {
      /// <summary>Enumeration of available text sizes</summary>
      enum Enum
      {
        /// <summary>The smallest text size in the game.</summary>
        Small,

        /// <summary>The standard text size, used for most things in the game such as chat messages.</summary>
        Default,

        /// <summary>A larger text size. This size is used for the in-game countdown timer seen in @CTF and @UMS game types.</summary>
        Large,

        /// <summary>The largest text size in the game.</summary>
        Huge
      };
    }

    /// <summary>Standard output stream operator for text formatting codes.</summary> This is
    /// used to correctly format the codes for output.
    ///
    /// <param name="out">
    ///   Reference to destination output stream.
    /// </param>
    /// <param name="t">
    ///   Reference to the Text formatting code to insert into the output stream.
    /// </param>
    ///
    /// @returns Reference to the \p out parameter that was passed in.
    std::ostream &operator << (std::ostream &out, const Text::Enum &t);
  }

  static_assert(sizeof(Color) == sizeof(int), "Expected type to resolve to primitive size.");
}
