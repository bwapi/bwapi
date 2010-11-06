#include <windows.h>
#include "../../svnrev.h"
#include "../../starcraftver.h"

#include "LocalPC.h"
#include "Template.h"

netModule networks[] = {
  { "Local PC", 'LUDP', "BWAPI " STARCRAFT_VER " r" SVN_REV_STR "\n\nConnect multiple instances of Starcraft together on the same PC via winsock UDP.",
    { sizeof(caps), 0x20000003, LUDP_PKT_SIZE, 0x10, 0x100, 100000, 50, 8, 0},
    { sizeof(netFunctions),
      &COMN::fxn0,
      &LUDP::spiDestroy,
      &COMN::spiFree,
      &COMN::spiError,
      &COMN::spiGetGameInfo,
      &COMN::spiGetPerformanceData,
      &LUDP::spiInitializeProvider,
      &COMN::spiInitializeDevice,
      &COMN::spiEnumDevices,
      &LUDP::spiLockGameList,
      &LUDP::spiReceiveFrom,
      &COMN::spiReceive,
      &COMN::spiSelectGame,
      &LUDP::spiSendTo,
      &COMN::spiSend,
      &LUDP::spiStartAdvertisingLadderGame,
      &LUDP::spiStopAdvertisingGame,
      &COMN::spiInitialize,
      &LUDP::spiUnlockGameList,
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

BOOL WINAPI SnpQuery(DWORD dwIndex, DWORD *dwNetworkCode, char **ppszNetworkName, char **ppszNetworkDescription, caps **ppCaps)
{
  if ( dwNetworkCode && ppszNetworkName && ppszNetworkDescription && ppCaps )
  {
    switch (dwIndex)
    {
    case LUDP_ID:
      *dwNetworkCode          =  networks[LUDP_ID].dwIdentifier;
      *ppszNetworkName        =  networks[LUDP_ID].pszName;
      *ppszNetworkDescription =  networks[LUDP_ID].pszDescription;
      *ppCaps                 = &networks[LUDP_ID].Caps;
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
