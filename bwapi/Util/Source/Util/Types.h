#pragma once

typedef unsigned char      u8;
typedef   signed char      s8;
typedef unsigned short     u16;
typedef   signed short     s16;
typedef unsigned int       u32;
typedef   signed int       s32;
typedef unsigned long long u64;
typedef   signed long long s64;
typedef unsigned int       uint;

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef void*           PVOID;
typedef int             BOOL;
typedef void*           HANDLE;

typedef struct __rect
{
  s16 left;
  s16 top;
  s16 right;
  s16 bottom;
} rect;
