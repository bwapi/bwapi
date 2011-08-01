#pragma once

#include <windows.h>

#include "BW/Offsets.h"

void __stdcall DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds);

BOOL  STORMAPI   _SNetLeaveGame(int type);
int   __cdecl    _nextFrameHook();
int   STORMAPI   _SStrCopy(char *dest, const char *source, size_t size);
BOOL  STORMAPI   _SNetReceiveMessage(int *senderplayerid, u8 **data, int *databytes);
void  __stdcall  DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds);
void  __stdcall  DrawDialogHook(BW::bitmap *pSurface, BW::bounds *pBounds);
BOOL  STORMAPI   _SFileAuthenticateArchive(HANDLE hArchive, DWORD *dwReturnVal);
BOOL  STORMAPI   _SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);
BOOL  STORMAPI   _SFileOpenFile(const char *filename, HANDLE *phFile);
void *STORMAPI   _SMemAlloc(int amount, char *logfilename, int logline, char defaultValue);
BOOL  STORMAPI   _SNetSendTurn(char *data, unsigned int databytes);
void  __fastcall CommandFilter(BYTE *buffer, DWORD length);
void __declspec() _spendRepairHook();

BOOL STORMAPI _SDrawCaptureScreen(const char *pszOutput);

HANDLE WINAPI _FindFirstFile(LPCSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
BOOL   WINAPI _DeleteFile(LPCTSTR lpFileName);
DWORD  WINAPI _GetFileAttributes(LPCTSTR lpFileName);
HANDLE WINAPI _CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HWND   WINAPI _CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
VOID   WINAPI _Sleep(DWORD dwMilliseconds);

extern bool hideHUD;
extern DWORD lastTurnTime;
extern DWORD lastTurnFrame;
extern double botAPM_noSelect;
extern double botAPM_select;
extern bool wantRefresh;
extern char gszScreenshotFormat[4];
extern char gszDesiredReplayName[MAX_PATH];
extern bool detourCreateWindow;

extern BOOL   (STORMAPI *_SNetLeaveGameOld)(int type);
extern int    (STORMAPI *_SStrCopyOld)(char *dest, const char *source, size_t size);
extern BOOL   (STORMAPI *_SNetReceiveMessageOld)(int *senderplayerid, u8 **data, int *databytes);
extern BOOL   (STORMAPI *_SFileOpenFileExOld)(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);
extern BOOL   (STORMAPI *_SFileOpenFileOld)(const char *filename, HANDLE *phFile);
extern void*  (STORMAPI *_SMemAllocOld)(int amount, char *logfilename, int logline, char defaultValue);
extern BOOL   (STORMAPI *_SNetSendTurnOld)(char *data, unsigned int databytes);
extern BOOL   (STORMAPI *_SDrawCaptureScreenOld)(const char *pszOutput);
extern HANDLE (WINAPI   *_FindFirstFileOld)(LPCSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
extern BOOL   (WINAPI   *_DeleteFileOld)(LPCTSTR lpFileName);
extern DWORD  (WINAPI   *_GetFileAttributesOld)(LPCTSTR lpFileName);
extern HANDLE (WINAPI   *_CreateFileOld)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
extern HWND   (WINAPI   *_CreateWindowExAOld)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
extern VOID   (WINAPI   *_SleepOld)(DWORD dwMilliseconds);

bool __fastcall TriggerActionReplacement(BW::Triggers::Action *pAction);
extern bool (__fastcall *BWTriggerActionFxnTable[60])(BW::Triggers::Action*);
