#pragma once
#include <cmath>
#include <algorithm>
#include <iosfwd>
#include <tuple>

#include <BWAPI/Vectorset.h>

#ifndef _MAKE_POSITION_TEMPLATE

#ifdef SWIG
#define _MAKE_POSITION_TEMPLATE(_n,_t,_s) typedef BWAPI::Point<_t,_s> _n;
#else
#define _MAKE_POSITION_TEMPLATE(_n,_t,_s) typedef BWAPI::Point<_t,_s> _n;   \
                      namespace _n ## s                                     \
                      { const _n Invalid(32000/_s,32000/_s);                \
                        const _n None(32000/_s,32032/_s);                   \
                        const _n Unknown(32000/_s,32064/_s);                \
                        const _n Origin(0,0);                               \
                      }
#endif

#define _OPERATOR_OP_PT(op) Point operator op (const Point &pos) const      \
              { return Point(this->x op pos.x, this->y op pos.y); };        \
              Point &operator op ## = (const Point &pos)                    \
              { this->x op ## = pos.x; this->y op ## = pos.y;               \
                return *this; }; 

#define _OPERATOR_OP_VAL(op) Point operator op (const _T &val) const        \
              { return Point(this->x op val, this->y op val); };            \
              Point &operator op ## = (const _T &val)                       \
              { this->x op ## = val; this->y op ## = val;                   \
                return *this; }; 

#define _OPERATOR_OP_VAL_CHK(op) Point operator op (const _T &val) const                  \
                { if ( val == 0 ) return Point(32000/__Scale,32000/__Scale);              \
                  return Point(this->x op val, this->y op val); };                        \
                Point &operator op ## = (const _T &val)                                   \
                { if ( val == 0 ) { this->x = 32000/__Scale; this->y = 32000/__Scale; }   \
                  else { this->x op ## = val; this->y op ## = val; }                      \
                  return *this; }; 

#endif

namespace BWAPI
{
  // Declaration
  template<typename _T, int __Scale = 1>
  class Point;

  // Restrictions (no division by 0 or types too small to contain map positions)
  template<typename _T> class Point<_T, 0> {};
  template<int __Scale> class Point<char, __Scale> {};
  template<int __Scale> class Point<unsigned char, __Scale> {};
  template<int __Scale> class Point<bool, __Scale> {};
  
  // ------------------------------------------------------ Point template ----------------
  template<typename _T, int __Scale>
  class Point
  {
  public:
    typedef Vectorset< Point<_T,__Scale> > set;

    // Constructors
    Point(_T _x = 0, _T _y = 0) : x(_x), y(_y) {};
    template<typename _NT> Point(const Point<_NT, __Scale> &pt) : x( (_T)pt.x ), y( (_T)pt.y ) {};

#pragma warning( push )
#pragma warning( disable: 4723 )
    // Conversion constructor
    template<typename _NT, int __NScale> explicit Point(const Point<_NT, __NScale> &pt)
      : x( (_T)(__NScale > __Scale ? pt.x*(__NScale/__Scale) : pt.x/(__Scale/__NScale)) )
      , y( (_T)(__NScale > __Scale ? pt.y*(__NScale/__Scale) : pt.y/(__Scale/__NScale)) ) { };
#pragma warning( pop )

    // Conversion restriction constructor
    template<typename _NT> Point(const Point<_NT, 0> &pt) : x(0), y(0) {};

    // Operators
    operator bool() const { return this->isValid(); };
    
    bool operator == (const Point<_T,__Scale> &pos) const
    { 
      return std::tie(this->x, this->y) == std::tie(pos.x, pos.y);
    }; 
    bool operator != (const Point<_T,__Scale> &pos) const
    { 
      return !(*this == pos);
    }; 

    bool operator  < (const Point<_T,__Scale> &position) const
    {
      return std::tie(this->x, this->y) < std::tie(position.x, position.y);
    };

    _OPERATOR_OP_PT(+)
    _OPERATOR_OP_PT(-)

    _OPERATOR_OP_VAL(*)
    _OPERATOR_OP_VAL(&)
    _OPERATOR_OP_VAL(|)
    _OPERATOR_OP_VAL(^)

    _OPERATOR_OP_VAL_CHK(/)
    _OPERATOR_OP_VAL_CHK(%)
    
    //friend std::ostream &operator << (std::ostream &out, const Point<_T,__Scale> &pt)
    template<typename _Tp, typename _CharT, class _Traits>
    friend 
    std::basic_ostream<_CharT, _Traits>& operator << (std::basic_ostream<_CharT, _Traits> &out, const Point<_T,__Scale> &pt)
    {
      //return out << '(' << pt.x << ',' << pt.y << ')';
         std::basic_ostream<_CharT, _Traits> __s;
         __s.flags(out.flags());
         __s.imbue(out.getloc());
         __s.precision(out.precision());
         __s << '(' << pt.x << ',' << pt.y << ')';
         return out << __s.str();
    };

    friend std::istream &operator >> (std::istream &in, Point<_T,__Scale> &pt)
    {
      return in >> pt.x >> pt.y;
    };

    // Functions
    
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
    double getDistance(const Point<_T,__Scale> &position) const
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
    int getApproxDistance(const Point<_T,__Scale> &position) const
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
    Point &setMax(_T max_x, _T max_y)
    {
      if ( x > max_x )
        x = max_x;
      if ( y > max_y )
        y = max_y;
      return *this;
    };
    /// @overload
    Point &setMax(const Point<_T,__Scale> &max)
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
    Point &setMin(_T min_x, _T min_y)
    {
      if ( x < min_x )
        x = min_x;
      if ( y < min_y )
        y = min_y;
      return *this;
    };
    /// @overload
    Point &setMin(const Point<_T,__Scale> &min)
    {
      this->setMin(min.x, min.y);
      return *this;
    };

    /// The x and y members for this class.
    ///
    /// Simply reference these members when retrieving a position's x and y values.
    _T x, y;
  };

  _MAKE_POSITION_TEMPLATE(WalkPosition,int,8)
  _MAKE_POSITION_TEMPLATE(Position,int,1)
  _MAKE_POSITION_TEMPLATE(TilePosition,int,32)
}

