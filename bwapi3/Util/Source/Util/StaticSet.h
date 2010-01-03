#pragma once

/**
 *  An array of entries with busy flags. Can be used:
 *  - as a set with integer keys below SIZE
 *  - as a slot array allocator / chunk memory manager
 *
 */

#include "Types.h"

#include <stdexcept>

namespace Util
{
  template<typename T, int SIZE>
  struct StaticSet
  {
  private:
    struct Entry
    {
      T item;
      bool valid;
    };
  public:
    typedef T ElementType;
    static const int size = SIZE;

    u32 count;
    u32 head;   // used to optimize allocations in large sets
    Entry data[SIZE];

    // make this an std compliant container
    class const_iterator
    {
    private:
      int slotsLeftCount;
      const Entry *pointer;
    public:
      void operator++()
      {
        if(!slotsLeftCount)
          throw out_of_range("StaticSet::iterator::++ past end");
        pointer++;
        slotsLeftCount--;
      }
      const Entry *operator*()
      {
        return pointer->item;
      }
    };

    const_iterator begin() const
    {
      return &data[0];
    }

    const_iterator end() const
    {
      return &data[count];
    }

    // functions
    void reset()
    {
      for(int i = 0;i < size; i++)
        data[i].valid = false;
    }

    const T& at(int index) const
    {
      if(index < 0 || index > count)
        throw GeneralException("index out of bound");
      return data[index];
    }

    T& at(int index)
    {
      if(index < 0 || index > count)
        throw GeneralException("index out of bound");
      return data[index];
    }
  };
}
