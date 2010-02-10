#pragma once

#include "BWSLInterface.h"

#include <string>
#include <set>

namespace BWSL
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
    BWSL_EXPORT extern const Color Red;
    BWSL_EXPORT extern const Color Green;
    BWSL_EXPORT extern const Color Blue;
    BWSL_EXPORT extern const Color Yellow;
    BWSL_EXPORT extern const Color Cyan;
    BWSL_EXPORT extern const Color Purple;
    BWSL_EXPORT extern const Color Orange;
    BWSL_EXPORT extern const Color Black;
    BWSL_EXPORT extern const Color White;
    BWSL_EXPORT extern const Color Grey;
  }
}
