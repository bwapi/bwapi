#pragma once
#include <set>

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
  /// @tparam HashT
  ///     Hash type. Defaults to integral hashing for BWAPI usage.
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
    void erase_if(Pred &&pred) {
      auto it = this->begin();
      while (it != this->end()) {
        if (pred(*it)) it = this->erase(it);
        else ++it;
      }
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
