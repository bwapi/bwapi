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
      Color(const Color& other);
      Color(int red, int green, int blue);
      Color& __cdecl operator=(const Color& other);
      bool __cdecl operator==(const Color& other) const;
      bool __cdecl operator!=(const Color& other) const;
      bool __cdecl operator<(const Color& other) const;
      int __cdecl getID() const;
      int __cdecl red() const;
      int __cdecl green() const;
      int __cdecl blue() const;
    private:
      int id;
  };
  namespace Colors
  {
    void __cdecl init();
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





