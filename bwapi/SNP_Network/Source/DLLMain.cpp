#include "SNPModule.h"
#include <windows.h>

// these modi are implemented in this DLL
#include "DirectIP.h"
#define DRIP_ID 0
#include "LocalPC.h"
#define SMEM_ID 1


BOOL WINAPI SnpQuery(DWORD dwIndex, DWORD *dwNetworkCode, char **ppszNetworkName, char **ppszNetworkDescription, CAPS **ppCaps)
{
  if ( dwNetworkCode && ppszNetworkName && ppszNetworkDescription && ppCaps )
  {
    switch (dwIndex)
    {
    case DRIP_ID:
      *dwNetworkCode          =  DRIP::networkInfo.dwIdentifier;
      *ppszNetworkName        =  DRIP::networkInfo.pszName;
      *ppszNetworkDescription =  DRIP::networkInfo.pszDescription;
      *ppCaps                 = &DRIP::networkInfo.caps;
      return TRUE;
    case SMEM_ID:
      *dwNetworkCode          =  SMEM::networkInfo.dwIdentifier;
      *ppszNetworkName        =  SMEM::networkInfo.pszName;
      *ppszNetworkDescription =  SMEM::networkInfo.pszDescription;
      *ppCaps                 = &SMEM::networkInfo.caps;
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

BOOL WINAPI SnpBind(DWORD dwIndex, SNP::NetFunctions **ppFxns)
{
  if ( ppFxns )
  {
    switch (dwIndex)
    {
    case DRIP_ID:
      *ppFxns = &SNP::spiFunctions;
      SNP::pluggedNetwork = (SNP::Network<SOCKADDR>*)(new DRIP::DirectIP());
      return TRUE;
    case SMEM_ID:
      *ppFxns = &SNP::spiFunctions;
      SNP::pluggedNetwork = (SNP::Network<SOCKADDR>*)(new SMEM::LocalPC());
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

CRITICAL_SECTION globalCriticalSection;

HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID)
{
  switch(fdwReason)
  {
  case DLL_PROCESS_ATTACH:
    InitializeCriticalSection(&globalCriticalSection);
    hInstance = hinstDLL;
    break;
  case DLL_PROCESS_DETACH:
    DeleteCriticalSection(&globalCriticalSection);
    break;
  default:
    break;
  }
  return TRUE;
}
