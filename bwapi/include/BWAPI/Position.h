#pragma once
#include <cmath>
#include <algorithm>
#include <iosfwd>
#include <tuple>
#include <deque>

#ifdef SWIG
#define BWAPI_MAKE_POSITION_TEMPLATE(_n,T,_s) typedef BWAPI::Point<T,_s> _n;
#else
#define BWAPI_MAKE_POSITION_TEMPLATE(_n,T,_s) typedef BWAPI::Point<T,_s> _n;   \
                      namespace _n ## s                                     \
                      { const _n Invalid(32000/_s,32000/_s);                \
                        const _n None(32000/_s,32032/_s);                   \
                        const _n Unknown(32000/_s,32064/_s);                \
                        const _n Origin(0,0);                               \
                      }
#endif

namespace BWAPI
{
  // Declaration
  template<typename T, int Scale = 1>
  class Point;

  // Restrictions (no division by 0 or types too small to contain map positions)
  template<typename T> class Point<T, 0> {};
  template<int Scale> class Point<char, Scale> {};
  template<int Scale> class Point<unsigned char, Scale> {};
  template<int Scale> class Point<bool, Scale> {};
  
  // ------------------------------------------------------ Point template ----------------
  template<typename T, int Scale>
  class Point
  {
  public:
    typedef std::deque< Point<T,Scale> > list;

    // Constructors
    Point() : x(T{}), y(T{}) {}
    Point(T _x, T _y) : x(_x), y(_y) {}
    template<typename _NT> Point(const Point<_NT, Scale> &pt) : x( (T)pt.x ), y( (T)pt.y ) {}

#pragma warning( push )
#pragma warning( disable: 4723 )
    // Conversion constructor
    template<typename _NT, int __NScale> explicit Point(const Point<_NT, __NScale> &pt)
      : x((T)(__NScale > Scale ? pt.x*(__NScale / Scale) : pt.x / (Scale / __NScale)))
      , y((T)(__NScale > Scale ? pt.y*(__NScale / Scale) : pt.y / (Scale / __NScale))) { }
#pragma warning( pop )

    // Operators
    explicit operator bool() const { return this->isValid(); };
    
    bool operator == (const Point<T,Scale> &pos) const
    { 
      return std::tie(this->x, this->y) == std::tie(pos.x, pos.y);
    }; 
    bool operator != (const Point<T,Scale> &pos) const
    { 
      return !(*this == pos);
    }; 

    bool operator  < (const Point<T,Scale> &position) const
    {
      return std::tie(this->x, this->y) < std::tie(position.x, position.y);
    };

    inline Point<T, Scale> &operator += (const Point<T, Scale> &p)
    {
      x += p.x;
      y += p.y;
      return *this;
    };
    inline Point<T, Scale> operator + (const Point<T, Scale> &p) const
    {
      Point<T, Scale> r(*this);
      return r += p;
    };
    inline Point<T, Scale> &operator -= (const Point<T, Scale> &p)
    {
      x -= p.x;
      y -= p.y;
      return *this;
    };
    inline Point<T, Scale> operator - (const Point<T, Scale> &p) const
    {
      Point<T, Scale> r(*this);
      return r -= p;
    };

    inline Point<T, Scale> &operator *= (const T &v)
    {
      x *= v;
      y *= v;
      return *this;
    };
    inline Point<T, Scale> operator *(const T &v) const
    {
      Point<T, Scale> r(*this);
      return r *= v;
    };
    inline Point<T, Scale> &operator |= (const T &v)
    {
      x |= v;
      y |= v;
      return *this;
    };
    inline Point<T, Scale> operator |(const T &v) const
    {
      Point<T, Scale> r(*this);
      return r |= v;
    };
    inline Point<T, Scale> &operator &= (const T &v)
    {
      x &= v;
      y &= v;
      return *this;
    };
    inline Point<T, Scale> operator &(const T &v) const
    {
      Point<T, Scale> r(*this);
      return r &= v;
    };
    inline Point<T, Scale> &operator ^= (const T &v)
    {
      x ^= v;
      y ^= v;
      return *this;
    };
    inline Point<T, Scale> operator ^(const T &v) const
    {
      Point<T, Scale> r(*this);
      return r ^= v;
    };

    Point<T, Scale> operator / (const T &v) const
    {
      Point<T, Scale> result(*this);
      return result /= v;
    };
    Point<T, Scale> &operator /= (const T &val)
    {
      if (val == 0) { x = 32000 / Scale; y = 32000 / Scale; }
      else { x /= val; y /= val; }
      return *this;
    };
    Point<T, Scale> operator %(const T &v) const
    {
      Point<T, Scale> result(*this);
      return result %= v;
    };
    Point<T, Scale> &operator %= (const T &val)
    {
      if (val == 0) { x = 32000 / Scale; y = 32000 / Scale; }
      else { x %= val; y %= val; }
      return *this;
    };

    /// Ouput stream operator overload. Outputs the Point in the format "(x,y)" without
    /// quotations.
    ///
    /// @param out
    ///   Output stream.
    /// @param pt
    ///   Point to output.
    /// @returns Output stream \p out.
    friend std::ostream &operator << (std::ostream &out, const Point<T, Scale> &pt)
    {
      return out << '(' << pt.x << ',' << pt.y << ')';
    };
    friend std::wostream &operator << (std::wostream &out, const Point<T, Scale> &pt)
    {
      return out << L'(' << pt.x << L',' << pt.y << L')';
    };

    /// Input stream operator overload. Reads the input in the form "x y" without quotations.
    /// The x and y values are read as type T(typically int or float) and stored into pt.
    ///
    /// @param in
    ///   The input stream.
    /// @param pt
    ///   The receiving variable.
    /// @returns Input stream \p in.
    friend std::istream &operator >> (std::istream &in, Point<T, Scale> &pt)
    {
      return in >> pt.x >> pt.y;
    };
    friend std::wistream &operator >> (std::wistream &in, Point<T, Scale> &pt)
    {
      return in >> pt.x >> pt.y;
    };

    /// Checks if this point is within the game's map bounds.
    ///
    /// @note If the Broodwar pointer is not initialized, this function will check validity
    /// against the largest (256x256) map size.
    ///
    /// @retval true If it is a valid position and on the map/playing field.
    /// @retval false If this is not a valid position.
    ///
    /// @see makeValid
    bool isValid() const;

    /// Checks if this point is within the game's map bounds, if not, then it will set the x and y
    /// values to be within map bounds. (Example: If x is less than 0, then x is set to 0)
    ///
    /// @note If the Broodwar pointer is not initialized, this function will check validity
    /// against the largest (256x256) map size.
    ///
    /// @returns A reference to itself.
    /// @see isValid
    Point &makeValid();

    /// Gets an accurate distance measurement from this point to the given position.
    ///
    /// @note This function impedes performance. In most cases you should use getApproxDistance.
    ///
    /// @param position
    ///   The target position to get the distance to.
    ///
    /// @returns A double representing the distance between this point and \p position.
    /// @see getApproxDistance
    double getDistance(const Point<T,Scale> &position) const
    {
      return ((*this) - position).getLength();
    };
    
    /// Gets the length of this point from the top left corner of the map.
    ///
    /// @note This function impedes performance. In most cases you should use getApproxDistance.
    ///
    /// @returns A double representing the length of this point from (0,0).
    /// @see getApproxDistance
    double getLength() const
    {
      double x = (double)this->x;
      double y = (double)this->y;
      return sqrt(x * x + y * y);
    };
    
    /// Retrieves the approximate distance using an algorithm from Starcraft: Broodwar.
    ///
    /// @note This function is desired because it uses the same "imperfect" algorithm used in
    /// Broodwar, so that calculations will be consistent with the game. It is also optimized
    /// for performance.
    ///
    /// @param position
    ///     The target point to measure the distance to.
    ///
    /// @returns An integer representing the distance between this point and \p position.
    /// @see getDistance
    int getApproxDistance(const Point<T,Scale> &position) const
    {
      unsigned int min = abs((int)(this->x - position.x));
      unsigned int max = abs((int)(this->y - position.y));
      if ( max < min )
        std::swap(min, max);

      if ( min < (max >> 2) )
        return max;

      unsigned int minCalc = (3*min) >> 3;
      return (minCalc >> 5) + minCalc + max - (max >> 4) - (max >> 6);
    };
    
    /// Sets the maximum x and y values. If the  current x or y values exceed the given maximum,
    /// then values are set to the maximum.
    ///
    /// @param max_x
    ///     Maximum x value.
    /// @param max_y
    ///     Maximum y value.
    ///
    /// @returns A reference to itself.
    /// @see setMin
    Point &setMax(T max_x, T max_y)
    {
      if ( x > max_x )
        x = max_x;
      if ( y > max_y )
        y = max_y;
      return *this;
    };
    /// @overload
    Point &setMax(const Point<T,Scale> &max)
    {
      this->setMax(max.x, max.y);
      return *this;
    };
    
    /// Sets the minimum x and y values. If the current x or y values are below the given minimum,
    /// then values are set to the minimum.
    ///
    /// @param min_x
    ///     Minimum x value.
    /// @param min_y
    ///     Minimum y value.
    ///
    /// @returns A reference to itself.
    /// @see setMax
    Point &setMin(T min_x, T min_y)
    {
      if ( x < min_x )
        x = min_x;
      if ( y < min_y )
        y = min_y;
      return *this;
    };
    /// @overload
    Point &setMin(const Point<T,Scale> &min)
    {
      this->setMin(min.x, min.y);
      return *this;
    };

    /// The x and y members for this class.
    ///
    /// Simply reference these members when retrieving a position's x and y values.
    T x, y;
  };

  BWAPI_MAKE_POSITION_TEMPLATE(WalkPosition,int,8)
  BWAPI_MAKE_POSITION_TEMPLATE(Position,int,1)
  BWAPI_MAKE_POSITION_TEMPLATE(TilePosition,int,32)
}
