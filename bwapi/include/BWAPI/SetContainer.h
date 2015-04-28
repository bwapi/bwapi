#pragma once
#include <unordered_set>
#include <set>

namespace BWAPI
{
  template <class T, class HashT>
  using SetContainerUnderlyingT = std::unordered_set < T, HashT >;

  /// <summary>This container is used to wrap convenience functions for BWAPI and be used as a
  /// bridge with a built-in set type.</summary>
  ///
  /// @tparam T
  ///     Type that this set contains.
  /// @tparam HashT
  ///     Hash type. Defaults to integral hashing for BWAPI usage.
  template <class T, class HashT = std::hash<int>>
  class SetContainer : public SetContainerUnderlyingT < T, HashT >
  {
  public:
    SetContainer() : SetContainerUnderlyingT<T, HashT>() {}
    SetContainer(SetContainer const &other) : SetContainerUnderlyingT<T, HashT>(other) {}
    SetContainer(SetContainer &&other) : SetContainerUnderlyingT<T, HashT>(std::forward<SetContainer>(other)) {}
    SetContainer(std::initializer_list<T> ilist) : SetContainerUnderlyingT<T, HashT>(ilist) {}
    
    template <class IterT>
    SetContainer(IterT _begin, IterT _end) : SetContainerUnderlyingT<T, HashT>(_begin, _end) {}
    
    /// <summary>Iterates the set and erases each element x where pred(x) returns true.</summary>
    ///
    /// <param name="pred">
    ///     Predicate for removing elements.
    /// </param>
    /// @see std::erase_if
    template<class Pred>
    void erase_if(const Pred& pred) {
      auto it = this->begin();
      while (it != this->end()) {
        if (pred(*it)) it = this->erase(it);
        else ++it;
      }
    };

    /// <summary>Checks if this set contains a specific value.</summary>
    ///
    /// <param name="value">
    ///     Value to search for.
    /// </param>
    bool contains(T const &value) const
    {
      return count(value) != 0;
    }
  };

}
