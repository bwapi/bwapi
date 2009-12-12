#pragma once

#include <Util\Types.h>

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
  
}
