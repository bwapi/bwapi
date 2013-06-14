#pragma once
#include <iterator>

#define _ITER_COMPARE(op) friend bool operator op(const VSetIterator<T> &lhs, const VSetIterator<T> &rhs) \
                          { return lhs.pVal op rhs.pVal; };

namespace BWAPI
{
  /// The iterator class template allows the iteration
  /// of elements of a Vectorset with ease while
  /// maintaining the compatibility with any familiar
  /// STL container iteration.
  ///
  /// @see Vectorset
  template<typename T>
  class VSetIterator : public std::iterator<std::random_access_iterator_tag, T>
  {
  public:
    // constructors
    VSetIterator(pointer ptr = nullptr) : pVal(ptr) {};
    VSetIterator(const VSetIterator<T> &other) : pVal(&other) {};

    // Copy assignment
    VSetIterator<T> &operator =(const VSetIterator<T> &other)
    {
      this->pVal = other.pVal;
      return *this;
    };

    // comparison operators
    _ITER_COMPARE(==);
    _ITER_COMPARE(!=);
    _ITER_COMPARE(<);
    _ITER_COMPARE(>);
    _ITER_COMPARE(<=);
    _ITER_COMPARE(>=);

    // modification operators
    VSetIterator<T> &operator ++()
    {
      ++pVal;
      return *this;
    };
    VSetIterator<T> operator ++(int)
    {
      VSetIterator<T> copy(*this);
      ++pVal;
      return copy;
    };
    VSetIterator<T> &operator --()
    {
      --pVal;
      return *this;
    };
    VSetIterator<T> operator --(int)
    {
      VSetIterator<T> copy(*this);
      --pVal;
      return copy;
    };
    VSetIterator<T> operator +(const difference_type &n) const
    {
      return VSetIterator(this->pVal + n);
    };
    VSetIterator<T> operator -(const difference_type &n) const
    {
      return VSetIterator(this->pVal - n);
    };
    friend difference_type operator -(const VSetIterator<T> &lhs, const VSetIterator<T> &rhs)
    {
      return lhs.pVal - rhs.pVal;
    };

    VSetIterator &operator +=(const difference_type &n)
    {
      this->pVal += n;
      return *this;
    };
    VSetIterator &operator -=(const difference_type &n)
    {
      this->pVal -= n;
      return *this;
    };

    // Dereference operators
    reference operator *() const
    {
      return *pVal;
    };
    // @TODO: should be "pointer" and "return pVal", need specialization
    reference operator ->() const
    {
      return *pVal;
    };
    reference operator [](const difference_type &n) const
    {
      return this->pVal[n];
    };


    // @TODO: remove
    T *operator &() const
    {
      return pVal;
    };
  protected:
    pointer pVal;
  };


}
