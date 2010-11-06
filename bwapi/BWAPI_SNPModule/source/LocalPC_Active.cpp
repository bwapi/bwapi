#include <stdio.h>
#include <deque>

#include "LocalPC.h"
#include "Connect.h"
#include "Threads.h"
#include "Common.h"
#include "Commands.h"

DWORD gdwProcId;

DWORD gdwProduct;
DWORD gdwVerbyte;
DWORD gdwMaxPlayers;
DWORD gdwLangId;

volatile gameStruc *gpMGameList;

CRITICAL_SECTION gCrit;
HANDLE ghRecvEvent;

bool __stdcall _spiDestroy()
{
  /* Called when unloading the module
     do any cleanup here */
  _spiStopAdvertisingGame();
  gbWantExit = true;

  EnterCriticalSection(&gCrit);
  // Free the game list
  volatile gameStruc *g = gpMGameList;
  while ( g )
  {
    volatile gameStruc *toFree = g;
    g = g->pNext;
    SMemFree((void*)toFree, __FILE__, __LINE__, 0);
  }

  // Free the receive queue
  volatile pktq *r = gpRecvQueue;
  while ( r )
  {
    volatile pktq *toFree = r;
    r = r->pNext;
    SMemFree((void*)toFree, __FILE__, __LINE__, 0);
  }
  LeaveCriticalSection(&gCrit);
  DestroySockets();
  return true;
}

bool __stdcall _spiGetGameInfo(DWORD dwFindIndex, char *pszFindGameName, int a3, gameStruc *pGameResult)
{
  // Finds the game struct that matches a name or index and returns it in pGameResult
  if ( pGameResult )
    memset(pGameResult, 0, sizeof(gameStruc));
  if ( pszFindGameName && pGameResult && (dwFindIndex || *pszFindGameName) )
  {
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
  }
  else
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }
  SetLastError(STORM_ERROR_GAME_NOT_FOUND);
  return false;
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

  gdwProcId = GetCurrentProcessId();

  // Retrieve Starcraft path
  if ( SRegLoadString("Starcraft", "InstallPath", SREG_LOCAL_MACHINE, gszInstallPath, MAX_PATH) )
    SStrNCat(gszInstallPath, "\\", MAX_PATH);

  // Retrieve config path
  SStrCopy(gszConfigPath, gszInstallPath, MAX_PATH);
  SStrNCat(gszConfigPath, "bwapi-data\\bwapi.ini", MAX_PATH);

  // Retrieve log path
  GetPrivateProfileString("paths", "log_path", "bwapi-data\\logs", gszLogPath, MAX_PATH, gszConfigPath);
  SStrNCat(gszLogPath, "\\SNPModule_", MAX_PATH);
  
  char tBuf[16];
  SStrNCat(gszLogPath, itoa(gdwProcId, tBuf, 10), MAX_PATH);
  SStrNCat(gszLogPath, ".log", MAX_PATH);

  // set exit flag to false
  gbWantExit  = false;

  // Save event and Initialize Sockets
  ghRecvEvent = hEvent;
  if ( !InitializeSockets() )
    return false;

  return true;
}

bool __stdcall _spiLockGameList(int a1, int a2, gameStruc **ppGameList)
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

bool __stdcall _spiReceiveFrom(SOCKADDR **addr, char **data, DWORD *databytes)
{
  /* Passes pointers from queued receive data to storm */
  if ( !addr || !data || !databytes || !gsSend )
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

  *addr       = (SOCKADDR*)&gpRecvQueue->saFrom;
  *data       = (char*)gpRecvQueue->bData;
  *databytes  = gpRecvQueue->dwLength;
  gpRecvQueue = gpRecvQueue->pNext;
  LeaveCriticalSection(&gCrit);
  LogBytes(*data, *databytes, "RECEIVE %s->%s", inet_ntoa( *(in_addr*)&(*addr)->sa_data[2]), gszThisIP );
  return true;
}

bool __stdcall _spiSendTo(DWORD addrCount, sockaddr **addrList, char *buf, DWORD bufLen)
{
  /* Sends data to all listed addresses specified by storm */
  if ( !addrCount || !addrList || !buf || !bufLen || bufLen > LOCL_PKT_SIZE || !gsSend)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  char buffer[LOCL_PKT_SIZE + sizeof(packet)];
  SMemZero(buffer, LOCL_PKT_SIZE + sizeof(packet));

  packet *pktHead = (packet*)buffer;
  char   *pktData = buffer + sizeof(packet);

  pktHead->wType = CMD_STORM;
  pktHead->wSize = (WORD)(bufLen + sizeof(packet));
  memcpy(pktData, buf, bufLen);

  for ( int i = addrCount; i > 0; --i )
  {
    sendto(gsSend, buffer, pktHead->wSize, 0, addrList[i-1], sizeof(SOCKADDR));
    LogBytes(buf, bufLen, "SEND %s->%s", gszThisIP, inet_ntoa(*(in_addr*)&addrList[i-1]->sa_data[2]) );
    ++gdwSendCalls;
    gdwSendBytes += bufLen;
  }
  return true;
}

bool __stdcall _spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pPlayerInfo, DWORD dwPlayerCount)
{
  /* Begin game advertisement
     Called when you create a game */
  if ( !pszGameName || !pszGameStatString )
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  EnterCriticalSection(&gCrit);
  if ( !gpGameAdvert )
  {
    gpGameAdvert = SMemAlloc(LOCL_PKT_SIZE + sizeof(packet), __FILE__, __LINE__, 0);
    if ( !gpGameAdvert )
    {
      LeaveCriticalSection(&gCrit);
      SetLastError(ERROR_NOT_ENOUGH_MEMORY);
      return false;
    }
  }
  memset((void*)gpGameAdvert, 0, LOCL_PKT_SIZE + sizeof(packet));
  packet *pktHd   = (packet*) gpGameAdvert;
  char   *pktData = (char*)   gpGameAdvert + sizeof(packet);

  // +2 is for the two null terminators
  pktHd->wSize       = (WORD)(strlen(pszGameName) + strlen(pszGameStatString) + dwPlayerCount + sizeof(packet) + 2);
  pktHd->wType       = CMD_ADDGAME;
  pktHd->dwGameState = dwGameState;
  
  SStrCopy(pktData, pszGameName, 128);
  SStrCopy(&pktData[strlen(pktData)+1], pszGameStatString, 128);

  LeaveCriticalSection(&gCrit);
  BroadcastAdvertisement();
  return true;
}

bool __stdcall _spiStopAdvertisingGame()
{
  /* Stops game advertisement
     Called when you leave a game */
  EnterCriticalSection(&gCrit);
  if ( gpGameAdvert )
  {
    ((packet*)gpGameAdvert)->wType = CMD_REMOVEGAME;
    WORD wPktSize = ((packet*)gpGameAdvert)->wSize;
    sendto(gsBroadcast, (char*)gpGameAdvert, wPktSize, 0, &gaddrBroadcast, sizeof(SOCKADDR));
    ++gdwSendCalls;
    gdwSendBytes += wPktSize;
    SMemFree((void*)gpGameAdvert, __FILE__, __LINE__, 0);
    gpGameAdvert = NULL;
  }
  LeaveCriticalSection(&gCrit);
  return true;
}

DWORD gdwLastTickCount;
bool __stdcall _spiUnlockGameList(gameStruc *pGameList, DWORD *a2)
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
  if ( dwThisTickCount - gdwLastTickCount > 400 )
  {
    gdwLastTickCount = dwThisTickCount;
    BroadcastGameListRequest();
  }
  return true;
}
