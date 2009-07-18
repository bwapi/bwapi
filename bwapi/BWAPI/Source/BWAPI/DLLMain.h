#pragma once
#include <BWAPI/CoordinateType.h>
void NewIssueCommand();
void drawDot(int _x, int _y, int color, BWAPI::CoordinateType::Enum ctype);
void drawBox(int _x, int _y, int _w, int _h, int color, BWAPI::CoordinateType::Enum ctype);