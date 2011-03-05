#include "LocalPC.h"
#include "Connect.h"
#include "Threads.h"

namespace COMN
{
  bool __stdcall spiCompareNetAddresses(SOCKADDR_IN *addr1, SOCKADDR_IN *addr2, DWORD *dwResult)
  {
    // This function is complete
    if ( dwResult )
      *dwResult = 0;
    if ( !addr1 || !addr2 || !dwResult )
    {
      SErrSetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    if ( addr1->sin_addr.S_un.S_un_b.s_b1 != addr2->sin_addr.S_un.S_un_b.s_b1 )
      *dwResult = 5;
    else if ( addr1->sin_addr.S_un.S_un_b.s_b2 != addr2->sin_addr.S_un.S_un_b.s_b2 )
      *dwResult = 4;
    else if ( addr1->sin_addr.S_un.S_un_b.s_b3 != addr2->sin_addr.S_un.S_un_b.s_b3 )
      *dwResult = 3;
    else if ( addr1->sin_addr.S_un.S_un_b.s_b4 != addr2->sin_addr.S_un.S_un_b.s_b4 )
      *dwResult = 2;
    else if ( addr1->sin_port != addr2->sin_port )
      *dwResult = 1;
    else
      *dwResult = 0;
    return true;
  }

  bool __stdcall spiDestroy()
  {
    /* Called when unloading the module
       do any cleanup here */
    gbWantExit = true;

    // Free the game list
    volatile gameStruc *g = gpMGameList;
    while ( g )
    {
      volatile gameStruc *toFree = g;
      g = g->pNext;
      SMFree((void*)toFree);
    }
    return true;
  }

  bool __stdcall spiFree(SOCKADDR_IN *addr, char *data, DWORD databytes)
  {
    // This function is complete
    if ( !addr || !data )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }
    SMFree(addr);
    return true;
  }

  bool __stdcall spiFreeExternalMessage(SOCKADDR_IN *addr, char *data, DWORD databytes)
  {
    // This function is complete
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  bool __stdcall spiGetGameInfo(DWORD dwFindIndex, char *pszFindGameName, int a3, gameStruc *pGameResult)
  {
    // Finds the game struct that matches a name or index and returns it in pGameResult
    if ( pGameResult )
      memset(pGameResult, 0, sizeof(gameStruc));
    if ( !pszFindGameName || !pGameResult || (!dwFindIndex && !(*pszFindGameName)) )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    EnterCriticalSection(&gCrit);
    volatile gameStruc *g = gpMGameList;
    while ( g && 
            (dwFindIndex && dwFindIndex != g->dwIndex || 
             *pszFindGameName && _strcmpi(pszFindGameName, (char*)g->szGameName)) )
      g = g->pNext;
    
    if ( g )
      memcpy(pGameResult, (void*)g, sizeof(gameStruc));

    LeaveCriticalSection(&gCrit);
    if ( pGameResult->dwIndex )
      return true;

    SetLastError(STORM_ERROR_GAME_NOT_FOUND);
    return false;
  }

  bool __stdcall spiGetPerformanceData(DWORD dwType, DWORD *dwResult, int a3, int a4)
  {
    // Returns performance data in dwResult, we are not using this
    *dwResult = 0;
    return true;
  }

  bool __stdcall spiInitialize(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent)
  {
    /* Called when the module is loaded
       Perform all initialization functions here */

    // Save client information
    gdwProduct    = gameClientInfo->dwProduct;
    gdwVerbyte    = gameClientInfo->dwVerbyte;
    gdwMaxPlayers = gameClientInfo->dwMaxPlayers;
    gdwLangId     = gameClientInfo->dwLangId;

    // set exit flag to false
    gbWantExit  = false;

    // Save event
    ghRecvEvent = hEvent;
    return true;
  }

  bool __stdcall spiInitializeDevice(int a1, void *a2, void *a3, DWORD *a4, void *a5)
  {
    // This function is complete
    return false;
  }

  bool __stdcall spiLockDeviceList(DWORD *a1)
  {
    // This function is complete
    *a1 = 0;
    return true;
  }

  bool __stdcall spiReceiveExternalMessage(SOCKADDR_IN **addr, char **data, DWORD *databytes)
  {
    // This function is complete
    if ( addr )
      *addr = NULL;
    if ( data )
      *data = NULL;
    if ( databytes )
      *databytes = 0;
    SetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
    return false;
  }

  bool __stdcall spiSelectGame(int a1, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, int a6)
  {
    /* Looks like an old function and doesn't seem like it's used anymore
       UDPN's function Creates an IPX game select dialog window  */
    return false;
  }

  bool __stdcall spiSendExternalMessage(int a1, int a2, int a3, int a4, int a5)
  {
    // This function is complete
    return false;
  }

  bool __stdcall spiUnlockDeviceList()
  {
    // This function is complete
    return true;
  }

  bool __stdcall spiLeagueGetName(char *pszDest, DWORD dwSize)
  {
    // This function is complete
    return true;
  }
};
