#pragma once
#include <windows.h>
#include "SNPNetwork.h"

namespace SNP
{
  struct NetFunctions
  {
    // The size of the vtable
    DWORD dwSize;
    // Compares two sockaddrs with each other and returns the number of differences in dwResult
    bool  (__stdcall *spiCompareNetAddresses)(SOCKADDR *addr1, SOCKADDR *addr2, DWORD *dwResult);
    // Called when the module is released
    bool  (__stdcall *spiDestroy)();
    // Called in order to free blocks of packet memory returned in the spiReceive functions
    bool  (__stdcall *spiFree)(SOCKADDR *addr, char *data, DWORD databytes);
    bool  (__stdcall *spiFreeExternalMessage)(SOCKADDR *addr, char *data, DWORD databytes);
    // Returns info on a specified game
    void  *spiGetGameInfo;
    // Returns packet statistics
    void  *spiGetPerformanceData;
    // Called when the module is initialized
    bool  (__stdcall *spiInitialize)(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent);
    void  *spiInitializeDevice;
    void  *spiLockDeviceList;
    // Called to prevent the game list from updating so that it can be processed by storm
    void  *spiLockGameList;
    // Return received data from a connectionless socket to storm
    bool (__stdcall *spiReceive)(SOCKADDR **addr, char **data, DWORD *databytes);
    // Return received data from a connected socket to storm
    bool (__stdcall *spiReceiveExternalMessage)(SOCKADDR **addr, char **data, DWORD *databytes);
    // Called when a game is selected to query information
    void  *spiSelectGame;
    // Sends data over a connectionless socket
    bool (__stdcall *spiSend)(DWORD addrCount, SOCKADDR **addrList, char *buf, DWORD bufLen);
    // Sends data over a connected socket
    void  *spiSendExternalMessage;
    // An extended version of spiStartAdvertisingGame
    void  *spiStartAdvertisingLadderGame;
    // Called to stop advertising the game
    bool (__stdcall *spiStopAdvertisingGame)();
    bool (__stdcall *spiUnlockDeviceList)();
    // Called after the game list has been processed and resume updating
    void  *spiUnlockGameList;
    // Called to begin advertising a created game to other clients
    bool (__stdcall *spiStartAdvertisingGame)(const char *pszGameName, DWORD dwGameNameSize, const char *pszPassword, DWORD dwPasswordSize);
    void  *spiReportGameResult;
    void  *spiCheckDataFile;
    void  *spiLeagueCommand;
    void  *spiLeagueSendReplayPath;
    void  *spiLeagueGetReplayPath;
    void  *spiLeagueLogout;
    bool (__stdcall *spiLeagueGetName)(char *pszDest, DWORD dwSize);
  };

  extern NetFunctions spiFunctions;
  extern Network<SOCKADDR> *pluggedNetwork;

  /*
  bool __stdcall spiInitialize(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, ModuleInfo *moduleData, HANDLE hEvent);
  bool __stdcall spiDestroy();

  bool __stdcall spiLockGameList(int a1, int a2, GameStrucI **ppGameList);
  bool __stdcall spiUnlockGameList(GameStrucI *pGameList, DWORD *a2);

  bool __stdcall spiReceive(PeerID * *addr, char **data, DWORD *databytes);
  bool __stdcall spiFree(PeerID * addr, char *data, DWORD databytes);

  bool __stdcall spiSend(DWORD addrCount, PeerID * *addrList, char *buf, DWORD bufLen);

  bool __stdcall spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pPlayerInfo, DWORD dwPlayerCount); // dwElapsedTime in seconds
  bool __stdcall spiStopAdvertisingGame();
  bool __stdcall spiGetGameInfo(DWORD dwFindIndex, char *pszFindGameName, int a3, GameStrucI *pGameResult);

  // unused
  bool __stdcall spiCompareNetAddresses(PeerID * addr1, PeerID * addr2, DWORD *dwResult);
  bool __stdcall spiFreeExternalMessage(PeerID * addr, char *data, DWORD databytes);
  bool __stdcall spiGetPerformanceData(DWORD dwType, DWORD *dwResult, int a3, int a4);
  bool __stdcall spiInitializeDevice(int a1, void *a2, void *a3, DWORD *a4, void *a5);
  bool __stdcall spiLockDeviceList(DWORD *a1);
  bool __stdcall spiReceiveExternalMessage(PeerID * *addr, char **data, DWORD *databytes);
  bool __stdcall spiSelectGame(int a1, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, ModuleInfo *moduleData, int a6);
  bool __stdcall spiSendExternalMessage(int a1, int a2, int a3, int a4, int a5);
  bool __stdcall spiUnlockDeviceList();
  bool __stdcall spiLeagueGetName(char *pszDest, DWORD dwSize);
  */
};
