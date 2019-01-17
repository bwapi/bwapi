#pragma once

#include <vector>

namespace BWAPI {
  template<typename T>
  struct Identifier {
    int id;
    constexpr explicit Identifier(int id_) : id{id_} { }
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
      auto operator()(Identifier id) {
        return std::hash<int>{}(static_cast<int>(id.getID()));
      }
    };

    using list = std::vector<Identifier>;
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
    auto operator()(BWAPI::Identifier<T> id) const {
      return BWAPI::Identifier<T>::Hash{}(id);
    }
  };
}
