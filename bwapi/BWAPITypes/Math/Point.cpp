#include "Point.h"

#include <math.h>

namespace BWAPI
{
  namespace Math
  {
    Point Point::Invalid(0x80000000, 0x80000000);
    //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
    Point::Point()
        : x(0)
        , y(0)
    {
    }
    //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
    Point::Point(int x, int y)
        : x(x)
        , y(y)
    {
    }
    //---------------------------------------------- OPERATOR == -----------------------------------------------
    bool Point::operator == (const Point& Point) const
    {
      return this->x == Point.x &&
             this->y == Point.y;
    }
    //---------------------------------------------- OPERATOR != -----------------------------------------------
    bool Point::operator != (const Point& Point) const
    {
      return this->x != Point.x ||
             this->y != Point.y;
    }
    //---------------------------------------------- OPERATOR < ------------------------------------------------
    bool Point::operator  < (const Point& Point) const
    {
      return this->x < Point.x ||
             (this->x == Point.x && this->y < Point.y);
    }
    //----------------------------------------------------------------------------------------------------------
    Point Point::operator+(const Point& point) const
    {
      return Point(this->x + point.x, this->y + point.y);
    }
    //----------------------------------------------------------------------------------------------------------
    Point Point::operator-(const Point& point) const
    {
      return Point(this->x - point.x, this->y - point.y);
    }
    //----------------------------------------------------------------------------------------------------------
    Point& Point::operator+=(const Point& point)
    {
      this->x += point.x;
      this->y += point.y;
      return *this;
    }
    //----------------------------------------------------------------------------------------------------------
    Point& Point::operator-=(const Point& point)
    {
      this->x -= point.x;
      this->y -= point.y;
      return *this;
    }
    //----------------------------------------------------------------------------------------------------------
    double Point::getDistance(const Point& point) const
    {
      return ((*this) - point).getLength();
    }
    //----------------------------------------------------------------------------------------------------------
    double Point::getLength() const
    {
      double x = this->x;
      double y = this->y;
      return sqrt(x * x + y * y);
    }
    //----------------------------------------------------------------------------------------------------------
  }
};