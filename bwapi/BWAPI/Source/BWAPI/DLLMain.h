#pragma once
#include <BWAPI/CoordinateType.h>
#include <windows.h>
void NewIssueCommand();
void drawDot(int _x, int _y, int color, int ctype);
void drawBox(int _x, int _y, int _w, int _h, int color, int ctype);
void drawText(int _x, int _y, const char* ptext, int ctype);
static char logPath[MAX_PATH];
static bool logging;
