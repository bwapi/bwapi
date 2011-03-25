#pragma once

#include <windows.h>

#include "BW/Offsets.h"

BOOL  __stdcall  _SNetLeaveGame(int type);
int   __cdecl    _nextFrameHook();
int   __stdcall  _SStrCopy(char *dest, const char *source, size_t size);
BOOL  __stdcall  _SNetReceiveMessage(int *senderplayerid, u8 **data, int *databytes);
void  __stdcall  DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds);
void  __stdcall  DrawDialogHook(BW::bitmap *pSurface, BW::bounds *pBounds);
BOOL  __stdcall  _SFileAuthenticateArchive(HANDLE hArchive, DWORD *dwReturnVal);
BOOL  __stdcall  _SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);
BOOL  __stdcall  _SFileOpenFile(const char *filename, HANDLE *phFile);
BOOL  __stdcall  _SFileOpenArchive(const char *szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE *phMpq);
void *__stdcall  _SMemAlloc(int amount, char *logfilename, int logline, char defaultValue);
BOOL  __stdcall  _SNetSendTurn(char *data, unsigned int databytes);
void  __fastcall CommandFilter(BYTE *buffer, DWORD length);

BOOL STORMAPI _SDrawCaptureScreen(const char *pszOutput);

extern bool hideHUD;
extern DWORD lastTurnTime;
extern DWORD lastTurnFrame;
extern double botAPM_noSelect;
extern double botAPM_select;
extern bool wantRefresh;
extern char gszScreenshotFormat[4];

