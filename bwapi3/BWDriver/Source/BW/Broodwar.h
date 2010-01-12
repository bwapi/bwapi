#pragma once

#include "Position.h"

#include <Util\Types.h>

namespace BW
{
  extern int selfPlayerId;
//public:
  extern void onMatchInit();
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
  extern Position getScreenPos();
  extern int getLatency();
  extern void setLocalSpeed(int speed);
  extern void resetLocalSpeed();

  extern void setScreenPosition(int x, int y);
  extern void startGame();
  extern void pauseGame();
  extern void resumeGame();
  extern void leaveGame();
  extern void restartGame();
  
}
