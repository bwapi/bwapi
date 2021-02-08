#pragma once

#include <cstring>
#include <type_traits>
#include <string>
#include <algorithm>

template <size_t N>
inline void StrTerminate(char (&str)[N])
{
  str[N-1] = '\0';
}

template <size_t N>
inline void VSNPrintf(char (&dst)[N], const char *fmt, va_list ap)
{
  vsnprintf(dst, N-1, fmt, ap);
  StrTerminate(dst);
}
