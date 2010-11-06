#include <windows.h>
#include <stdio.h>
#include "../../Storm/storm.h"

#include "Template.h"
#include "Common.h"

extern DWORD gdwProcId;

extern DWORD gdwProduct;
extern DWORD gdwVerbyte;
extern DWORD gdwMaxPlayers;
extern DWORD gdwLangId;

extern gameStruc *gpMGameList;

extern HANDLE ghRecvEvent;
extern CRITICAL_SECTION gCrit;

#define LOCL 0
#define LOCL_PKT_SIZE 512

bool __stdcall fxn0(int a1, int a2, int a3);
bool __stdcall _spiDestroy();
bool __stdcall _spiFree(void *a1, int a2, int a3);
bool __stdcall _spiError(int a1, int a2, int a3);
bool __stdcall _spiGetGameInfo(DWORD dwFindIndex, char *pszFindGameName, int a3, gameStruc *pGameResult);
bool __stdcall _spiGetPerformanceData(DWORD dwType, DWORD *dwResult, int a3, int a4);
bool __stdcall _spiInitializeProvider(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent);
bool __stdcall _spiInitializeDevice(int a1, void *a2, void *a3, DWORD *a4, void *a5);
bool __stdcall _spiEnumDevices(DWORD *a1);
bool __stdcall _spiLockGameList(int a1, int a2, gameStruc **ppGameList);
bool __stdcall _spiReceiveFrom(SOCKADDR **addr, char **data, DWORD *databytes);
bool __stdcall _spiReceive(SOCKADDR **addr, char **data, DWORD *databytes);
bool __stdcall _spiSelectGame(int a1, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, int a6);
bool __stdcall _spiSendTo(DWORD addrCount, sockaddr **addrList, char *buf, DWORD bufLen);
bool __stdcall _spiSend(int a1, int a2, int a3, int a4, int a5);
bool __stdcall _spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pPlayerInfo, DWORD dwPlayerCount); // dwElapsedTime in seconds
bool __stdcall _spiStopAdvertisingGame();
bool __stdcall _spiInitialize();
bool __stdcall _spiUnlockGameList(gameStruc *pGameList, DWORD *a2);


