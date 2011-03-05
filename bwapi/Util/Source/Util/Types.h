#pragma once

typedef unsigned char      u8;
typedef signed char        s8;

typedef unsigned short     u16;
typedef signed short       s16;

typedef unsigned int       u32;
typedef signed int         s32;

typedef unsigned long long u64;
typedef signed long long s64;

typedef u8              _UNKNOWN;

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef void*           PVOID;
typedef int             BOOL;
typedef void*           HANDLE;

typedef struct
{
  s16 Xmin;
  s16 Ymin;
  s16 Xmax;
  s16 Ymax;
} rect;

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#define ever (;;)

#define vstretchyprintf(buff,fmt) {\
  va_list ap;\
  va_start(ap, fmt);\
  int buffsize = _vscprintf(fmt, ap);\
  buff = new char[buffsize+1];\
  vsprintf_s(buff, buffsize+1, fmt, ap);\
  va_end(ap);\
}

#define MB_ICONINFO MB_ICONASTERISK