#include <windows.h>
#include <stdio.h>
#include "../../Storm/storm.h"

#include "Template.h"
#include "Common.h"

#define LUDP_ID 0
namespace LUDP
{
  bool __stdcall spiDestroy();
  bool __stdcall spiInitialize(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent);
  bool __stdcall spiLockGameList(int a1, int a2, gameStruc **ppGameList);
  bool __stdcall spiReceive(SOCKADDR_IN **addr, char **data, DWORD *databytes);
  bool __stdcall spiSend(DWORD addrCount, SOCKADDR_IN **addrList, char *buf, DWORD bufLen);
  bool __stdcall spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pPlayerInfo, DWORD dwPlayerCount); // dwElapsedTime in seconds
  bool __stdcall spiStopAdvertisingGame();
  bool __stdcall spiUnlockGameList(gameStruc *pGameList, DWORD *a2);
};

#define LPIP_ID 1
namespace LPIP
{
  bool __stdcall spiDestroy();
  bool __stdcall spiInitialize(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent);
  bool __stdcall spiLockGameList(int a1, int a2, gameStruc **ppGameList);
  bool __stdcall spiReceive(SOCKADDR_IN **addr, char **data, DWORD *databytes);
  bool __stdcall spiSend(DWORD addrCount, SOCKADDR_IN **addrList, char *buf, DWORD bufLen);
  bool __stdcall spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pPlayerInfo, DWORD dwPlayerCount); // dwElapsedTime in seconds
  bool __stdcall spiStopAdvertisingGame();
  bool __stdcall spiUnlockGameList(gameStruc *pGameList, DWORD *a2);
};

namespace COMN
{
  bool __stdcall spiCompareNetAddresses(SOCKADDR_IN *addr1, SOCKADDR_IN *addr2, DWORD *dwResult);
  bool __stdcall spiFree(SOCKADDR_IN *addr, char *data, DWORD databytes);
  bool __stdcall spiFreeExternalMessage(int a1, int a2, int a3);
  bool __stdcall spiGetGameInfo(DWORD dwFindIndex, char *pszFindGameName, int a3, gameStruc *pGameResult);
  bool __stdcall spiGetPerformanceData(DWORD dwType, DWORD *dwResult, int a3, int a4);
  bool __stdcall spiInitializeDevice(int a1, void *a2, void *a3, DWORD *a4, void *a5);
  bool __stdcall spiLockDeviceList(DWORD *a1);
  bool __stdcall spiReceiveExternalMessage(SOCKADDR_IN **addr, char **data, DWORD *databytes);
  bool __stdcall spiSelectGame(int a1, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, int a6);
  bool __stdcall spiSendExternalMessage(int a1, int a2, int a3, int a4, int a5);
  bool __stdcall spiUnlockDeviceList();
  bool __stdcall spiLeagueGetName(char *pszDest, DWORD dwSize);
};
