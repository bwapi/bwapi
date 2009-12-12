#pragma once

#include <Util\Types.h>

namespace BW
{
  extern DWORD installHooks();

  // commands
  void issueCommandFromMemory(void *pbBuffer, int iSize);
  template<typename T>
    void issueCommand(T &cmdStruct)
    {
      issueCommandFromMemory(&cmdStruct, sizeof(T));
    }

  // draw functions
  extern void drawBox(int x, int y, int w, int h, int color);
  extern void drawText(int x, int y, const char* ptext);
  static void inline drawDot(int x, int y, int color)
  {
    drawBox(x, y, 1, 1, color);
  }

  extern void printEx(s32 pID, const char* text, ...);
  extern void sendText(const char* text, ...);
  extern void printf(const char* text, ...);
}
