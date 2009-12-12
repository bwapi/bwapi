#include "Broodwar.h"
#include "Hook.h"
#include "Command.h"
#include "Latency.h"
#include "Offsets.h"
#include "CheatFlags.h"
#include "CheatType.h"

#include <DLLMain.h>

#include <Util/StaticArray.h>

namespace BW
{
//public:
  //----------------------------------- IN REPLAY -----------------------------------------
  bool isInReplay()
  {
    return *(BW::BWDATA_InReplay) != 0;
  }
  //----------------------------------- IN MATCH ------------------------------------------
  bool isInGame()
  {
    return *(BW::BWDATA_InGame) != 0;
  }
  //----------------------------------- MULTIPLAYER ---------------------------------------
  bool isMultiplayer()
  {
    return (*BW::BWDATA_IsMultiplayer != 0);
  }
  //----------------------------------- SINGLEPLAYER --------------------------------------
  bool isSingleplayer()
  {
    return (*BW::BWDATA_IsMultiplayer == 0);
  }
  //----------------------------------- IS IN LOBBY ---------------------------------------
  bool isInLobby()
  {
    return *BW::BWDATA_NextMenu==3;
  }
  //----------------------------------- IS PAUSED -----------------------------------------
  bool isPaused()
  {
    return *BW::BWDATA_IsNotPaused == 0;
  }
  //----------------------------------- GET MOUSE X ---------------------------------------
  int getMouseX()
  {
    // Retrieves the mouse's X coordinate
    return *(BW::BWDATA_MouseX);
  }
  //----------------------------------- GET MOUSE Y ---------------------------------------
  int getMouseY()
  {
    // Retrieves the mouse's Y coordinate
    return *(BW::BWDATA_MouseY);
  }
  //----------------------------------- GET SCREEN X --------------------------------------
  int getScreenX()
  {
    // Retrieves the screen's X coordinate in relation to the map
    return *(BW::BWDATA_ScreenX);
  }
  //----------------------------------- GET SCREEN Y --------------------------------------
  int getScreenY()
  {
    // Retrieves the screen's Y coordinate in relation to the map
    return *(BW::BWDATA_ScreenY);
  }
  //-------------------------------- SET SCREEN POSITION ----------------------------------
  void setScreenPosition(int x, int y)
  {
    // Sets the screen's position in relation to the map
    *(BW::BWDATA_ScreenX) = x;
    *(BW::BWDATA_ScreenY) = y;
  }
  //------------------------------------ START GAME ---------------------------------------
  void startGame()
  {
    // Starts the game as a lobby host
    issueCommand(BW::Command::StartGame());
  }
  //------------------------------------ PAUSE GAME ---------------------------------------
  void pauseGame()
  {
    // Pauses the game
    issueCommand(BW::Command::PauseGame());
  }
  //------------------------------------ RESUME GAME --------------------------------------
  void resumeGame()
  {
    // Resumes the game
    issueCommand(BW::Command::ResumeGame());
  }
  //------------------------------------- LEAVE GAME --------------------------------------
  void leaveGame()
  {
    // Leaves the current game. Moves directly to the post-game score screen
    *BW::BWDATA_GameState = 0;
    *BW::BWDATA_GamePosition = 6;
  }
  //------------------------------------- RESTART GAME ------------------------------------
  void restartGame()
  {
    // Restarts the current match 
    // Does not work on Battle.net
    *BW::BWDATA_GameState = 0;
    *BW::BWDATA_GamePosition = 5;
  }
  //------------------------------------- GET LATENCY -------------------------------------
  int getLatency()
  {
    // Returns the real latency values

    // special case
    if (BW::isSingleplayer())
      return BW::Latencies::Singleplayer;

    /* Lame options checking */
    switch(*BW::BWDATA_Latency)
    {
      case 0:
        return BW::Latencies::LanLow;
      case 1:
        return BW::Latencies::LanMedium;
      case 2:
        return BW::Latencies::LanHigh;
      default:
        return BW::Latencies::LanLow;
    }
  }
  //------------------------------------ CHANGE SLOT ---------------------------------------
  void changeSlot(u8 slot, u8 slotState)
  {
    issueCommand(BW::Command::ChangeSlot(slot, slotState));
  }
  //--------------------------------- PRINT WITH PLAYER ID ---------------------------------
  void printEx(int pID, const char* text, ...)
  {
    // TODO: capsulate formatting to one function
    Util::StaticArray<char, 2048> buffer;
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer.data, buffer.size, buffer.size, text, ap);
    va_end(ap);

    char* txtout = buffer.data;
    if (BW::isInGame() || BW::isInReplay())
    {
      __asm
      {
        pushad
        push 0       // Unknown
        mov eax, pID   // Player ID (-1 for notification area)
        push txtout  // Text
        call dword ptr [BW::BWFXN_PrintText]
        popad
      }
    }
    else
      printf(txtout); // until lobby print private text is found
  }
  //-------------------------------------- PRINTF ------------------------------------------
  void printf(const char* text, ...)
  {
    // TODO: capsulate formatting to one function
    Util::StaticArray<char, 2048> buffer;
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer.data, buffer.size, buffer.size, text, ap);
    va_end(ap);

    if (BW::isInReplay() || BW::isInGame())
    {
      printEx(8, buffer.data);
      return;
    }

    char* txtout = buffer.data;
    if (!BW::isInGame() && BW::isInLobby())
      __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
      }
  }
  void sendText(const char* text, ...)
  {
    // TODO: capsulate formatting to one function
    Util::StaticArray<char, 2048> buffer;
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer.data, buffer.size, buffer.size, text, ap);
    va_end(ap);
    char* txtout = buffer.data;

    if (BW::isInReplay())
    {
      printEx(8, buffer.data);
      return;
    }

    if (BW::isInGame() && BW::isSingleplayer())
    {
      static DWORD activeCheatFlags = 0;
      // TODO: separate cheat enabling and text printing
      BW::CheatFlag cheatID = BW::getCheatFlag(text);
      if (cheatID!=BW::CheatFlags::None)
      {
        activeCheatFlags ^= cheatID;
        issueCommand(BW::Command::UseCheat(activeCheatFlags));
        if (cheatID==BW::CheatFlags::ShowMeTheMoney ||
            cheatID==BW::CheatFlags::BreateDeep ||
            cheatID==BW::CheatFlags::WhatsMineIsMine)
          activeCheatFlags ^= cheatID;
      }
      else
      {
        // TODO: fix
        //printEx(self player id, buffer);
      }
      return;
    }

    if (BW::isInGame())
    {
      memset(BW::BWDATA_SendTextRequired, 0xFF, 2);
      __asm
      {
        pushad
        mov esi, txtout
        call [BW::BWFXN_SendPublicCallTarget]
        popad
      }

    }
    else
      __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
      }
  }
}
