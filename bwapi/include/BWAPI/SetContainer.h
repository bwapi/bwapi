#pragma once
#include <set>
#include <functional>
#include <algorithm>

namespace BWAPI
{
  template <class T, typename Compare>
  // Transparent comparators for lookups from comparables, without constructing temporary objects
  using SetContainerUnderlyingT = std::set < T, Compare >;

  /// <summary>This container is used to wrap convenience functions for BWAPI and be used as a
  /// bridge with a built-in set type.</summary>
  ///
  /// @tparam T
  ///     Type that this set contains.
  /// @tparam Compare
  ///     Function to ensure ordering.
  template <class T, typename Compare = std::less<>>
  class SetContainer : public SetContainerUnderlyingT < T, Compare >
  {
  public:
#ifndef SWIG
    using SetContainerUnderlyingT<T, Compare >::SetContainerUnderlyingT;
#endif

    /// <summary>Iterates the set and erases each element x where pred(x) returns true.</summary>
    ///
    /// <param name="pred">
    ///     Predicate for removing elements.
    /// </param>
    /// @see std::erase_if
    template<class Pred>
    SetContainer<T, Compare>& erase_if(Pred &&pred) {
      auto it = this->begin();
      while (it != this->end()) {
        if (pred(*it)) it = this->erase(it);
        else ++it;
      }
      return *this;
    }

    /// <summary>Iterates the set and finds the first element where pred(x) returns true.</summary>
    ///
    /// <param name="pred">
    ///     Predicate for searching.
    /// </param>
    /// @see std::find_if
    template<class Pred>
    typename SetContainer<T, Compare>::iterator find_if(Pred &&pred) const {
      return std::find_if(this->begin(), this->end(), pred);
    }

    /// <summary>Checks if this set contains a specific value.</summary>
    ///
    /// <param name="value">
    ///     Value to search for.
    /// </param>
    bool contains(T const &value) const
    {
      return this->count(value) != 0;
    }
  };

}
