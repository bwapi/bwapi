#pragma once
/**
 * Forked from Engine.cpp
 * draws grapchics based on BW's drawing capabilities
 */

namespace BWAPI
{
  void drawLine(int x1, int y1, int x2, int y2, int color);
  void drawCircle(int x1, int y1, double r, int color, bool isSolid);
  void drawRectangle(int left, int top, int width, int height, int color, bool isSolid);
};
