#include <windows.h>
#include "../../svnrev.h"
#include "../../starcraftver.h"

#include "LocalPC.h"

HINSTANCE ghInstance;

struct caps
{
  DWORD dwSize;
  DWORD dwUnk1;
  DWORD dwBufferSize;
  DWORD dwUnk3;
  DWORD dwUnk4;
  DWORD dwUnk5;
  DWORD dwPingTimeout;
  DWORD dwPlayerCount;
  DWORD dwLatencyCalls;
};
//spiGetGameInfo
struct netFunctions
{
  DWORD dwSize;
  void  *fxn_0;
  bool  (__stdcall *spiDestroy)();  // official
  void  *spiFree;                   // official spiFree(0x%08x,0x%08x,%u)
  void  *spiError;                  // unofficial
  void  *spiGetGameInfo;           // official spiGetGameInfo(0x%08x,\"%s\",*gameinfo)
  void  *spiGetPerformanceData; // unknown/guess
  void  *spiInitialize;         // official spiInitialize(0x%08x) spiInitialize(0x%08x,0x%08x,0x%08x,0x%08x,0x%08x)
  void  *fxn_7;                 // InitializeDevice
  void  *fxn_8;                 // EnumProviders
  void  *spiLockGameList;       // official spiLockGameList(0x%08x,0x%08x,*gamelist) (0x%08x,0x%08x,*gamehead); spiLockGameList(*gamelist)
  void  *fxn_10;
  void  *spiReceive;            // official spiReceive(*addr,*data,*databytes) (int *, int *, char **) 
  void  *spiSelectGame;         // official spiSelectGame(0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,*playerid)
  bool (__stdcall *spiSend)(DWORD addrCount, sockaddr **addrList, void *buf, DWORD bufLen);
  void  *spiSendServerMessage;                // unofficial; last param is the message
  void  *spiStartAdvertisingLadderGame;       // unofficial
  bool (__stdcall *spiStopAdvertisingGame)(); // official
  bool (__stdcall *InitializeUser)();         // EnumProviders
  void  *spiUnlockGameList;                   // official spiUnlockGameList(0x%08x,*hintnextcall) 
  void  *spiStartAdvertisingGame;             // official spiStartAdvertisingGame(0x%08x,0x%08x,"%s","%s") spiStartAdvertisingGame("%s","%s",%u)
  void  *spiReportGameResult;         // unknown/guess
  void  *spiCheckDataFile;            // unknown/guess
  void  *spiLeagueCommand;            // unknown/guess
  void  *fxn_23;
  void  *spiLeagueGetReplayPath;      // unknown/guess
  void  *spiLeagueLogout;             // unknown/guess
  bool (__stdcall *spiLeagueGetName)(char *pszDest, DWORD dwSize); // unknown/guess
};

struct netStruct
{
  char          *pszName;
  DWORD         dwIdentifier;
  char          *pszDescription;
  caps          Caps;
  netFunctions  NetFxns;
} networks[] = {
  { "Local PC", 'LOCL', "BWAPI " STARCRAFT_VER " r" SVN_REV_STR "\n\nConnect multiple instances of Starcraft together on the same PC.",
    { sizeof(caps), 0x20000000, 0x200, 0x10, 0x100, 100000, 20, 8, 0},
    { sizeof(netFunctions),
      &fxn0,                    &_spiDestroy,             &_spiFree,              &_spiError,
      &_spiGetGameInfo,         &_spiGetPerformanceData,  &_spiInitialize,        &fxn7,
      &fxn8,                    &_spiLockGameList,        &fxn10,                 &_spiReceive,
      &_spiSelectGame,          &_spiSend,                &_spiSendServerMessage, &_spiStartAdvertisingLadderGame,
      &_spiStopAdvertisingGame, &InitializeUser,          &_spiUnlockGameList,    NULL,
      NULL,                     NULL,                     NULL,                   NULL,
      NULL,                     NULL,                     NULL }
  }
};

BOOL WINAPI SnpQuery(DWORD dwIndex, DWORD *dwNetworkCode, char **ppszNetworkName, char **ppszNetworkDescription, caps **ppCaps)
{
  if ( dwNetworkCode && ppszNetworkName && ppszNetworkDescription && ppCaps )
  {
    switch (dwIndex)
    {
    case 0:
      *dwNetworkCode          = networks[0].dwIdentifier;
      *ppszNetworkName        = networks[0].pszName;
      *ppszNetworkDescription = networks[0].pszDescription;
      *ppCaps                 = &networks[0].Caps;
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

BOOL WINAPI SnpBind(DWORD dwIndex, netFunctions **ppFxns)
{
  if ( ppFxns )
  {
    switch (dwIndex)
    {
    case 0:
      *ppFxns = &networks[0].NetFxns;
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch(fdwReason)
  {
  case DLL_PROCESS_ATTACH:
    ghInstance = hinstDLL;
    break;
  default:
    break;
  }
  return TRUE;
}
