#include <windows.h>
#include "../../svnrev.h"

#include "LocalPC.h"

HINSTANCE ghInstance;

struct caps
{
  DWORD dwSize;
  DWORD dwUnk1;
  DWORD dwUnk2;
  DWORD dwUnk3;
  DWORD dwUnk4;
  DWORD dwUnk5;
  DWORD dwUnk6;
  DWORD dwPlayerCount;
  DWORD dwUnk8;
};

struct netFunctions
{
  DWORD dwSize;
  void  *fxn_0;
  bool  (__stdcall *spiDestroy)(); // official
  void  *spiFree; // official spiFree(0x%08x,0x%08x,%u)
  void  *spiError;
  void  *fxn_4;
  void  *fxn_5;
  void  *spiInitialize;  // official spiInitialize(0x%08x)
  void  *fxn_7;
  void  *fxn_8;
  void  *spiLockGameList; // official spiLockGameList(0x%08x,0x%08x,*gamelist) (0x%08x,0x%08x,*gamehead)
  void  *fxn_10;
  void  *spiReceive; // official spiReceive(*addr,*data,*databytes) (int *, int *, char **)
  void  *fxn_12;
  bool (__stdcall *spiSend)(DWORD addrCount, sockaddr **addrList, void *buf, DWORD bufLen);
  void  *fxn_14;
  void  *spiStartAdvertisingLadderGame;
  bool (__stdcall *spiStopAdvertisingGame)(); // official
  bool (__stdcall *InitializeUser)();
  void  *spiUnlockGameList; // official spiUnlockGameList(0x%08x,*hintnextcall)
  void  *spiStartAdvertisingGame; // official spiStartAdvertisingGame(0x%08x,0x%08x,"%s","%s")
  void  *fxn_20;
  void  *fxn_21;
  void  *fxn_22;
  void  *fxn_23;
  void  *fxn_24;
  void  *fxn_25;
  bool (__stdcall *GetLeagueName)(char *pszDest, DWORD dwSize);
};

struct netStruct
{
  char          *pszName;
  DWORD         dwIdentifier;
  char          *pszDescription;
  caps          Caps;
  netFunctions  NetFxns;
} networks[] = {
  { "Local PC", 'LOCL', "BWAPI 1.16.1 r" SVN_REV_STR "\n\nConnect multiple instances of Starcraft together on the same PC.",
    { sizeof(caps), 0x20000000, 0x200, 0x10, 0x100, 100000, 50, 8, 2},
    { sizeof(netFunctions), &fxn0, &_spiDestroy, &_spiFree, &_spiError,
      &fxn4, &fxn5, &_spiInitialize, &fxn7,
      &fxn8, &_spiLockGameList, &fxn10, &_spiReceive,
      &fxn12, &_spiSend, &fxn14, &_spiStartAdvertisingLadderGame,
      &_spiStopAdvertisingGame, &InitializeUser, &_spiUnlockGameList, NULL,
      NULL, NULL, NULL, NULL,
      NULL, NULL, NULL}
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
