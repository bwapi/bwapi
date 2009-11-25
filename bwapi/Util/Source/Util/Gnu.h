#pragma once

#ifdef __GNUC__
#include <string.h>

#define fprintf_s fprintf
#define sprintf_s snprintf
#define vsnprintf_s(buf, s1, s2, fmt, ap) vsnprintf(buf, s1, fmt, ap)
#define memcpy_s(dest, ds, src, ss) memcpy(dest, src, ss)

inline void strcpy_s(char* dest, size_t size, char* src) {
    size_t s = strlen(src);
    if(s > size - 1) {
        s = size - 1;
    }
    memcpy(dest, src, s);
    dest[s] = 0;
}
#endif

