#pragma once
#include <windows.h>
#include "../../Storm/storm.h"

#ifndef TEMPLATE_STRUCTS
#define TEMPLATE_STRUCTS

struct netFunctions
{
  DWORD dwSize;
  void  *fxn_0;
  bool  (__stdcall *spiDestroy)();
  void  *spiFree;
  void  *spiError;
  void  *spiGetGameInfo;
  void  *spiGetPerformanceData;
  bool  (__stdcall *spiInitializeProvider)(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent);
  void  *spiInitializeDevice;
  void  *spiEnumDevices;
  void  *spiLockGameList;
  bool (__stdcall *spiReceiveFrom)(SOCKADDR **addr, char **data, DWORD *databytes);
  bool (__stdcall *spiReceive)(SOCKADDR **addr, char **data, DWORD *databytes);
  void  *spiSelectGame;
  bool (__stdcall *spiSendTo)(DWORD addrCount, sockaddr **addrList, char *buf, DWORD bufLen);
  void  *spiSend;
  void  *spiStartAdvertisingLadderGame;
  bool (__stdcall *spiStopAdvertisingGame)();
  bool (__stdcall *spiInitialize)();
  void  *spiUnlockGameList;
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
  caps          Caps;
  netFunctions  NetFxns;
};


#endif
