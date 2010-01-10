#pragma once

/**
 *  A string limited to BUFFER_SIZE, as a static structure
 *  has functions to better enforce the limit
 */

#include <string>

namespace Util
{
  template<int BUFFER_SIZE> struct StaticString
  {
    static const int bufferSize = BUFFER_SIZE;

    char buffer[bufferSize];

    // limit obiding functions
    const char* c_str() const
    {
      return buffer;
    }

    operator const char*() const
    {
      return buffer;
    }

    void set(const char* source)
    {
      strcpy_s(buffer, bufferSize, source);
    }

    void set(const std::string &source)
    {
      set(source.c_str());
    }

    bool isEmpty() const
    {
      return buffer[0] == 0;
    }
  };
}
