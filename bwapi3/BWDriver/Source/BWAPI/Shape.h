#pragma once
/**
 * Forked from Engine.cpp
 * draws grapchics based on BW's drawing capabilities
 */

namespace BWAPI
{
  extern void drawLine(int x1, int y1, int x2, int y2, int color);
  extern void drawCircle(int x, int y, double r, int color, bool isSolid);
  extern void drawEllipse(int x, int y, int w, int h, int color, bool isSolid);
  extern void drawRectangle(int left, int top, int width, int height, int color, bool isSolid);
  extern void drawTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color, bool isSolid);
};
