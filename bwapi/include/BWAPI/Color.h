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
  }
}





