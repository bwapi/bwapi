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
  bool  (__stdcall *Cleanup)();
  void  *fxn_2;
  void  *fxn_3;
  void  *fxn_4;
  void  *fxn_5;
  void  *fxn_6;
  void  *fxn_7;
  void  *fxn_8;
  void  *fxn_9;
  void  *fxn_10;
  void  *fxn_11;
  void  *fxn_12;
  void  *fxn_13;
  void  *fxn_14;
  void  *fxn_15;
  bool (__stdcall *LeaveGame)();
  bool (__stdcall *InitializeUser)();
  void  *fxn_18;
  void  *fxn_19;
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
    { sizeof(netFunctions), &fxn0, &Cleanup, &fxn2, &fxn3,
      &fxn4, &fxn5, &InitializeModule, &fxn7,
      &fxn8, &fxn9, &fxn10, &fxn11,
      &fxn12, &fxn13, &fxn14, &CreateGame,
      &LeaveGame, &InitializeUser, &fxn18, NULL,
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
