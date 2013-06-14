#include "Resolution.h"
#include "WMode.h"
#include "Config.h"
#include "DLLMain.h"

#include <BWAPI/CoordinateType.h>

#include "BW/Offsets.h"
#include "BW/Dialog.h"
#include "GameDrawing.h"

#include "NewHackUtil.h"

#include "../../Debug.h"

void SetResolution(int width, int height)
{
  if ( !isCorrectVersion )
    return;

  // Resize game screen data buffer
  BW::BWDATA::GameScreenBuffer->resize(width, height);

  // Resize game terrain cache buffer
  bmpTerrainCache.resize(width+32, height-32);
  //*BW::BWDATA::GameTerrainCache = (BYTE*)SMReAlloc(*BW::BWDATA::GameTerrainBuffer, (width+32)*(height-32) );

  // Set new screen limits
  BW::BWDATA::ScreenLayers[5].width  = (WORD)width;
  BW::BWDATA::ScreenLayers[5].height = (WORD)height;
  SetRect(BW::BWDATA::ScrLimit, 0, 0, width - 1, height - 1);
  SetRect(BW::BWDATA::ScrSize,  0, 0, width,     height);
  
  // Resize game screen console (HUD) buffer
  BW::BWDATA::GameScreenConsole->resize(width, height);
  
  // Recreate STrans thingy
  BW::BlizzVectorEntry<BW::TransVectorEntry> *transEntry = BW::BWDATA::TransMaskVector->begin;
  if ( (u32)transEntry && (u32)transEntry != (u32)&BW::BWDATA::TransMaskVector->begin )
  {
    HANDLE oldTrans = transEntry->container.hTrans;
    SetRect(&transEntry->container.info, 0, 0, width, height);
    STransCreateE(BW::BWDATA::GameScreenConsole->getData(), width, height, 8, 0, 0, &transEntry->container.hTrans);
    if ( oldTrans )
      STransDelete(oldTrans);

    // call a function that does some weird stuff
    BW::BWFXN_UpdateBltMasks();
  }

  STransSetDirtyArrayInfo(width, height, 16, 16);
  
  // re-initialize w-mode or ddraw, this function can do both
  SetWMode(width, height, wmode);
}

HMODULE ddLib;
void DDrawDestroy()
{
  SDrawManualInitialize(ghMainWnd);   // NOTE: Necessary so Storm knows the main window
  if ( *BW::BWDATA::PrimaryPalette )
    (*BW::BWDATA::PrimaryPalette)->Release();
  *BW::BWDATA::PrimaryPalette = NULL;

  if ( *BW::BWDATA::PrimarySurface > (LPDIRECTDRAWSURFACE)1 )
    (*BW::BWDATA::PrimarySurface)->Release();
  *BW::BWDATA::PrimarySurface = NULL;

  if ( *BW::BWDATA::BackSurface )
    (*BW::BWDATA::BackSurface)->Release();
  *BW::BWDATA::BackSurface = NULL;

  if ( *BW::BWDATA::DDInterface )
    (*BW::BWDATA::DDInterface)->Release();
  *BW::BWDATA::DDInterface = NULL;

  if ( ghMainWnd )
    ShowWindow(ghMainWnd, SW_MINIMIZE);
}

#define DDCHECK(x) { if ( !(x) ) BWAPIError(GetLastError(), "\"" #x "\" failed on %s:%u.", __FILE__, __LINE__); }

void DDrawInitialize(int width, int height)
{
  if ( !ghMainWnd || wmode )
    return;

  // Initialize module
  ShowWindow(ghMainWnd, SW_NORMAL);
  if ( !ddLib )
  {
    ddLib = LoadLibrary("ddraw.dll");
    DDCHECK(ddLib);
  }

  // Obtain function DirectDrawCreate
  HRESULT (WINAPI *_DirectDrawCreate)(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter);
  (FARPROC&)_DirectDrawCreate = GetProcAddress(ddLib, "DirectDrawCreate");
  DDCHECK(_DirectDrawCreate);

  // Create and initialize DirectDrawInterface
  DDCHECK( _DirectDrawCreate(NULL, BW::BWDATA::DDInterface, NULL) == DD_OK );
  DDCHECK( (*BW::BWDATA::DDInterface)->SetCooperativeLevel(ghMainWnd, DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE) == DD_OK );
  if ( (*BW::BWDATA::DDInterface)->SetDisplayMode(width, height, 8) != DD_OK )
    DDCHECK( (*BW::BWDATA::DDInterface)->SetDisplayMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 8) == DD_OK );

  // Create DirectDrawPalette
  DDCHECK( (*BW::BWDATA::DDInterface)->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, BW::BWDATA::GamePalette, BW::BWDATA::PrimaryPalette, NULL) == DD_OK );

  DDSURFACEDESC surfaceDesc = { 0 };
  surfaceDesc.dwSize          = sizeof(DDSURFACEDESC);
  surfaceDesc.dwFlags         = DDSD_CAPS;
  surfaceDesc.ddsCaps.dwCaps  = DDSCAPS_PRIMARYSURFACE;

  DDCHECK( (*BW::BWDATA::DDInterface)->CreateSurface(&surfaceDesc, BW::BWDATA::PrimarySurface, NULL) == DD_OK );
  DDCHECK( (*BW::BWDATA::PrimarySurface)->SetPalette(*BW::BWDATA::PrimaryPalette) == DD_OK );
  if ( (*BW::BWDATA::PrimarySurface)->Lock(NULL, &surfaceDesc, DDLOCK_WAIT, NULL) != DD_OK )
  {
    memset(&surfaceDesc, 0, sizeof(DDSURFACEDESC));
    surfaceDesc.dwSize          = sizeof(DDSURFACEDESC);
    surfaceDesc.dwFlags         = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    surfaceDesc.ddsCaps.dwCaps  = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
    surfaceDesc.dwWidth         = width;
    surfaceDesc.dwHeight        = height;

    DDCHECK( (*BW::BWDATA::DDInterface)->CreateSurface(&surfaceDesc, BW::BWDATA::BackSurface, NULL) == DD_OK );
  }
  else
  {
    (*BW::BWDATA::PrimarySurface)->Unlock(&surfaceDesc);
  }
  SDrawManualInitialize(ghMainWnd, *BW::BWDATA::DDInterface, *BW::BWDATA::PrimarySurface, NULL, NULL, *BW::BWDATA::BackSurface, *BW::BWDATA::PrimaryPalette, NULL);
}
