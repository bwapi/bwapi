#pragma once
#include <cmath>
#include <algorithm>
#include <tuple>
#include <deque>
#include <iostream>

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

  /// <summary>The Point class is a base class that implements convenience members and performs
  /// conversions for several different position scales.</summary> It is intended to be inherited
  /// or typedef'd for use with BWAPI. Users can extend the Point class, and implement their own
  /// members, and it will remain compatible with BWAPI.
  ///
  /// @tparam T
  ///     The underlying type of the x and y values. BWAPI uses int.
  /// @tparam Scale
  ///     The underlying scale that this is intended to be used for. The smaller this value, the
  ///     higher the precision. A value of 1 indicates the pixel level.
  ///
  /// Consider the following:
  /// @code
  /// class VectorPos : public BWAPI::Point<double, 1>   // Same as BWAPI::Position with underlying type double
  /// {
  /// public:
  ///   VectorPos(double x_, double y_) : BWAPI::Point<double,1>(x_, y_) {}
  ///   // ... whatever members that operate with the underling type double
  /// };
  /// @endcode
  ///
  /// It then follows that this code will work without incident:
  /// @code
  /// BWAPI::Unit myUnit; // assume that the unit is valid and assigned elsewhere
  /// VectorPos myPos{5.7, 8.2};
  /// myUnit->move(myPos);  // Automatic type conversion, unit is moved to (5,8)
  /// @endcode
  ///
  /// @note For full compatibility with BWAPI, \p T must have a precision of at least 16 bits and
  /// \p Scale must be a factor of 32.
  template<typename T, int Scale>
  class Point
  {
  public:
    typedef std::deque< Point<T,Scale> > list;

    Point() = default;
    Point(T _x, T _y) : x(_x), y(_y) {}

    /// <summary>A copy constructor for positions with different underlying types.</summary>
    ///
    /// <param name="pt">
    ///     The Point to receive data from.
    /// </param>
    ///
    /// @tparam FromT
    ///     The type being converted to type T.
    template<typename FromT> Point(const Point<FromT, Scale> &pt) : x( static_cast<T>(pt.x) ), y( static_cast<T>(pt.y) ) {}

#pragma warning( push )
#pragma warning( disable: 4723 )
    /// <summary>A conversion copy constructor to convert positions of different scales to one
    /// another.</summary>
    ///
    /// @tparam FromT
    ///     The type that it is converting from.
    /// @tparam FromScale
    ///     The scale that it is converting from.
    template<typename FromT, int FromScale> explicit Point(const Point<FromT, FromScale> &pt)
      : x(static_cast<T>(FromScale > Scale ? pt.x*(FromScale / Scale) : pt.x / (Scale / FromScale)))
      , y(static_cast<T>(FromScale > Scale ? pt.y*(FromScale / Scale) : pt.y / (Scale / FromScale))) { }
#pragma warning( pop )

    // Operators
    /// <summary>A convenience for use with if statements to identify if a position is valid.</summary>
    /// @see isValid
    explicit operator bool() const { return this->isValid(); };
    
    bool operator == (const Point<T,Scale> &pos) const
    { 
      return std::tie(this->x, this->y) == std::tie(pos.x, pos.y);
    }; 
    bool operator != (const Point<T,Scale> &pos) const
    { 
      return !(*this == pos);
    }; 

    /// <summary>A less than operator that enables positions to be used by additional STL containers.</summary>
    /// Compares lexicographically the x position, followed by the y position.
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

    /// <summary>Ouput stream operator overload. Outputs the Point in the format "(x,y)" without
    /// quotations.</summary>
    ///
    /// <param name="os">
    ///   Output stream.
    /// </param>
    /// <param name="pt">
    ///   Point to output.
    /// </param>
    /// @returns Output stream \p out.
    friend std::ostream &operator << (std::ostream &os, const Point<T, Scale> &pt)
    {
      return os << '(' << pt.x << ',' << pt.y << ')';
    };
    /// @overload
    friend std::wostream &operator << (std::wostream &os, const Point<T, Scale> &pt)
    {
      return os << L'(' << pt.x << L',' << pt.y << L')';
    };

    /// <summary>Input stream operator overload. Reads the input in the form "x y" without
    /// quotations.</summary> The x and y values are read as type T(typically int or float) and
    /// stored into pt.
    ///
    /// <param name="in">
    ///   The input stream.
    /// </param>
    /// <param name="pt">
    ///   The receiving variable.
    /// </param>
    /// @returns Input stream \p in.
    friend std::istream &operator >> (std::istream &in, Point<T, Scale> &pt)
    {
      return in >> pt.x >> pt.y;
    };
    /// @overload
    friend std::wistream &operator >> (std::wistream &in, Point<T, Scale> &pt)
    {
      return in >> pt.x >> pt.y;
    };

    /// <summary>Checks if this point is within the game's map bounds.</summary>
    ///
    /// @note If the Broodwar pointer is not initialized, this function will check validity
    /// against the largest (256x256) map size.
    ///
    /// @retval true If it is a valid position and on the map/playing field.
    /// @retval false If this is not a valid position.
    ///
    /// @see makeValid
    bool isValid() const;

    /// <summary>Checks if this point is within the game's map bounds, if not, then it will set
    /// the x and y values to be within map bounds.</summary> For example, if x is less than 0,
    /// then x is set to 0.
    ///
    /// @note If the Broodwar pointer is not initialized, this function will check validity
    /// against the largest (256x256) map size.
    ///
    /// @returns A reference to itself.
    /// @see isValid
    Point &makeValid();

    /// <summary>Gets an accurate distance measurement from this point to the given position.</summary>
    ///
    /// @note This is a direct distance calculation that ignores all collision.
    ///
    /// @note This function impedes performance. In most cases you should use getApproxDistance.
    ///
    /// <param name="position">
    ///   The target position to get the distance to.
    /// </param>
    ///
    /// @returns A double representing the distance between this point and \p position.
    /// @see getApproxDistance
    double getDistance(const Point<T,Scale> &position) const
    {
      return ((*this) - position).getLength();
    };
    
    /// <summary>Gets the length of this point from the top left corner of the map.</summary>
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
    
    /// <summary>Retrieves the approximate distance using an algorithm from Starcraft: Broodwar.</summary>
    ///
    /// @note This is a direct distance calculation that ignores all collision.
    ///
    /// @note This function is desired because it uses the same "imperfect" algorithm used in
    /// Broodwar, so that calculations will be consistent with the game. It is also optimized
    /// for performance.
    ///
    /// <param name="position">
    ///     The target point to measure the distance to.
    /// </param>
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
    
    /// <summary>Sets the maximum x and y values.</summary> If the  current x or y values exceed
    /// the given maximum, then values are set to the maximum.
    ///
    /// <param name="max_x">
    ///     Maximum x value.
    /// </param>
    /// <param name="max_y">
    ///     Maximum y value.
    /// </param>
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
    
    /// <summary>Sets the minimum x and y values.</summary> If the current x or y values are
    /// below the given minimum, then values are set to the minimum.
    ///
    /// <param name="min_x">
    ///     Minimum x value.
    /// </param>
    /// <param name="min_y">
    ///     Minimum y value.
    /// </param>
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

    /// <summary>The x and y members for this class.</summary>
    ///
    /// Simply reference these members when retrieving a position's x and y values.
#ifndef SWIG
    T x = T{}, y = T{};
#else
    T x, y;
#endif
  };

  /// <summary>The scale of a @ref Position. Each position corresponds to a 1x1 pixel area.</summary>
  /// @see Position
  const int POSITION_SCALE = 1;
  
  /// <summary>The scale of a @ref WalkPosition. Each walk position corresponds to an 8x8 pixel area.</summary>
  /// @see WalkPosition
  const int WALKPOSITION_SCALE = 8;

  /// <summary>The scale of a @ref TilePosition. Each tile position corresponds to a 32x32 pixel area.</summary>
  /// @see TilePosition
  const int TILEPOSITION_SCALE = 32;

  /// <summary>Indicates a position that is 1x1 pixel in size. This is the most precise position type.</summary>
  /// @see Positions
  typedef BWAPI::Point<int, POSITION_SCALE> Position;

  /// <summary>List of special @ref Position constants.</summary>
  namespace Positions
  {
    /// @hideinitializer
    const Position Invalid{32000 / POSITION_SCALE, 32000 / POSITION_SCALE};
    /// @hideinitializer
    const Position None{32000 / POSITION_SCALE, 32032 / POSITION_SCALE};
    /// @hideinitializer
    const Position Unknown{32000 / POSITION_SCALE, 32064 / POSITION_SCALE};
    const Position Origin{0, 0};
  }

  /// <summary>Indicates a position that is 8x8 pixels in size.</summary>
  /// @see Game::isWalkable, WalkPositions
  typedef BWAPI::Point<int, WALKPOSITION_SCALE> WalkPosition;

  /// <summary>List of special @ref WalkPosition constants.</summary>
  namespace WalkPositions
  {
    /// @hideinitializer
    const WalkPosition Invalid{32000 / WALKPOSITION_SCALE, 32000 / WALKPOSITION_SCALE};
    /// @hideinitializer
    const WalkPosition None{32000 / WALKPOSITION_SCALE, 32032 / WALKPOSITION_SCALE};
    /// @hideinitializer
    const WalkPosition Unknown{32000 / WALKPOSITION_SCALE, 32064 / WALKPOSITION_SCALE};
    const WalkPosition Origin{0,0};
  }

  /// <summary>Indicates a position that is 32x32 pixels in size. Typically used for building placement.</summary>
  /// @see TilePositions
  typedef BWAPI::Point<int, TILEPOSITION_SCALE> TilePosition;

  /// <summary>List of special @ref TilePosition constants.</summary>
  namespace TilePositions
  {
    /// @hideinitializer
    const TilePosition Invalid{32000 / TILEPOSITION_SCALE, 32000 / TILEPOSITION_SCALE};
    /// @hideinitializer
    const TilePosition None{32000 / TILEPOSITION_SCALE, 32032 / TILEPOSITION_SCALE};
    /// @hideinitializer
    const TilePosition Unknown{32000 / TILEPOSITION_SCALE, 32064 / TILEPOSITION_SCALE};
    const TilePosition Origin{0, 0};
  }

  static_assert(sizeof(Position) == 8, "Expected BWAPI Position to be 8 bytes.");
  static_assert(sizeof(TilePosition) == 8, "Expected BWAPI Position to be 8 bytes.");
  static_assert(sizeof(WalkPosition) == 8, "Expected BWAPI Position to be 8 bytes.");
}
