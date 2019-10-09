#pragma once
#include <BWAPI/CoordinateType.h>

void bwDrawLine(int _x1, int _y1, int _x2, int _y2, int color, BWAPI::CoordinateType::Enum ctype);
void bwDrawDot(int  _x,  int _y, int color, BWAPI::CoordinateType::Enum ctype);
void bwDrawBox(int  _x,  int _y, int _w, int _h, int color, BWAPI::CoordinateType::Enum ctype);
void bwDrawText(int _x,  int _y, const char* ptext, BWAPI::CoordinateType::Enum ctype, char size);

