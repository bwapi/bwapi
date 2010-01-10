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
  //extern void drawDot(int x, int y, int color);
  static void inline drawDot(int x, int y, int color)
  {
//    drawBox(x, y, 1, 1, color);
    struct ScreenBufferData
    {
      u16 width;
      u16 height;
      BYTE* buffer;
    };
    static const ScreenBufferData** screenBufferData = (const ScreenBufferData**)0x006CF4A8;

    if(x >= 0 && x < 640 && y >= 0 && y < 480)
      (*screenBufferData)->buffer[y*640 + x] = color;
  }

  extern void printEx(s32 pID, const char* text, ...);
  extern void sendText(const char* text, ...);
  extern void printf(const char* text, ...);
}
