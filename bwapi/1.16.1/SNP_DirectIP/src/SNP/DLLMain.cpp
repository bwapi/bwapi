#include "SNPNetwork.h"
#include <windows.h>

// these modi are implemented in this DLL
#include "../DirectIP/DirectIP.h"
#define DRIP_ID 0
#include "../LocalPC/LocalPC.h"
#define SMEM_ID 1

BOOL WINAPI SnpQuery(DWORD index, DWORD *id, LPCSTR *description, LPCSTR *requirements, SNETCAPSPTR* caps)
{
  if ( id && description && requirements && caps )
  {
    switch (index)
    {
    case DRIP_ID:
      *id           =  DRIP::networkInfo.dwIdentifier;
      *description  =  DRIP::networkInfo.pszName;
      *requirements =  DRIP::networkInfo.pszDescription;
      *caps         = &DRIP::networkInfo.caps;
      return TRUE;
    case SMEM_ID:
      *id           = SMEM::networkInfo.dwIdentifier;
      *description  = SMEM::networkInfo.pszName;
      *requirements = SMEM::networkInfo.pszDescription;
      *caps         = &SMEM::networkInfo.caps;
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

BOOL WINAPI SnpBind(DWORD index, SNETSPIPTR* spi)
{
  if ( spi )
  {
    switch (index)
    {
    case DRIP_ID:
      *spi = &SNP::spiFunctions;
      SNP::pluggedNetwork = new DRIP::DirectIP();
      return TRUE;
    case SMEM_ID:
      *spi = &SNP::spiFunctions;
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
