#pragma once
#include <windows.h>

#include "BW/Offsets.h"

struct BITMAPINFO256 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[256];
};

BOOL WINAPI _GetCursorPos(LPPOINT lpPoint);
BOOL WINAPI _SetCursorPos(int X, int Y);
BOOL WINAPI _ClipCursor(const RECT *lpRect);
BOOL __stdcall _SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused);
BOOL __stdcall _SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect);
BOOL __stdcall _SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);
BOOL __stdcall _SDrawRealizePalette();

void InitializeWModeBitmap(int width, int height);
void GetBorderRect(HWND hWnd, LPRECT lpRect);
void GetBorderSize(HWND hWnd, LPSIZE lpSize);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern WNDPROC wOriginalProc;
extern HWND ghMainWnd;
extern HDC  hdcMem;
extern void* pBits;
extern RGBQUAD palette[256];
