#pragma once

void bwDrawLine(int _x1, int _y1, int _x2, int _y2, int color, int ctype);
void bwDrawDot(int  _x,  int _y, int color, int ctype);
void bwDrawBox(int  _x,  int _y, int _w, int _h, int color, int ctype);
void bwDrawText(int _x,  int _y, const char* ptext, int ctype, char size);

void bwDrawBoxEx(int _x, int _y, int _w, int _h, int borderColor, int borderThickness, int color, int ctype, int style, int intensity);
