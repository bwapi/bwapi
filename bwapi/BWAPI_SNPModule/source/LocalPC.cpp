#include "LocalPC.h"
#include "Connect.h"

/* @TODO LIST:
[Initialization]
  _spiInitializeProvider
  _spiDestroy

[Game List]     // Note: Havn't been able to record usage of other functions with no partner game
  _spiLockGameList
  _spiUnlockGameList

[Packets]
  _spiReceiveFrom
  _spiSendTo

*/

char buffer[1024];

bool __stdcall fxn0(int a1, int a2, int a3)
{
  // not important right now
  i(__FUNCTION__);
  return true;
}

bool __stdcall _spiDestroy()
{
  /* Called when unloading the module
     do any cleanup here */
  return true;
}

bool __stdcall _spiFree(void *a1, int a2, int a3)
{
  // This function is complete
  if ( a1 && a2 )
  {
    SMemFree(a1, __FILE__, __LINE__, 0);
    return true;
  }
  else
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }
}

bool __stdcall _spiError(int a1, int a2, int a3)
{
  // This function is complete
  SetLastError(ERROR_INVALID_PARAMETER);
  return false;
}

bool __stdcall _spiGetGameInfo(int a1, int a2, int a3, int a4)
{
  // Unknown, to do with unknown struct
  i(__FUNCTION__);
  return true;
}

bool __stdcall _spiGetPerformanceData(DWORD dwType, DWORD *dwResult, int a3, int a4)
{
  // Returns performance data in dwResult
  switch ( dwType )
  {
  case 12:    // Total number of calls made to sendto
    *dwResult = 0;
    return true;
  case 13:    // Total number of calls made to recvfrom
    *dwResult = 0;
    return true;
  case 14:    // Total number of bytes sent using sendto
    *dwResult = 0;
    return true;
  case 15:    // Total number of bytes received using recvfrom
    *dwResult = 0;
    return true;
  default:
    return false;
  }
}

bool __stdcall _spiInitializeProvider(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent)
{
  /* Called when the module is loaded
     Perform all initialization functions here */

  return true;
}

bool __stdcall _spiInitializeDevice(int a1, PSTRUCT a2, PSTRUCT a3, DWORD *a4, void *a5)
{
  // This function is complete
  return false;
}

bool __stdcall fxn8(DWORD *a1)
{
  // This function is complete
  *a1 = 0;
  return true;
}

// spiLockGameList(0x%08x,0x%08x,*gamelist)
bool __stdcall _spiLockGameList(int a1, int a2, void **a3)
{
  /*sprintf(buffer, "_spiLockGameList(%p, %p, %p)", a1, a2, *a3);
  i(buffer);*/
  if ( a3 )
  {
    /* Lock the game list (thread) 
       UDPN locks this thread for 10000 ms 
       unknown struct involved
       @todo: Research the struct */
    return true;
  }
  else
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }
}

bool __stdcall _spiReceiveFrom(SOCKADDR **addr, BYTE **data, DWORD *databytes)
{
  if ( addr )
    *addr = NULL;
  if ( data )
    *data = NULL;
  if ( databytes )
    *databytes = 0;

  
  /* Return data obtained from recvfrom thread */

  return true;
}

bool __stdcall _spiReceive(SOCKADDR **addr, BYTE **data, DWORD *databytes)
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

bool __stdcall _spiSelectGame(int a1, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, int a6)
{
  /* Looks like an old function and doesn't seem like it's used anymore
     UDPN's function Creates an IPX game select dialog window  */
  i(__FUNCTION__);
  return false;
}

bool __stdcall _spiSendTo(DWORD addrCount, sockaddr **addrList, void *buf, DWORD bufLen)
{
  // @todo: samples & research
  i(__FUNCTION__);
  return true;
}

bool __stdcall _spiSend(int a1, int a2, int a3, int a4, int a5)
{
  // This function is complete
  return false;
}

bool __stdcall _spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, int a9, DWORD dwMaxStormPlayers)
{
  /* Begin game advertisement
     Needs a little more research
     Called when you create a game */
  return true;
}

bool __stdcall _spiStopAdvertisingGame()
{
  /* Stops game advertisement
     Called when you leave a game */
  return true;
}

bool __stdcall _spiInitialize()
{
  // This function is complete
  return true;
}

// spiUnlockGameList(0x%08x,*hintnextcall)
bool __stdcall _spiUnlockGameList(int a1, DWORD *a2)
{
  /*
  sprintf(buffer, "_spiUnlockGameList(%p, %p)", a1, a2);
  i(buffer);
  */

  /* Unlocks the game list and does something?
     @Todo: Research needed */

  if ( a2 )
  {
    *a2 = 500;
  }
  return true;
}
