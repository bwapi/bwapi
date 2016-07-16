#pragma once
#include <windows.h>
#include <storm.h>

static const int WMODE_MIN_WIDTH  = 200;
static const int WMODE_MIN_HEIGHT = 150;
static const int WMODE_SNAP_RANGE = 20;

struct BITMAPINFO256 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[256];
};

extern BITMAPINFO256 wmodebmp;

BOOL WINAPI   _GetCursorPos(LPPOINT lpPoint);
BOOL WINAPI   _SetCursorPos(int X, int Y);
BOOL WINAPI   _ClipCursor(const RECT *lpRect);
BOOL STORMAPI _SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused);
BOOL STORMAPI _SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect);
BOOL STORMAPI _SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);
BOOL STORMAPI _SDrawRealizePalette();

extern BOOL (WINAPI   *_GetCursorPosOld)(LPPOINT lpPoint);
extern BOOL (WINAPI   *_SetCursorPosOld)(int X, int Y);
extern BOOL (WINAPI   *_ClipCursorOld)(const RECT *lpRect);
extern BOOL (STORMAPI *_SDrawLockSurfaceOld)(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused);
extern BOOL (STORMAPI *_SDrawUnlockSurfaceOld)(int surfacenumber, void *lpSurface, int a3, RECT *lpRect);
extern BOOL (STORMAPI *_SDrawUpdatePaletteOld)(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);
extern BOOL (STORMAPI *_SDrawRealizePaletteOld)();

void InitializeWModeBitmap(int width, int height);
void GetBorderRect(HWND hWnd, LPRECT lpRect);
void GetBorderSize(HWND hWnd, LPSIZE lpSize);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SetWMode(int width, int height, bool state);
void SetCursorShowState(bool bShow);

extern WNDPROC  wOriginalProc;
extern HWND     ghMainWnd;
extern HDC      hdcMem;
extern void*    pBits;
extern RGBQUAD  palette[256];
extern bool     wmode;

extern bool recordingUpdated;

extern bool switchToWMode;
extern RECT windowRect;

