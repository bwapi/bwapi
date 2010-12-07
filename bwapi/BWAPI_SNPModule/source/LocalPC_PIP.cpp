#include <stdio.h>

#include "LocalPC.h"
#include "Threads.h"
#include "Common.h"
#include "Commands.h"

#include "../../LPIP_Server/SharedMemory.h"

namespace LPIP
{
  bool __stdcall spiDestroy()
  {
    /* Called when unloading the module
       do any cleanup here */
    spiStopAdvertisingGame();
    gbWantExit = true;

    EnterCriticalSection(&gCrit);
    // Free the game list
    volatile gameStruc *g = gpMGameList;
    while ( g )
    {
      volatile gameStruc *toFree = g;
      g = g->pNext;
      SMFree((void*)toFree);
    }

    // Free the receive queue
    volatile pktq *r = gpRecvQueue;
    while ( r )
    {
      volatile pktq *toFree = r;
      r = r->pNext;
      SMFree((void*)toFree);
    }
    LeaveCriticalSection(&gCrit);

    // @TODO: Destroy any allocations/stuff here
    if ( s )
    {
      delete s;
      s = NULL;
    }
    Sleep(50);  // 10 is more than enough here
    return true;
  }

  bool __stdcall spiInitializeProvider(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent)
  {
    /* Called when the module is loaded
       Perform all initialization functions here */

    // Save client information
    gdwProduct    = gameClientInfo->dwProduct;
    gdwVerbyte    = gameClientInfo->dwVerbyte;
    gdwMaxPlayers = gameClientInfo->dwMaxPlayers;
    gdwLangId     = gameClientInfo->dwLangId;

    // Reset performance data
    gdwSendCalls = 0;
    gdwSendBytes = 0;
    gdwRecvCalls = 0;
    gdwRecvBytes = 0;

    // set exit flag to false
    gbWantExit  = false;

    // Save event
    ghRecvEvent = hEvent;
    
    // @TODO: Initialize any data/allocations/stuff here
    s = new SharedMemory();
    if ( !s->connect() )
    {
      Error(GetLastError(), "Unable to connect to pipe or shared memory!");
      return false;
    }

    hRecvThread = CreateThread(NULL, 0, &RecvThread, NULL, 0, NULL);
    if ( !hRecvThread )
    {
      Error(ERROR_INVALID_HANDLE, "Unable to create the recv thread.");
      return false;
    }
    SetThreadPriority(hRecvThread, 1);

    return true;
  }

  bool __stdcall spiLockGameList(int a1, int a2, gameStruc **ppGameList)
  {
    /* Lock the game list for management and passing the updates to storm
       Clears games after a certain time */
    if ( !ppGameList )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }
    CleanGameList(10000);
    EnterCriticalSection(&gCrit);
    *ppGameList = (gameStruc*)gpMGameList;
    return true;
  }

  bool __stdcall spiReceiveFrom(SOCKADDR_IN **addr, char **data, DWORD *databytes)
  {
    /* Passes pointers from queued receive data to storm */
    if ( !addr || !data || !databytes )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }
    
    *addr      = NULL;
    *data      = NULL;
    *databytes = 0;

    EnterCriticalSection(&gCrit);
    if ( !gpRecvQueue )
    {
      LeaveCriticalSection(&gCrit);
      SetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
      return false;
    }

    memset((void*)&gpRecvQueue->saFrom, 0, sizeof(SOCKADDR));
    *(WORD*)&gpRecvQueue->saFrom.sa_data[0]  = 1337;
    *(DWORD*)&gpRecvQueue->saFrom.sa_data[2] = gpRecvQueue->dwProcID;
    gpRecvQueue->saFrom.sa_family = AF_INET;

    *addr       = (SOCKADDR_IN*)&gpRecvQueue->saFrom;
    *data       = (char*)gpRecvQueue->bData;
    *databytes  = gpRecvQueue->dwLength;
    gpRecvQueue = gpRecvQueue->pNext;
    LeaveCriticalSection(&gCrit);
    return true;
  }

  bool __stdcall spiSendTo(DWORD addrCount, SOCKADDR_IN **addrList, char *buf, DWORD bufLen)
  {
    /* Sends data to all listed addresses specified by storm */
    if ( !addrCount || !addrList || !buf || !bufLen || bufLen > PKT_SIZE )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    for ( int i = addrCount; i > 0; --i )
    {
      DWORD dwProcSendTo = *(DWORD*)&((SOCKADDR*)addrList[i-1])->sa_data[2];
      
      // @TODO: send stuff here using dwProcSendTo, buf, and bufLen
      s->sendData(buf, bufLen, dwProcSendTo);

      ++gdwSendCalls;
      gdwSendBytes += bufLen;
    }
    return true;
  }

  bool __stdcall spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pPlayerInfo, DWORD dwPlayerCount)
  {
    /* Begin game advertisement
       Called when you create a game */
    if ( !pszGameName || !pszGameStatString )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }
    EnterCriticalSection(&gCrit);

    // @TODO: Begin or update the game advertisement
    // using the following:
    //    pszGameName
    //    pszGameStatString
    //    dwGameState

    GameInfo myGameInfo;
    memset(&myGameInfo, 0, sizeof(GameInfo));
    strncpy(myGameInfo.chGameName, pszGameName, 128);
    strncpy(myGameInfo.chGameStats, pszGameStatString, 128);
    myGameInfo.dwGameState = dwGameState;

    s->advertiseLadderGame(&myGameInfo);

    LeaveCriticalSection(&gCrit);
    return true;
  }

  bool __stdcall spiStopAdvertisingGame()
  {
    /* Stops game advertisement
       Called when you leave a game */
    EnterCriticalSection(&gCrit);

    // @TODO: Remove the game advertisement
    s->removeLadderGameAd();

    LeaveCriticalSection(&gCrit);
    return true;
  }

  DWORD gdwLastTickCount;
  bool __stdcall spiUnlockGameList(gameStruc *pGameList, DWORD *a2)
  {
    /* Unlocks the game list and makes requests to update the list internally */
    if ( pGameList != gpMGameList )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    LeaveCriticalSection(&gCrit);
    if ( a2 )
      *a2 = 500;

    DWORD dwThisTickCount = GetTickCount();
    if ( dwThisTickCount - gdwLastTickCount > 100 )
    {
      gdwLastTickCount = dwThisTickCount;
      // @TODO: Update the game list here (local SNP/Storm module listing, just call below function with correct params)
      //UpdateGameList(procID from, game state, game name, stat string, true = remove it from the list);
      s->updateGameList();
      for each( GameInfo *g in s->games )
      {
        UpdateGameList(g->serverProcessID, g->dwGameState, g->chGameName, g->chGameStats, g->lastUpdate == 0 );
      }
    }
    return true;
  }
};
