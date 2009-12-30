#pragma once

/**
 *  A structure with a static array and a count, assuming the first count entries are valid
 */

#include "StaticArray.h"
#include "Types.h"

namespace Util
{
  template<typename T, int SIZE>
  struct LimitedArray
  {
    typedef T ElementType;
    static const int size = SIZE;

    u32 count;
    T data[SIZE];
  };
}
