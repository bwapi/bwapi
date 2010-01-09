#pragma once

/**
 *  A structure with a static array and a count, assuming the first count entries are valid
 */

#include "Types.h"

namespace Util
{
  template<typename T, int SIZE>
  struct StaticVector
  {
    typedef T ElementType;
    static const int size = SIZE;

    u32 count;
    T data[SIZE];

    // make this an std compliant container
    typedef T* iterator;

    const T* begin() const
    {
      return &data[0];
    }

    const T* end() const
    {
      return &data[count];
    }

    T& at(int index)
    {
      if(index < 0 || index > (int)count)
        throw GeneralException("index out of bound");
      return data[index];
    }

    const T& at(int index) const
    {
      if(index < 0 || index > (int)count)
        throw GeneralException("index out of bound");
      return data[index];
    }

    void clear()
    {
      count = 0;
    }

    void push_back(const T& item)
    {
      if(count >= size)
        throw GeneralException("pushed past bound");
      data[count] = item;
      count++;
    }

    T& push_back()
    {
      if(count >= size)
        throw GeneralException("pushed past bound");
      T& retval = data[count];
      count++;
      return retval;
    }

    const T& pop_back()
    {
      if(count <= 0)
        throw GeneralException("popped empty StaticVector");
      count--;
      return data[count];
    }
  };
}
