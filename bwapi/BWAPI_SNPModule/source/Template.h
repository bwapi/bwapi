#pragma once
#include <windows.h>
#include "../../Storm/storm.h"

#ifndef TEMPLATE_STRUCTS
#define TEMPLATE_STRUCTS

struct netFunctions
{
  // The size of the vtable
  DWORD dwSize;
  // Compares two sockaddrs with each other and returns the number of differences in dwResult
  bool  (__stdcall *spiCompareNetAddresses)(SOCKADDR_IN *addr1, SOCKADDR_IN *addr2, DWORD *dwResult);
  // Called when the module is released
  bool  (__stdcall *spiDestroy)();
  // Called in order to free blocks of packet memory returned in the spiReceive functions
  bool  (__stdcall *spiFree)(SOCKADDR_IN *addr, char *data, DWORD databytes);
  void  *spiFreeExternalMessage;
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
  bool (__stdcall *spiReceive)(SOCKADDR_IN **addr, char **data, DWORD *databytes);
  // Return received data from a connected socket to storm
  bool (__stdcall *spiReceiveExternalMessage)(SOCKADDR_IN **addr, char **data, DWORD *databytes);
  // Called when a game is selected to query information
  void  *spiSelectGame;
  // Sends data over a connectionless socket
  bool (__stdcall *spiSend)(DWORD addrCount, SOCKADDR_IN **addrList, char *buf, DWORD bufLen);
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

struct netModule
{
  char          *pszName;
  DWORD         dwIdentifier;
  char          *pszDescription;
  CAPS          caps;
  netFunctions  NetFxns;
};


#endif
