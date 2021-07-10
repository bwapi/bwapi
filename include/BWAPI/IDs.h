#pragma once

#include <vector>

namespace BWAPI {
  struct NoneIdentifier {};

  template<typename T>
  struct Identifier {
    int id;
    using IdT = Identifier;
    constexpr explicit Identifier(int id_) : id{id_} { }
    constexpr Identifier(NoneIdentifier) : id{ -1 } { }
    T getID() const { return T{id}; }
    constexpr explicit operator bool() const { return id >= 0; }
    constexpr explicit operator int() const  { return id; }
    constexpr bool operator==(Identifier const &other) const { return id == other.id; }
    constexpr bool operator!=(Identifier const &other) const { return id != other.id; }
    constexpr bool operator< (Identifier const &other) const { return id <  other.id; }
    constexpr bool operator> (Identifier const &other) const { return id >  other.id; }
    constexpr bool operator<=(Identifier const &other) const { return id <= other.id; }
    constexpr bool operator>=(Identifier const &other) const { return id >= other.id; }
    constexpr bool operator!=(std::nullptr_t) const { return static_cast<bool>(*this); }
    constexpr bool operator==(std::nullptr_t) const { return !(*this != nullptr); }

    struct Hash {
      auto operator()(const Identifier& id) const {
        return std::hash<int>{}(static_cast<int>(id.getID()));
      }
    };

    using list = std::vector<T>;

    static constexpr NoneIdentifier None{};
  };

  struct UnitID   : Identifier<UnitID>   { using Identifier<UnitID>  ::Identifier; };
  struct PlayerID : Identifier<PlayerID> { using Identifier<PlayerID>::Identifier; };
  struct RegionID : Identifier<RegionID> { using Identifier<RegionID>::Identifier; };
  struct BulletID : Identifier<BulletID> { using Identifier<BulletID>::Identifier; };
  struct ForceID  : Identifier<ForceID>  { using Identifier<ForceID> ::Identifier; };
} // namespace BWAPI

namespace std {
  template<typename T>
  struct hash<BWAPI::Identifier<T>> {
    auto operator()(const BWAPI::Identifier<T>& id) const {
      return BWAPI::Identifier<T>::Hash()(id);
    }
  };
} // namespace std

namespace BWAPI {
  class Game;

  template<typename Interface, typename Data>
  struct InterfaceDataWrapper {
    Data const *dataptr;
    using IdT = std::decay_t<decltype(Data::id)>;

    /// <summary>Retrieves a unique identifier for this interface.</summary>
    ///
    /// @returns An integer containing the interface's identifier. -1 if the
    /// interface is null/None.
    constexpr IdT getID() const
    {
      return dataptr == nullptr ? IdT::None : dataptr->id;
    }

    constexpr InterfaceDataWrapper(Data const &ptr) : dataptr{ &ptr } { }
    constexpr InterfaceDataWrapper(std::nullptr_t) : dataptr{ nullptr } { }
    constexpr auto const &getData() const { return *dataptr; }
    constexpr Game &getGame() const { return dataptr->game; }
    constexpr explicit operator bool() const { return dataptr != nullptr; }
    constexpr explicit operator int() const { return static_cast<int>(dataptr->id); }
    constexpr bool operator==(InterfaceDataWrapper const &other) const { return getID() == other.getID(); }
    constexpr bool operator!=(InterfaceDataWrapper const &other) const { return getID() != other.getID(); }
    constexpr bool operator< (InterfaceDataWrapper const &other) const { return getID() <  other.getID(); }
    constexpr bool operator> (InterfaceDataWrapper const &other) const { return getID() >  other.getID(); }
    constexpr bool operator<=(InterfaceDataWrapper const &other) const { return getID() <= other.getID(); }
    constexpr bool operator>=(InterfaceDataWrapper const &other) const { return getID() >= other.getID(); }
    constexpr bool operator!=(std::nullptr_t) const { return dataptr != nullptr; }
    constexpr bool operator==(std::nullptr_t) const { return dataptr == nullptr; }

    struct Hash { 
      auto operator()(const InterfaceDataWrapper &idw) const {
        using hash = typename decltype(Data::id)::Hash;
        return hash{}(idw.getData().id);
      }
    };

    // Backwards compatibility with when BWAPI::Unit etc were pointers
    constexpr Interface *operator->() { return static_cast<Interface *>(this); }
    constexpr Interface const *operator->() const { return static_cast<Interface const *>(this); }
    constexpr explicit operator IdT() const { return getData().id; }
  };

  struct IDCompare {
    using is_transparent = std::true_type;
    template<typename Lhs, typename Rhs>
    auto operator()(Lhs const &lhs, Rhs const &rhs) const
    {
      return getID(lhs) < getID(rhs);
    }
  private:
    template<typename T, typename = std::void_t<>> struct hasIdT : std::false_type {};
    template<typename T> struct hasIdT<T, std::void_t<typename T::IdT>> : std::true_type {};

    template<typename T>
    auto getID(T const &val) const
    {
      if constexpr (!hasIdT<T>::value)
        return val.id;
      else
        return typename T::IdT{ val };
    }
  };
} // namespace BWAPI

