#pragma once
#include <windows.h>
#include "../../Storm/storm.h"

#ifndef TEMPLATE_STRUCTS
#define TEMPLATE_STRUCTS

struct netFunctions
{
  DWORD dwSize;
  void  *fxn_0;
  bool  (__stdcall *spiDestroy)();  // official
  void  *spiFree;                   // official spiFree(0x%08x,0x%08x,%u)
  void  *spiError;                  // unofficial
  void  *spiGetGameInfo;           // official spiGetGameInfo(0x%08x,\"%s\",*gameinfo)
  void  *spiGetPerformanceData; // unknown/guess
  bool  (__stdcall *spiInitializeProvider)(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent);         // official spiInitialize(0x%08x) spiInitialize(0x%08x,0x%08x,0x%08x,0x%08x,0x%08x)
  void  *spiInitializeDevice;   // InitializeDevice
  void  *fxn_8;                 // EnumProviders
  void  *spiLockGameList;       // official spiLockGameList(0x%08x,0x%08x,*gamelist) (0x%08x,0x%08x,*gamehead); spiLockGameList(*gamelist)
  bool (__stdcall *spiReceiveFrom)(SOCKADDR **addr, char **data, DWORD *databytes);        // unknown/guess
  bool (__stdcall *spiReceive)(SOCKADDR **addr, char **data, DWORD *databytes);            // official spiReceive(*addr,*data,*databytes) (int *, int *, char **) 
  void  *spiSelectGame;         // official spiSelectGame(0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,*playerid)
  bool (__stdcall *spiSendTo)(DWORD addrCount, sockaddr **addrList, char *buf, DWORD bufLen);
  void  *spiSend;                // unofficial; last param is the message
  void  *spiStartAdvertisingLadderGame;       // unofficial
  bool (__stdcall *spiStopAdvertisingGame)(); // official
  bool (__stdcall *spiInitialize)();         // EnumProviders
  void  *spiUnlockGameList;                   // official spiUnlockGameList(0x%08x,*hintnextcall) 
  bool (__stdcall *spiStartAdvertisingGame)(const char *pszGameName, DWORD dwGameNameSize, const char *pszPassword, DWORD dwPasswordSize);
  void  *spiReportGameResult;         // unknown/guess
  void  *spiCheckDataFile;            // unknown/guess
  void  *spiLeagueCommand;            // unknown/guess
  void  *spiLeagueSendReplayPath;     // unknown/guess
  void  *spiLeagueGetReplayPath;      // unknown/guess
  void  *spiLeagueLogout;             // unknown/guess
  bool (__stdcall *spiLeagueGetName)(char *pszDest, DWORD dwSize); // unknown/guess
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
