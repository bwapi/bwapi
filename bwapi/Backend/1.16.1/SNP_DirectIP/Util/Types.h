#pragma once
#include <cstdint>

typedef uint8_t  u8;
typedef int8_t   s8;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint32_t u32;
typedef int32_t  s32;
typedef uint64_t u64;
typedef int64_t  s64;

typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef void*           PVOID;
typedef int             BOOL;
typedef void*           HANDLE;

typedef struct __point
{
  long x, y;
} point;

typedef struct __points
{
  short x, y;
} points;

typedef struct __rect
{
  s16 left, top, right, bottom;
} rect;
