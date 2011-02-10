#pragma once
#include <windows.h>
#include "BW/Offsets.h"

void __fastcall QueueGameCommand(BYTE *buffer, DWORD length);
void drawLine(int _x1, int _y1, int _x2, int _y2, int color, int ctype);
void drawDot(int _x, int _y, int color, int ctype);
void drawBox(int _x, int _y, int _w, int _h, int color, int ctype);
void drawText(int _x, int _y, const char* ptext, int ctype, char size);
void __stdcall DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds);

void BWAPIError(const char *format, ...);
void BWAPIError(DWORD dwErrCode, const char *format, ...);

extern char logPath[MAX_PATH];
extern bool logging;

extern DWORD gdwProcNum;
extern char szInstallPath[MAX_PATH];
extern char szConfigPath[MAX_PATH];
