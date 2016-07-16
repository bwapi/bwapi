#include "Resolution.h"
#include "WMode.h"
#include "Config.h"
#include "DLLMain.h"

#include <BWAPI/CoordinateType.h>

#include "BW/Offsets.h"
#include "BW/Dialog.h"

#include "NewHackUtil.h"

#include "../../Debug.h"

HMODULE ddLib;
void DDrawDestroy()
{
#ifndef SHADOW_BROODWAR
  SDrawManualInitialize(ghMainWnd);   // NOTE: Necessary so Storm knows the main window
  if ( BW::BWDATA::PrimaryPalette )
    (BW::BWDATA::PrimaryPalette)->Release();
  BW::BWDATA::PrimaryPalette = NULL;

  if ( BW::BWDATA::PrimarySurface > (LPDIRECTDRAWSURFACE)1 )
    (BW::BWDATA::PrimarySurface)->Release();
  BW::BWDATA::PrimarySurface = NULL;

  if ( BW::BWDATA::BackSurface )
    (BW::BWDATA::BackSurface)->Release();
  BW::BWDATA::BackSurface = NULL;

  if ( BW::BWDATA::DDInterface )
    (BW::BWDATA::DDInterface)->Release();
  BW::BWDATA::DDInterface = NULL;

  if ( ghMainWnd )
    ShowWindow(ghMainWnd, SW_MINIMIZE);
#endif
}

#define DDCHECK(x) { if ( !(x) ) BWAPIError(GetLastError(), "\"" #x "\" failed on %s:%u.", __FILE__, __LINE__); }

void DDrawInitialize(int width, int height)
{
#ifndef SHADOW_BROODWAR
  if ( !ghMainWnd || wmode )
    return;

  // Initialize module
  ShowWindow(ghMainWnd, SW_NORMAL);
  if ( !ddLib )
  {
    ddLib = LoadLibraryA("ddraw.dll");
    DDCHECK(ddLib);
  }

  // Obtain function DirectDrawCreate
  HRESULT (WINAPI *_DirectDrawCreate)(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter);
  (FARPROC&)_DirectDrawCreate = GetProcAddress(ddLib, "DirectDrawCreate");
  DDCHECK(_DirectDrawCreate);

  // Create and initialize DirectDrawInterface
  DDCHECK( _DirectDrawCreate(NULL, &BW::BWDATA::DDInterface, NULL) == DD_OK );
  DDCHECK( BW::BWDATA::DDInterface->SetCooperativeLevel(ghMainWnd, DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE) == DD_OK );
  if ( BW::BWDATA::DDInterface->SetDisplayMode(width, height, 8) != DD_OK )
    DDCHECK( BW::BWDATA::DDInterface->SetDisplayMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 8) == DD_OK );

  // Create DirectDrawPalette
  DDCHECK( BW::BWDATA::DDInterface->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, BW::BWDATA::GamePalette.data(), &BW::BWDATA::PrimaryPalette, NULL) == DD_OK );

  DDSURFACEDESC surfaceDesc = { 0 };
  surfaceDesc.dwSize          = sizeof(DDSURFACEDESC);
  surfaceDesc.dwFlags         = DDSD_CAPS;
  surfaceDesc.ddsCaps.dwCaps  = DDSCAPS_PRIMARYSURFACE;

  DDCHECK( BW::BWDATA::DDInterface->CreateSurface(&surfaceDesc, &BW::BWDATA::PrimarySurface, NULL) == DD_OK );
  DDCHECK( BW::BWDATA::PrimarySurface->SetPalette(BW::BWDATA::PrimaryPalette) == DD_OK );
  if ( BW::BWDATA::PrimarySurface->Lock(NULL, &surfaceDesc, DDLOCK_WAIT, NULL) != DD_OK )
  {
    memset(&surfaceDesc, 0, sizeof(DDSURFACEDESC));
    surfaceDesc.dwSize          = sizeof(DDSURFACEDESC);
    surfaceDesc.dwFlags         = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    surfaceDesc.ddsCaps.dwCaps  = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
    surfaceDesc.dwWidth         = width;
    surfaceDesc.dwHeight        = height;

    DDCHECK( BW::BWDATA::DDInterface->CreateSurface(&surfaceDesc, &BW::BWDATA::BackSurface, NULL) == DD_OK );
  }
  else
  {
    BW::BWDATA::PrimarySurface->Unlock(&surfaceDesc);
  }
  SDrawManualInitialize(ghMainWnd, BW::BWDATA::DDInterface, BW::BWDATA::PrimarySurface, NULL, NULL, BW::BWDATA::BackSurface, BW::BWDATA::PrimaryPalette, NULL);
#endif
}
#undef DDCHECK