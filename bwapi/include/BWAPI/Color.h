#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  // TODO: Add color palette image and info about text color
  /** StarCraft uses a 256 color palette to render everything, so the colors we can use to draw shapes using
   * BWAPI is limited to the colors available in the Palette. */
  class Color
  {
    public:
      Color();

      /** Create a color using the specified index from the Broodwar color palette. */
      Color(int id);
      Color(const Color& other);

      /** Create a color using the color in the palette that is closest to the RGB color specified. */
      Color(int red, int green, int blue);

      /** Return the index of the color in the color palette. */
      int getID() const;

      /** Return the red component of the color. */
      int red() const;

      /** Return the green component of the color. */
      int green() const;

      /** Return the blue component of the color. */
      int blue() const;

      Color& operator=(const Color& other);
      operator int() const;
    private:
      int id;
  };

  /** While any color from the palette can be used, the following colors are available as short cuts. */
  namespace Colors
  {
    void init();
    extern const Color Red;
    extern const Color Blue;
    extern const Color Teal;
    extern const Color Purple;
    extern const Color Orange;
    extern const Color Brown;
    extern const Color White;
    extern const Color Yellow;
    extern const Color Green;
    extern const Color Cyan;
    extern const Color Black;
    extern const Color Grey;
  }
}
