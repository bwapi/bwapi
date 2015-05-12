#pragma once

typedef char  CHAR;
typedef short SHORT;
typedef long  LONG;
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef int BOOL;
typedef void* HANDLE;
typedef void* PVOID;
typedef void* LPVOID;

#ifndef WINAPI
#define WINAPI __stdcall
#endif

#ifndef APIENTRY
#define APIENTRY WINAPI
#endif

#ifndef DLL_PROCESS_ATTACH
#define DLL_PROCESS_ATTACH   1
#define DLL_THREAD_ATTACH    2
#define DLL_THREAD_DETACH    3
#define DLL_PROCESS_DETACH   0
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
