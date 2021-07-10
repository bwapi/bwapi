#pragma once
#include <BWAPI/SetContainer.h>

#include <string>
#include <cctype>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace BWAPI
{
  template<class T, int UnknownId> class Type;

  /// <summary>Base superclass for all BWAPI Types.</summary>
  ///
  /// @tparam T
  ///     The type class that this Type is representing.
  ///
  /// @tparam UnknownId
  ///     The enum ID of the unknown entry, which is also treated as the last entry for type
  ///     validity.
  template<class T, int UnknownId>
  class Type
  {
  protected:
    /// @cond HIDDEN
    /// <summary>Primitive storage unit for the type identifier (type id).</summary>
    int tid;

    /// <summary>Array of strings containing the type names.</summary>
    static const std::string typeNames[UnknownId+1];

    /// <summary>Type that maps names to their type values.</summary>
    typedef std::unordered_map<std::string,T> typeMapT;
    /// @endcond
  private:
    static typeMapT typeMapInit()
    {
      typeMapT result(UnknownId+1);
      for ( int i = 0; i < UnknownId + 1; ++i ) // include unknown
      {
        std::string n( typeNames[i] );

        // erase-remove idiom, eliminates spaces and underscores from the string
        n.erase( std::remove_if(n.begin(), n.end(), [](char const &c){ return isspace(c) || c == '_'; }), n.end() );

        // Make lowercase
        std::transform(n.begin(), n.end(), n.begin(), 
          [](std::string::const_reference c) { return static_cast<char>(std::tolower(c)); }
        );

        result.insert( typename typeMapT::value_type(n, T(i)) );
      }
      return result;
    }
  public:
    static constexpr int UnkID = UnknownId;
    using BWAPIHashCast = int;
    /// <summary>Expected type constructor.</summary> If the type is an invalid type, then it
    /// becomes Types::Unknown. A type is invalid if its value is less than 0 or greater than
    /// Types::Unknown.
    ///
    /// <param name="id">
    ///   The id that corresponds to this type. It is typically an integer value that corresponds
    ///   to an internal Broodwar type. If the given id is invalid, then it becomes Types::Unknown.
    /// </param>
    explicit constexpr Type(int id) : tid( id < 0 || id > UnknownId ? UnknownId : id ) {};

    /// <summary>A set type that contains the current type.</summary>
    using set = SetContainer<T>;

    /// <summary>A list type that contains the current type.</summary>
    using list = std::vector<T>;

    /// <summary>Conversion/convenience operator to convert this type to its primitive type.</summary>
    ///
    /// @returns An integer representation of this type.
    constexpr operator int() const { return tid; };

    /// <summary>Conversion/convenience operator to validate this type.</summary>
    ///
    /// @returns The validity of the type.
    /// @see isValid
    constexpr explicit operator bool() const { return isValid(); }
    constexpr bool operator==(Type const &other) { return tid == other.tid; }
    constexpr bool operator!=(Type const &other) { return tid != other.tid; }

    /// <summary>Retrieves this type's identifier as an integer.</summary>
    ///
    /// @returns An integer representation of this type.
    constexpr int getID() const { return this->tid; };

    /// <summary>Checks if the current type has a valid identifier.</summary> The purpose of
    /// this function is to prevent buffer overflows if a type has been handled improperly.
    ///
    /// A type is valid if it is between 0 and Unknown (inclusive).
    ///
    /// @returns true If this type is valid and false otherwise.
    constexpr bool isValid() const { return this->tid >= 0 && this->tid <= UnknownId; };

    /// <summary>Retrieves the variable name of the type.</summary>
    ///
    /// @returns Reference to std::string object containing the name.
    inline const std::string &getName() const
    {
      return typeNames[this->isValid() ? this->tid : UnknownId];
    };

    /// @copydoc Type::getName
    inline const std::string &toString() const
    {
      return this->getName();
    };

    /// <summary>Retrieves the variable name of the type as a c-style string.</summary> Meant to
    /// be a convenience member.
    ///
    /// @returns Pointer to constant c-style string containing the name.
    inline const char *c_str() const
    {
      return this->getName().c_str();
    };

    /// <summary>Output stream operator overload.</summary> Allows printing of the type without
    /// calling Type::getName.
    ///
    /// <param name="os">
    ///     The destination output stream.
    /// </param>
    /// <param name="t">
    ///     The type to write as a string.
    /// </param>
    friend inline std::ostream &operator << (std::ostream &os, const Type<T, UnknownId> &t)
    {
      return os << t.getName();
    };
    /// @overload
    friend inline std::wostream &operator << (std::wostream &os, const Type<T, UnknownId> &t)
    {
      std::wstring wideName{ t.getName().begin(), t.getName().end() };
      return os << wideName;
    };

    /// <summary>Searches for the type associated with the given string and returns it.</summary>
    ///
    /// <param name="name">
    ///     A string containing the name of the type.
    /// </param>
    /// @returns The type that resolves to the given name.
    static T getType(std::string name)
    {
      // Mapping of strings to types
      static const typeMapT typeMap( typeMapInit() );

      // erase-remove idiom, eliminates spaces and underscores from the string to search
      name.erase( std::remove_if(name.begin(), name.end(), [](char const &c){ return isspace(c) || c == '_'; }), name.end() );

      // Make lowercase
      std::transform(name.begin(), name.end(), name.begin(),
        [](std::string::const_reference c) { return static_cast<char>(std::tolower(c)); }
      );

      // Find the type
      auto it = typeMap.find(name);
      if ( it != typeMap.end() )
        return it->second;

      // Return unknown if it wasn't found
      return T(UnknownId);
    };

    struct Hash {
      auto operator()(Type tp) const {
        return std::hash<int>{}(tp.getID());
      }
    };
  };

} // namespace BWAPI
