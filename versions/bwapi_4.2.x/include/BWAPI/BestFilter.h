#pragma once
#include <functional>
#include "ComparisonFilter.h"

namespace BWAPI
{
  /// <summary>The BestFilter is used to compare two objects with each other.</summary> Each
  /// function object compares two of the same object and returns the most desirable one.
  ///
  /// @note: A function object should return one of the two given parameters, never nullptr. Both
  /// parameters being given should never be nullptr.
  template<typename _PARAM>
  class BestFilter
  {
  private:
    std::function<_PARAM(_PARAM,_PARAM)> pred;
  public:
    // Constructor
    template <typename _T>
    BestFilter(const _T &predicate) : pred(predicate)
    {};

    // Assignment
    template <typename _T>
    BestFilter<_PARAM> &operator =(const _T &other)
    {
      this->pred = other;
      return *this;
    };

    // Bitwise operators
    template <typename _T>
    inline BestFilter<_PARAM> operator &&(const _T &other) const
    {
      return [=](_PARAM p1, _PARAM p2)->_PARAM{ return other( (*this)(p1, p2) ); };
    };

    // call
    inline _PARAM operator()(const _PARAM &p1, const _PARAM &p2) const
    {
      return this->pred(p1, p2);
    };

  };

  template <typename _PARAM>
  BestFilter<_PARAM> Lowest(const CompareFilter<_PARAM,int> &filter)
  {
    return [=](_PARAM p1, _PARAM p2)->_PARAM{ return filter(p2) < filter(p1) ? p2 : p1; };
  };
  template <typename _PARAM>
  BestFilter<_PARAM> Highest(const CompareFilter<_PARAM,int> &filter)
  {
    return [=](_PARAM p1, _PARAM p2)->_PARAM{ return filter(p2) > filter(p1) ? p2 : p1; };
  };

}

