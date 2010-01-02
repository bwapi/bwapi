#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class Color
  {
    public:
      Color();
      Color(int id);
      Color(const Color& other);
      Color(int red, int green, int blue);
      Color& operator=(const Color& other);
      bool operator==(const Color& other) const;
      bool operator!=(const Color& other) const;
      bool operator<(const Color& other) const;
      int getID() const;
      int red() const;
      int green() const;
      int blue() const;
    private:
      int id;
  };
  namespace Colors
  {
    void init();
    BWAPI2_EXPORT extern const Color Red;
    BWAPI2_EXPORT extern const Color Green;
    BWAPI2_EXPORT extern const Color Blue;
    BWAPI2_EXPORT extern const Color Yellow;
    BWAPI2_EXPORT extern const Color Cyan;
    BWAPI2_EXPORT extern const Color Purple;
    BWAPI2_EXPORT extern const Color Orange;
    BWAPI2_EXPORT extern const Color Black;
    BWAPI2_EXPORT extern const Color White;
    BWAPI2_EXPORT extern const Color Grey;
  }
}
