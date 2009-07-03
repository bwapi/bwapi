#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class Color
  {
  public:
    Color();
    Color(int id);
    Color(const Color &other);
    Color(int red, int green, int blue);
    Color& operator=(const Color &other);
    bool operator==(const Color &other) const;
    bool operator!=(const Color &other) const;
    bool operator<(const Color &other) const;
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
    extern const Color Red;
    extern const Color Green;
    extern const Color Blue;
    extern const Color Yellow;
    extern const Color Cyan;
    extern const Color Purple;
    extern const Color Orange;
    extern const Color Black;
    extern const Color White;
    extern const Color Grey;
  }
}





