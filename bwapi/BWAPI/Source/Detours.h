#pragma once

#include <windows.h>
#include <string>

#include "BW/Offsets.h"

#define DECL_OLDFXN(x) decltype(&x) _ ## x ## Old

void __stdcall DrawHook(BW::Bitmap *pSurface, BW::bounds *pBounds);

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

HANDLE WINAPI _FindFirstFile(LPCSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
BOOL   WINAPI _DeleteFile(LPCTSTR lpFileName);
DWORD  WINAPI _GetFileAttributes(LPCTSTR lpFileName);
HANDLE WINAPI _CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HWND   WINAPI _CreateWindowEx(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
VOID   WINAPI _Sleep(DWORD dwMilliseconds);
HANDLE WINAPI _CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize,LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);

extern bool hideHUD;
extern DWORD lastTurnTime;
extern DWORD lastTurnFrame;
extern double botAPM_noSelect;
extern double botAPM_select;
extern bool wantRefresh;
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
extern DECL_OLDFXN(FindFirstFile);
extern DECL_OLDFXN(DeleteFile);
extern DECL_OLDFXN(GetFileAttributes);
extern DECL_OLDFXN(CreateFile);
extern DECL_OLDFXN(CreateWindowEx);
extern DECL_OLDFXN(Sleep);
extern DECL_OLDFXN(CreateThread);

bool __fastcall TriggerActionReplacement(BW::Triggers::Action *pAction);
extern bool (__fastcall *BWTriggerActionFxnTable[60])(BW::Triggers::Action*);

void _RandomizePlayerRaces();
void _InitializePlayerConsole();
