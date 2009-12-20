#pragma once
/**
 * Encapsulates a simple C array
 */

namespace Util
{
  template<typename T, unsigned int SIZE>
    struct StaticArray
    {
      typedef T ElementType;
      T data[SIZE];
      static const int size = SIZE;
    };
}
