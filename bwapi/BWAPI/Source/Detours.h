#pragma once

#include <windows.h>
#include <string>
#include <storm.h>

namespace BW
{
  namespace Triggers
  {
    class Action;
  }
  class Bitmap;
  struct bounds;
}

#define DECL_OLDFXN(x) decltype(&x) _ ## x ## Old

BOOL  STORMAPI   _SNetLeaveGame(int type);
int   __cdecl    _nextFrameHook();
int   STORMAPI   _SStrCopy(char *dest, const char *source, int size);
BOOL  STORMAPI   _SNetReceiveMessage(int *senderplayerid, char **data, int *databytes);
void  __stdcall  DrawHook(BW::Bitmap *pSurface, BW::bounds *pBounds);
void  __stdcall  DrawDialogHook(BW::Bitmap *pSurface, BW::bounds *pBounds);
BOOL  STORMAPI   _SFileAuthenticateArchive(HANDLE hArchive, DWORD *dwReturnVal);
BOOL  STORMAPI   _SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);
BOOL  STORMAPI   _SFileOpenFile(const char *filename, HANDLE *phFile);
void *STORMAPI   _SMemAlloc(size_t amount, char *logfilename, int logline, char defaultValue);
BOOL  STORMAPI   _SNetSendTurn(char *data, unsigned int databytes);
void  __fastcall CommandFilter(BYTE *buffer, DWORD length);
void __stdcall ExecuteGameTriggers(DWORD dwMillisecondsPerFrame);

BOOL STORMAPI _SDrawCaptureScreen(const char *pszOutput);

HANDLE WINAPI _FindFirstFile(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
BOOL   WINAPI _DeleteFile(LPCSTR lpFileName);
DWORD  WINAPI _GetFileAttributes(LPCSTR lpFileName);
HANDLE WINAPI _CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HWND   WINAPI _CreateWindowEx(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
VOID   WINAPI _Sleep(DWORD dwMilliseconds);
HANDLE WINAPI _CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize,LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
HANDLE WINAPI _CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
void WINAPI _GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime);

extern DWORD lastTurnTime;
extern DWORD lastTurnFrame;
extern std::string gDesiredReplayName;
extern bool detourCreateWindow;

extern DECL_OLDFXN(SNetLeaveGame);
extern DECL_OLDFXN(SStrCopy);
extern DECL_OLDFXN(SNetReceiveMessage);
extern DECL_OLDFXN(SFileOpenFileEx);
extern DECL_OLDFXN(SFileOpenFile);
extern DECL_OLDFXN(SMemAlloc);
extern DECL_OLDFXN(SNetSendTurn);
extern DECL_OLDFXN(SDrawCaptureScreen);
extern DECL_OLDFXN(FindFirstFileA);
extern DECL_OLDFXN(DeleteFileA);
extern DECL_OLDFXN(GetFileAttributesA);
extern DECL_OLDFXN(CreateFileA);
extern DECL_OLDFXN(CreateWindowExA);
extern DECL_OLDFXN(Sleep);
extern DECL_OLDFXN(CreateThread);
extern DECL_OLDFXN(CreateEventA);
extern DECL_OLDFXN(GetSystemTimeAsFileTime);

void _RandomizePlayerRaces();
void _InitializePlayerConsole();
