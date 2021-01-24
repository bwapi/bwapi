#include "SNPNetwork.h"
#include <windows.h>

// these modi are implemented in this DLL
#include "DirectIP.h"
#define DRIP_ID 0
#include "LocalPC.h"
#define SMEM_ID 1

BOOL WINAPI SnpQuery(DWORD dwIndex, DWORD *dwNetworkCode, LPCSTR *ppszNetworkName, LPCSTR *ppszNetworkDescription, SNETCAPSPTR* ppCaps)
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

BOOL WINAPI SnpBind(DWORD dwIndex, SNETSPIPTR* ppFxns)
{
  if ( ppFxns )
  {
    switch (dwIndex)
    {
    case DRIP_ID:
      *ppFxns = &SNP::spiFunctions;
      SNP::pluggedNetwork = new DRIP::DirectIP();
      return TRUE;
    case SMEM_ID:
      *ppFxns = &SNP::spiFunctions;
      SNP::pluggedNetwork = new SMEM::LocalPC();
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch(fdwReason)
  {
  case DLL_PROCESS_ATTACH:
    hInstance = hinstDLL;
    break;
  case DLL_PROCESS_DETACH:
    break;
  default:
    break;
  }
  return TRUE;
}
