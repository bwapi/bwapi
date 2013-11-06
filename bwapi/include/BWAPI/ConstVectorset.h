#pragma once
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <algorithm>

#include "Vectorset_iterator.h"

namespace BWAPI
{
  template<typename T>
  class Vectorset;

  /// The Vectorset is a class template designed specifically for trivial classes or PODs and 
  /// performance. It mimics the usage of various stl containers (specifically the Vector and Set)
  /// in order to replace them. The Vectorset is designed for BWAPI usage and is recommended, 
  /// especially if stl containers are causing a bottleneck in your bot's code.
  ///
  /// @warning The Vectorset should only be used with objects that have a trivial destructor and 
  /// trivial copy constructor. Objects in the Vectorset should not do any memory management 
  /// or the behaviour will be undefined. Use with caution.
  ///
  /// @note The Vectorset will only free memory when the object is destroyed.
  ///
  /// @see std::vector, std::set
  template<typename T>
  class ConstVectorset
  {
#ifndef SWIG
    static_assert(
#ifdef _MSC_VER
      std::tr1::has_trivial_copy<T>::value &&
      std::tr1::has_trivial_copy_constructor<T>::value &&
      std::tr1::has_trivial_destructor<T>::value,
#else
      std::is_trivially_copyable<T>::value &&
      std::is_trivially_constructible<T>::value &&
      std::is_trivially_destructible<T>::value,
#endif
      "Vectorset can only be used with classes that have a trivial destructor and trivial copy constructor.");
#endif

  private:
    ConstVectorset();
  public:
    /// The iterator is used to traverse the elements in the set. It is used the same way as in
    /// the stl containers.
    typedef BWAPI::VSetIterator<T> iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

  // ----------------------------------------------------------------- Constructors
    /// This constructor uses an existing array of objects and copies them into the vector.
    /// The Vectorset will allocate only enough memory to copy the array's contents.
    ///
    /// @param pArray
    ///   Pointer to an array of objects of type T.
    /// @param arrSize
    ///   The number of elements contained in the given array.
    ///
    /// @note Duplicate entries are not removed.
    ConstVectorset(const void *pArray, size_t arrSize = 0)
      : pStartArr( (T*)pArray )
      , pEndArr( (T*)pArray + arrSize )
    {};
    
    ConstVectorset(const ConstVectorset &other)
      : pStartArr(other.pStartArr)
      , pEndArr(other.pEndArr)
    {}

  // ----------------------------------------------------------------- Operators
    /// Creates a new Vectorset containing all elements of the current Vectorset with all elements
    /// of the other Vectorset appended on to it.
    ///
    /// @param other
    ///   The other Vectorset of the same type to use in combination.
    ///
    /// @returns A new Vectorset containing the contents of both this and other.
    ///
    /// @note Duplicate entries are not removed.
    ///
    /// @see operator|
    Vectorset<T> operator +(const ConstVectorset<T> &other) const
    {
      Vectorset<T> vcopy(this->size() + other.size());
      vcopy.push_back(*this);
      vcopy.push_back(other);
      return vcopy;
    };
    Vectorset<T> operator +(const T &val) const
    {
      Vectorset<T> vcopy(this->size() + 1);
      vcopy.push_back(*this);
      vcopy.push_back(val);
      return vcopy;
    };

    /// Creates a new Vectorset containing all elements of the current Vectorset and all elements
    /// of the other Vectorset with duplicates removed.
    ///
    /// @param other
    ///   The other Vectorset of the same type to use in combination.
    ///
    /// @returns A new Vectorset containing the contents of both this and other.
    Vectorset<T> operator |(const ConstVectorset<T> &other) const
    {
      Vectorset<T> vcopy(this->size() + other.size());
      vcopy.push_back(*this);
      vcopy.insert(other);
      return vcopy;
    };
    Vectorset<T> operator |(const T &val) const
    {
      Vectorset<T> vcopy(this->size() + 1);
      vcopy.push_back(*this);
      vcopy.insert(val);
      return vcopy;
    };
    
    /// Compares this Vectorset with another Vectorset of the same type.
    /// 
    /// @param other
    ///   A Vectorset of the same type to use for comparison.
    ///
    /// @retval true if the other Vectorset is exactly the same as this one.
    /// @retval false if at least one element in this Vectorset is not found in the other, or if
    /// they are not in the exact same positions.
    ///
    /// @note This simply calls memcmp.
    bool operator ==(const ConstVectorset<T> &other) const
    {
      if ( this->empty() && other.empty() )
        return true;

      if ( this->size() != other.size() )
        return true;

      return memcmp( this->pStartArr, other.pStartArr, this->size()*sizeof(T) ) == 0;
    };
    
    /// Retrieves a pointer to the raw data in the Vectorset.
    ///
    /// @note Vectorset data is simply an array.
    ///
    /// @returns A pointer to the Vectorset's array data.
    inline operator void*() const
    {
      return this->pStartArr;
    };

    /// Used as a macro to check if the Vectorset is empty.
    ///
    /// @retval true if the Vectorset is not empty.
    /// @retval false if the Vectorset is empty.
    inline operator bool() const
    {
      return !this->empty();
    };

    /// Array indexing operator.
    /// 
    /// @TODO Needs to be modified
    ///
    /// @param index
    ///   The array index in the Vectorset to retrieve the value from.
    inline T operator [](unsigned int index) const
    {
      if ( index < this->size() )
        return this->pStartArr[index];
      return pStartArr[0];
    };

  // ----------------------------------------------------------------- Custom const functions
    /// This function checks if an element exists in the Vectorset.
    ///
    /// @param element
    ///   The value to search for in the Vectorset.
    ///
    /// @retval true if the element is in the Vectorset.
    /// @retval false if the element was not found.
    bool exists(const T &element) const
    {
      return this->find(element) != this->end();
    };
    
    /// This macro is used to choose a random value from a Vectorset. It returns a default T
    /// value if the Vectorset is empty.
    ///
    /// @TODO needs to be modified
    ///
    /// @note This function calls the rand() function. A call to srand() should be used for
    /// initialization.
    ///
    /// @see rand()
    T rand() const
    {
      size_t size = this->size();
      switch ( size )
      {
      case 0:
        return T();
      case 1:
        return this->pStartArr[0];
      case 2:
        return this->pStartArr[::rand()%2];
      case 4:
        return this->pStartArr[::rand()%4];
      case 8:
        return this->pStartArr[::rand()%8];
      case 16:
        return this->pStartArr[::rand()%16];
      case 32:
        return this->pStartArr[::rand()%32];
      case 64:
        return this->pStartArr[::rand()%64];
      case 128:
        return this->pStartArr[::rand()%128];
      case 256:
        return this->pStartArr[::rand()%256];
      case 512:
        return this->pStartArr[::rand()%512];
      case 1024:
        return this->pStartArr[::rand()%1024];
      case 2048:
        return this->pStartArr[::rand()%2048];
      }
      return this->pStartArr[::rand()%size];
    };
    
    /// Iterates the Vectorset and retrieves the best entry using two callback procedures. The
    /// first returning the value to compare, the second being a binary comparison.
    ///
    /// @param cmpValue
    ///   A functor taking one argument, T, and returning a value to compare.
    ///
    /// @param cmpProc
    ///   A functor taking two values, (the ones returned by cmpValue), and returns a boolean
    ///   indicating that the first value passed in is the new best value.
    ///
    /// @retval NULL If the Vectorset is empty.
    /// @returns A T representing the best in the Vectorset.
    template < typename _V, typename _C >
    T getBest(const _V &cmpValue, const _C &cmpProc) const
    {
      // Return if empty
      if ( this->empty() )
        return NULL;
      
      // retrieve a value as the placeholder for the "best"
      T best = this->front();
      int bestVal = cmpValue(best);

      // Iterate all (remaining) elements
      for ( auto i = this->begin()+1; i != this->end(); ++i )
      {
        // Retrieve new value
        int newVal = cmpValue(*i);

        // Set as new best if new value > best
        if ( cmpProc(newVal, bestVal) )
        {
          bestVal = newVal;
          best = *i;
        }
      }

      return best;
    };
    /// @copydoc getBest
    /// @see getBest
    template < typename _V >
    T most(const _V &cmpValue) const
    {
      return this->getBest( std::forward<_V>(cmpValue), [](const int &v1, const int &v2)->bool{ return v1 > v2; } );
    };
    /// @copydoc getBest
    /// @see getBest
    template < typename _V >
    T least(const _V &cmpValue) const
    {
      return this->getBest( std::forward<_V>(cmpValue), [](const int &v1, const int &v2)->bool{ return v1 < v2; } );
    };
    
    /// Calculates a total by applying a functor to each element and adding what the functor
    /// returns.
    ///
    /// @param valProc
    ///   A unary functor that takes T as a parameter and returns the integer used to add to
    ///   the total.
    ///
    /// @returns An integer representing the sum of results from \p valProc applied to every
    /// element in the Vectorset.
    template < typename _V >
    int total(const _V &valProc) const
    {
      int sum = 0;
      for ( auto i = this->begin(); i != this->end(); ++i )
        sum += valProc(*i);
      return sum;
    };
  // ----------------------------------------------------------------- stl spinoff const functions
    /// Retrieves the number of elements currently in the Vectorset.
    ///
    /// @returns The number of elements contained in this Vectorset.
    ///
    /// @note For efficiency it is recommended to use empty() to check if the Vectorset has 0
    /// elements.
    ///
    /// @see empty
    inline size_t size() const
    {
      return ((size_t)this->pEndArr - (size_t)this->pStartArr)/sizeof(T);
    };
    
    /// Checks if the Vectorset is empty.
    ///
    /// @retval true if the Vectorset is empty.
    /// @retval false if the Vectorset contains elements.
    inline bool empty() const
    {
      return this->pEndArr == this->pStartArr;
    };

    // iterators
    inline iterator begin() const
    {
      return this->pStartArr;
    };
    inline reverse_iterator rbegin() const
    {
      return this->pEndArr - 1;
    };
    inline iterator end() const
    {
      return this->pEndArr;
    };
    inline reverse_iterator rend() const
    {
      return this->pStartArr - 1;
    };
    inline T front() const
    {
      return *this->begin();
    };
    inline T back() const
    {
      return *this->rbegin();
    };

  // ----------------------------------------------------------------- stl macros
    inline iterator find(const T &value) const
    {
      return std::find(this->begin(), this->end(), value);
    };

  // -----------------------------------------------------------------
  protected:
    // Variables
    T *pStartArr;  // ptr to beginning of array
    T *pEndArr;    // ptr to last element + 1
  };

}

