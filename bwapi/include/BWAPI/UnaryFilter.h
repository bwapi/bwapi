#pragma once
#include <functional>

namespace BWAPI
{
  /// <summary>UnaryFilter allows for logical functor combinations.</summary>
  ///
  /// @code{.cpp}
  ///   Unit myUnit;
  ///   // The following two if statements are equivalent
  ///   if ( myUnit->getType().isWorker() && myUnit->isCompleted() && myUnit->isIdle() )
  ///   {}
  ///
  ///   if ( (IsWorker && IsCompleted && IsIdle)(myUnit) )
  ///   {}
  /// @endcode
  ///
  /// @tparam PType
  ///   The type being passed into the predicate, which will be of type bool(PType).
  /// @tparam Container (optional)
  ///   Storage container for the function predicate. It is std::function<bool(PType)> by default.
  template <class PType, class Container = std::function<bool(PType)> >
  class UnaryFilter
  {
  private:
    Container pred;
  public:
    // ctor
    template < typename T >
    UnaryFilter(const T &predicate) : pred(predicate) {}

    // Default copy/move ctor/assign and dtor
    
    // logical operators
    template <typename T>
    inline UnaryFilter<PType,std::function<bool(PType)> > operator &&(const T& other) const
    {
      return [=](PType v){ return (*this)(v) && other(v); };
    }

    template <typename T>
    inline UnaryFilter<PType,std::function<bool(PType)> > operator ||(const T& other) const
    {
      return [=](PType v){ return (*this)(v) || other(v); };
    }

    inline UnaryFilter<PType,std::function<bool(PType)> > operator !() const
    {
      if ( !this->pred )
        return nullptr;
      return [=](PType v){ return !(*this)(v); };
    }

    // call
    inline bool operator()(PType v) const
    {
      return pred(v);
    }

    // operator bool
    inline bool isValid() const
    {
      return (bool)pred;
    }

  };

}
