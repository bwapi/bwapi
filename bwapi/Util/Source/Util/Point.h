#pragma once

namespace Util
{
  template<typename L>  // linear type
  struct Point
  {
      L x;
      L y;

      typedef L LinearType;

      static const Point<L> Invalid;

      Point();
      Point(L x, L y);
      template<typename T>
        Point(Point<T> point)
          : x((L)point.x)
          , y((L)point.y)
        {
        }
      bool operator == (const Point<L>& b) const;
      bool operator != (const Point<L>& b) const;
      bool operator  < (const Point<L>& b) const;
      Point<L> operator + (const Point<L>& b) const;
      Point<L> operator - (const Point<L>& b) const;
      Point<L>& operator += (const Point<L>& b);
      Point<L>& operator -= (const Point<L>& b);
      void exchange(Point<L>& b);
      double getDistance(const Point<L>& b) const;
      double getLength() const;
  };
  template<typename L>
  const Point<L> Point<L>::Invalid = Point<L>(1 << (sizeof(L)*8 - 1), 1 << (sizeof(L)*8 - 1));
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  template<typename L>
  Point<L>::Point()
      : x(0)
      , y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  template<typename L>
  Point<L>::Point(L x, L y)
      : x(x)
      , y(y)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  template<typename L>
  bool Point<L>::operator == (const Point<L>& point) const
  {
    return this->x == point.x &&
           this->y == point.y;
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  template<typename L>
  bool Point<L>::operator != (const Point<L>& point) const
  {
    return this->x != point.x ||
           this->y != point.y;
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  template<typename L>
  bool Point<L>::operator  < (const Point<L>& point) const
  {
    return this->x < point.x ||
           (this->x == point.x && this->y < point.y);
  }
  //----------------------------------------------------------------------------------------------------------
  template<typename L>
  Point<L> Point<L>::operator+(const Point<L>& point) const
  {
    return Point(this->x + point.x, this->y + point.y);
  }
  //----------------------------------------------------------------------------------------------------------
  template<typename L>
  Point<L> Point<L>::operator-(const Point<L>& point) const
  {
    return Point(this->x - point.x, this->y - point.y);
  }
  //----------------------------------------------------------------------------------------------------------
  template<typename L>
  Point<L>& Point<L>::operator+=(const Point<L>& point)
  {
    this->x += point.x;
    this->y += point.y;
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  template<typename L>
  Point<L>& Point<L>::operator-=(const Point<L>& point)
  {
    this->x -= point.x;
    this->y -= point.y;
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  template<typename L>
  double Point<L>::getDistance(const Point<L>& point) const
  {
    return ((*this) - point).getLength();
  }
  //----------------------------------------------------------------------------------------------------------
  template<typename L>
  double Point<L>::getLength() const
  {
    double x = this->x;
    double y = this->y;
    return sqrt(x * x + y * y);
  }
  //----------------------------------------------------------------------------------------------------------
}

