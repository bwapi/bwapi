#pragma once
// Prevent warnings if someone includes windows.h
#ifdef max
#undef max
#endif

#include <functional>
#include <limits>

#include "UnaryFilter.h"

#define BWAPI_COMPARE_FILTER_OP(op) UnaryFilter<PType> operator op(const RType &cmp) const               \
                                    {   return [=](PType v)->bool{ return (*this)(v) op cmp; };   }

#define BWAPI_ARITHMATIC_FILTER_OP(op) template <typename T>                                            \
                                       CompareFilter<PType,RType,std::function<RType(PType)> > operator op(const T &other) const     \
                                       {   return [=](PType v)->int{ return (*this)(v) op other(v); };   }    \
                                       CompareFilter<PType,RType,std::function<RType(PType)> > operator op(RType other) const     \
                                       {   return [=](PType v)->int{ return (*this)(v) op other; };   }

namespace BWAPI
{
  /// <summary>The CompareFilter is a container in which a stored function predicate returns a
  /// value.</summary> Arithmetic and bitwise operators will return a new CompareFilter that
  /// applies the operation to the result of the original functor. If any relational operators are
  /// used, then it creates a UnaryFilter that returns the result of the operation.
  ///
  /// @tparam PType
  ///   The parameter type, which is the type passed into the functor.
  /// @tparam RType (optional)
  ///   The functor's return type. It is int by default.
  /// @tparam Container (optional)
  ///   Storage container for the function predicate. It is std::function<RType(PType)> by default.
  template < typename PType, typename RType=int, class Container = std::function<RType(PType)> >
  class CompareFilter
  {
  private:
    Container pred;
  public:
    // ctor
    template <typename T>
    CompareFilter(const T &predicate) : pred(predicate) {}

    // Default copy/move ctor/assign and dtor

    // Comparison operators
    BWAPI_COMPARE_FILTER_OP(==);
    BWAPI_COMPARE_FILTER_OP(!=);
    BWAPI_COMPARE_FILTER_OP(<=);
    BWAPI_COMPARE_FILTER_OP(>=);
    BWAPI_COMPARE_FILTER_OP(<);
    BWAPI_COMPARE_FILTER_OP(>);

    // Arithmetic operators
    BWAPI_ARITHMATIC_FILTER_OP(+);
    BWAPI_ARITHMATIC_FILTER_OP(-);
    BWAPI_ARITHMATIC_FILTER_OP(|);
    BWAPI_ARITHMATIC_FILTER_OP(&);
    BWAPI_ARITHMATIC_FILTER_OP(*);
    BWAPI_ARITHMATIC_FILTER_OP(^);

    // Division
    template <typename T>
    CompareFilter<PType,RType,std::function<RType(PType)> > operator /(const T &other) const
    {   
      return [=](PType v)->int{ int rval = other(v);
                                 return rval == 0 ? std::numeric_limits<int>::max() : (*this)(v) / rval;
                               };
    };

    // Modulus
    template <typename T>
    CompareFilter<PType,RType,std::function<RType(PType)> > operator %(const T &other) const
    {   
      return [=](PType v)->int{ int rval = other(v);
                                 return rval == 0 ? 0 : (*this)(v) % rval;
                               };
    };

    // call
    inline RType operator()(PType u) const
    {
      return pred(u);
    };
    
    inline bool isValid() const
    {
      return (bool)pred;
    };
  };
}

