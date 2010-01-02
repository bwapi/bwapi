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

    T* begin()
    {
      return &data[0];
    }

    T* end()
    {
      return &data[count];
    }

    T& at(int index)
    {
      if(index < 0 || index > count)
        throw GeneralException("index out of bound");
      return data[index];
    }
  };
}
