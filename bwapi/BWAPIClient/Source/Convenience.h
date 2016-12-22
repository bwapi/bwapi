#include <algorithm>
#include <cstring>
#include <type_traits>
#include <string>

template <typename T>
inline void MemZero(T (&data))
{
  static_assert(!std::is_pointer<T>::value, "MemZero cannot be used with pointer types!");
  memset(&data, 0, sizeof(data));
};

template <size_t N>
inline void StrTerminate(char (&str)[N])
{
  str[N-1] = '\0';
};

template <size_t N>
inline void StrCopy(char (&dst)[N], const char *src)
{
  strncpy(dst, src, N - 1);
  StrTerminate(dst);
};

template <size_t N>
inline void StrCopy(char (&dst)[N], const std::string &src)
{
  StrCopy(dst, src.c_str());
};

template <size_t N>
inline void VSNPrintf(char (&dst)[N], const char *fmt, va_list &ap)
{
  vsnprintf(dst, N-1, fmt, ap);
  StrTerminate(dst);
}
