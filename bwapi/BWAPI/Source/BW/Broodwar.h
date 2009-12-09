#pragma once

namespace BW
{
//public:
  extern bool isInReplay();
  extern bool isInGame();
  extern bool isMultiplayer();
  extern bool isSingleplayer();
  extern bool isInLobby();
  extern bool isPaused();
  extern int getMouseX();
  extern int getMouseY();
  extern int getScreenX();
  extern int getScreenY();
  extern int getLatency();

  extern void setScreenPosition(int x, int y);
  extern void startGame();
  extern void pauseGame();
  extern void resumeGame();
  extern void leaveGame();
  extern void restartGame();
  
  // commands
  void issueCommandFromMemory(void *pbBuffer, int iSize);
  template<typename T>
    void issueCommand(T &cmdStruct)
    {
      issueCommandFromMemory(&cmdStruct, sizeof(T));
    }

  // draw functions
  void drawBox(int x, int y, int w, int h, int color);
  void drawText(int x, int y, const char* ptext);
  static void inline drawDot(int x, int y, int color)
  {
    drawBox(x, y, 1, 1, color);
  }
}
