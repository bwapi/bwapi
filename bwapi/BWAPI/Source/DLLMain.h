#pragma once
#include <BWAPI/CoordinateType.h>
#include <windows.h>
#include <ddraw.h>
#include "BW/Offsets.h"

struct BITMAPINFO256 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[256];
};

void __fastcall QueueGameCommand(BYTE *buffer, DWORD length);
void drawDot(int _x, int _y, int color, int ctype);
void drawBox(int _x, int _y, int _w, int _h, int color, int ctype);
void drawText(int _x, int _y, const char* ptext, int ctype, char size);
void __stdcall DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds);

void BWAPIError(const char *format, ...);
void BWAPIError(DWORD dwErrCode, const char *format, ...);

extern char logPath[MAX_PATH];
extern bool logging;

extern bool wantRefresh;
extern DWORD gdwProcNum;
extern DWORD lastTurnTime;
extern DWORD lastTurnFrame;
extern char szInstallPath[MAX_PATH];
extern char szConfigPath[MAX_PATH];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern WNDPROC wOriginalProc;
extern HWND ghMainWnd;
extern HDC  hdcMem;
extern void* pBits;
