#include <windows.h>
#include "../../svnrev.h"
#include "../../starcraftver.h"

#include "LocalPC.h"
#include "Template.h"

netModule networks[] = {
  { "Local PC", 'LOCL', "BWAPI " STARCRAFT_VER " r" SVN_REV_STR "\n\nConnect multiple instances of Starcraft together on the same PC.",
    { sizeof(caps), 0x20000000, LOCL_PKT_SIZE, 0x10, 0x100, 100000, 100, 8, 0},
    { sizeof(netFunctions),
      &fxn0,                    &_spiDestroy,             &_spiFree,                &_spiError,
      &_spiGetGameInfo,         &_spiGetPerformanceData,  &_spiInitializeProvider,  &_spiInitializeDevice,
      &_spiEnumDevices,         &_spiLockGameList,        &_spiReceiveFrom,         &_spiReceive,
      &_spiSelectGame,          &_spiSendTo,              &_spiSend,                &_spiStartAdvertisingLadderGame,
      &_spiStopAdvertisingGame, &_spiInitialize,          &_spiUnlockGameList,      NULL,
      NULL,                     NULL,                     NULL,                     NULL,
      NULL,                     NULL,                     NULL }
  }
};

BOOL WINAPI SnpQuery(DWORD dwIndex, DWORD *dwNetworkCode, char **ppszNetworkName, char **ppszNetworkDescription, caps **ppCaps)
{
  if ( dwNetworkCode && ppszNetworkName && ppszNetworkDescription && ppCaps )
  {
    switch (dwIndex)
    {
    case LOCL:
      *dwNetworkCode          = networks[LOCL].dwIdentifier;
      *ppszNetworkName        = networks[LOCL].pszName;
      *ppszNetworkDescription = networks[LOCL].pszDescription;
      *ppCaps                 = &networks[LOCL].Caps;
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
    case LOCL:
      *ppFxns = &networks[LOCL].NetFxns;
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
    break;
  default:
    break;
  }
  return TRUE;
}
