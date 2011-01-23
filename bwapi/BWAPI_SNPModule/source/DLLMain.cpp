#include <windows.h>

#include "LocalPC.h"
#include "Template.h"

netModule networks[] = {
  { "Local PC (UDP)", 'LUDP', "",
    { sizeof(CAPS), 0, PKT_SIZE, 0, 0, 1000, 50, 8, 0},
    { sizeof(netFunctions),
      &COMN::spiCompareNetAddresses,
      &LUDP::spiDestroy,
      &COMN::spiFree,
      &COMN::spiFreeExternalMessage,
      &COMN::spiGetGameInfo,
      &COMN::spiGetPerformanceData,
      &LUDP::spiInitialize,
      &COMN::spiInitializeDevice,
      &COMN::spiLockDeviceList,
      &LUDP::spiLockGameList,
      &LUDP::spiReceive,
      &COMN::spiReceiveExternalMessage,
      &COMN::spiSelectGame,
      &LUDP::spiSend,
      &COMN::spiSendExternalMessage,
      &LUDP::spiStartAdvertisingLadderGame,
      &LUDP::spiStopAdvertisingGame,
      &COMN::spiUnlockDeviceList,
      &LUDP::spiUnlockGameList,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      &COMN::spiLeagueGetName }
  },
  { "Local PC (Pipes)", 'LPIP', "",
    { sizeof(CAPS), 0, PKT_SIZE, 0, 0, 100000, 200, 8, 0},
    { sizeof(netFunctions),
      &COMN::spiCompareNetAddresses,
      &LPIP::spiDestroy,
      &COMN::spiFree,
      &COMN::spiFreeExternalMessage,
      &COMN::spiGetGameInfo,
      &COMN::spiGetPerformanceData,
      &LPIP::spiInitialize,
      &COMN::spiInitializeDevice,
      &COMN::spiLockDeviceList,
      &LPIP::spiLockGameList,
      &LPIP::spiReceive,
      &COMN::spiReceiveExternalMessage,
      &COMN::spiSelectGame,
      &LPIP::spiSend,
      &COMN::spiSendExternalMessage,
      &LPIP::spiStartAdvertisingLadderGame,
      &LPIP::spiStopAdvertisingGame,
      &COMN::spiUnlockDeviceList,
      &LPIP::spiUnlockGameList,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      &COMN::spiLeagueGetName }
  }
};

BOOL WINAPI SnpQuery(DWORD dwIndex, DWORD *dwNetworkCode, char **ppszNetworkName, char **ppszNetworkDescription, CAPS **ppCaps)
{
  if ( dwNetworkCode && ppszNetworkName && ppszNetworkDescription && ppCaps )
  {
    switch (dwIndex)
    {
    case LUDP_ID:
      *dwNetworkCode          =  networks[LUDP_ID].dwIdentifier;
      *ppszNetworkName        =  networks[LUDP_ID].pszName;
      *ppszNetworkDescription =  networks[LUDP_ID].pszDescription;
      *ppCaps                 = &networks[LUDP_ID].caps;
      return TRUE;
    case LPIP_ID:
      *dwNetworkCode          =  networks[LPIP_ID].dwIdentifier;
      *ppszNetworkName        =  networks[LPIP_ID].pszName;
      *ppszNetworkDescription =  networks[LPIP_ID].pszDescription;
      *ppCaps                 = &networks[LPIP_ID].caps;
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
    case LUDP_ID:
      *ppFxns = &networks[LUDP_ID].NetFxns;
      return TRUE;
    case LPIP_ID:
      *ppFxns = &networks[LPIP_ID].NetFxns;
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
    InitializeCriticalSection(&gCrit);

    gdwProcId = GetCurrentProcessId();
    if ( gdwProcId == 0 )
      Error(ERROR_INVALID_PARAMETER, "Proc ID is 0");

    // Retrieve Starcraft path
    if ( SRegLoadString("Starcraft", "InstallPath", SREG_LOCAL_MACHINE, gszInstallPath, MAX_PATH) )
      SStrNCat(gszInstallPath, "\\", MAX_PATH);

    // Retrieve config path
    SStrCopy(gszConfigPath, gszInstallPath, MAX_PATH);
    SStrNCat(gszConfigPath, "bwapi-data\\bwapi.ini", MAX_PATH);

    // Retrieve log path
    GetPrivateProfileString("paths", "log_path", "bwapi-data\\logs", gszLogPath, MAX_PATH, gszConfigPath);
    SStrNCat(gszLogPath, "\\SNPModule_", MAX_PATH);

    char tBuf[16];
    SStrNCat(gszLogPath, itoa(gdwProcId, tBuf, 10), MAX_PATH);
    SStrNCat(gszLogPath, ".log", MAX_PATH);
    break;
  case DLL_PROCESS_DETACH:
    DeleteCriticalSection(&gCrit);
    break;
  default:
    break;
  }
  return TRUE;
}
