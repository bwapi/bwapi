#include "LocalPC.h"
#include "Connect.h"
#include "Threads.h"

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

HANDLE ghRecvEvent;

bool __stdcall _spiDestroy()
{
  /* Called when unloading the module
     do any cleanup here */
  gbWantExit = true;
  DestroySockets();
  return true;
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
    *dwResult = gdwSendCalls;
    return true;
  case 13:    // Total number of calls made to recvfrom
    *dwResult = gdwRecvCalls;
    return true;
  case 14:    // Total number of bytes sent using sendto
    *dwResult = gdwSendBytes;
    return true;
  case 15:    // Total number of bytes received using recvfrom
    *dwResult = gdwRecvBytes;
    return true;
  default:
    return false;
  }
}

bool __stdcall _spiInitializeProvider(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent)
{
  /* Called when the module is loaded
     Perform all initialization functions here */

  // Reset performance data
  gdwSendCalls = 0;
  gdwSendBytes = 0;
  gdwRecvCalls = 0;
  gdwRecvBytes = 0;

  // Retrieve Starcraft path
  if ( SRegLoadString("Starcraft", "InstallPath", SREG_LOCAL_MACHINE, gszInstallPath, MAX_PATH) )
    SStrNCat(gszInstallPath, "\\", MAX_PATH);

  // Retrieve config path
  SStrCopy(gszConfigPath, gszInstallPath, MAX_PATH);
  SStrNCat(gszConfigPath, "bwapi-data\\bwapi.ini", MAX_PATH);

  // Retrieve log path
  GetPrivateProfileString("paths", "log_path", "bwapi-data\\logs", gszLogPath, MAX_PATH, gszConfigPath);
  SStrNCat(gszLogPath, "\\SNPModule.log", MAX_PATH);

  gbWantExit = false;

  // Save event and Initialize Sockets
  ghRecvEvent = hEvent;
  if ( !InitializeSockets() )
    return false;

  SOCKADDR addr;
  InitAddr(&addr, "127.255.255.255", 6111);
  int rval = sendto(gsSendSock, "test", 5, 0, &addr, sizeof(SOCKADDR));
  if ( rval == SOCKET_ERROR || rval == 0 )
    Error(WSAGetLastError(), "sendto fail");

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

bool __stdcall _spiReceiveFrom(SOCKADDR **addr, char **data, DWORD *databytes)
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

bool __stdcall _spiSendTo(DWORD addrCount, sockaddr **addrList, char *buf, DWORD bufLen)
{
  // pretty sure this is now complete
  if ( !addrCount || !addrList || !buf || !bufLen || !gsGame )
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  for ( int i = addrCount; i > 0; --i )
  {
    sendto(gsGame, buf, bufLen, 0, addrList[i-1], sizeof(SOCKADDR));
    ++gdwSendCalls;
    gdwSendBytes += bufLen;
  }
  return true;
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
