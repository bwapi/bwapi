#include "Resolution.h"
#include "WMode.h"
#include "DLLMain.h"
#include "BW/Offsets.h"
#include "NewHackUtil.h"

#include "../../Debug.h"

void SetResolution(int width, int height)
{
  HWND hWnd = SDrawGetFrameWindow();
  MoveWindow(hWnd, 0, 0, width, height, TRUE);

  if ( !wmode )
  {
    // re-initialize directdraw here
    DDrawDestroy();
    DDrawInitialize(width, height);
  }
  // Resize buffers and stuff

  void *newBuf = SMAlloc(width * height);
  void *oldBuf = BW::BWDATA_GameScreenBuffer->data;
  
  memset(newBuf, 0, width * height);

  BW::BWDATA_GameScreenBuffer->data = (u8*)newBuf;
  BW::BWDATA_GameScreenBuffer->wid = (u16)width;
  BW::BWDATA_GameScreenBuffer->ht = (u16)height;
  if ( oldBuf )
    SMFree(oldBuf);

  BW::BWDATA_ScreenLayers[5].width = (WORD)width;
  BW::BWDATA_ScreenLayers[5].height = (WORD)height;
  BW::BWDATA_ScrLimit->right  = width - 1;
  BW::BWDATA_ScrLimit->bottom = height - 1;
  BW::BWDATA_ScrSize->right   = width;
  BW::BWDATA_ScrSize->bottom  = height;

  newBuf = SMAlloc(width * height);
  oldBuf = BW::BWDATA_GameScreenConsole->data;

  BW::BWDATA_GameScreenConsole->data = (u8*)newBuf;
  BW::BWDATA_GameScreenConsole->wid = (u16)width;
  BW::BWDATA_GameScreenConsole->ht = (u16)height;
  if ( oldBuf )
    SMFree(oldBuf);

  if ( (*BW::BWDATA_MainBltMask)->hTrans )
    STransDelete((*BW::BWDATA_MainBltMask)->hTrans);

  (*BW::BWDATA_MainBltMask)->info.right  = width;
  (*BW::BWDATA_MainBltMask)->info.bottom = height;
  STransCreateE(newBuf, width, height, 8, 0, 0, &(*BW::BWDATA_MainBltMask)->hTrans);

  BW::BWFXN_UpdateBltMasks();

  if ( hdcMem )
    InitializeWModeBitmap(width, height);
}

void DDrawDestroy()
{
  SDrawManualInitialize(ghMainWnd);
  if ( *BW::BWDATA_PrimaryPalette )
  {
    (*BW::BWDATA_PrimaryPalette)->Release();
    *BW::BWDATA_PrimaryPalette = NULL;
  }
  if ( *BW::BWDATA_PrimarySurface > (LPDIRECTDRAWSURFACE)1 )
  {
    (*BW::BWDATA_PrimarySurface)->Release();
    *BW::BWDATA_PrimarySurface = NULL;
  }
  if ( *BW::BWDATA_BackSurface )
  {
    (*BW::BWDATA_BackSurface)->Release();
    *BW::BWDATA_BackSurface = NULL;
  }
  if ( *BW::BWDATA_DDInterface )
  {
    (*BW::BWDATA_DDInterface)->Release();
    *BW::BWDATA_DDInterface = NULL;
  }
  if ( ghMainWnd )
    ShowWindow(ghMainWnd, SW_MINIMIZE);
}

#define DDCHECK(x) { if ( !(x) ) BWAPIError(GetLastError(), "\"" #x "\" failed on %s:%u.", __FILE__, __LINE__); }

void DDrawInitialize(int width, int height)
{
  if ( !ghMainWnd )
    return;

  // Initialize module
  ShowWindow(ghMainWnd, SW_NORMAL);
  HMODULE ddLib = LoadLibrary("ddraw.dll");
  DDCHECK(ddLib);

  // Obtain function DirectDrawCreate
  HRESULT (WINAPI *_DirectDrawCreate)(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter);
  *(FARPROC*)&_DirectDrawCreate = GetProcAddress(ddLib, "DirectDrawCreate");
  DDCHECK(_DirectDrawCreate);

  // Create and initialize DirectDrawInterface
  DDCHECK( _DirectDrawCreate(NULL, BW::BWDATA_DDInterface, NULL) == DD_OK );
  DDCHECK( (*BW::BWDATA_DDInterface)->SetCooperativeLevel(ghMainWnd, DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE) == DD_OK );
  if ( (*BW::BWDATA_DDInterface)->SetDisplayMode(width, height, 8) != DD_OK )
    DDCHECK( (*BW::BWDATA_DDInterface)->SetDisplayMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 8) == DD_OK );

  // Create DirectDrawPalette
  DDCHECK( (*BW::BWDATA_DDInterface)->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, BW::BWDATA_GamePalette, BW::BWDATA_PrimaryPalette, NULL) == DD_OK );

  DDSURFACEDESC surfaceDesc;
  memset(&surfaceDesc, 0, sizeof(DDSURFACEDESC));
  surfaceDesc.dwSize          = sizeof(DDSURFACEDESC);
  surfaceDesc.dwFlags         = DDSD_CAPS;
  surfaceDesc.ddsCaps.dwCaps  = DDSCAPS_PRIMARYSURFACE;

  DDCHECK( (*BW::BWDATA_DDInterface)->CreateSurface(&surfaceDesc, BW::BWDATA_PrimarySurface, NULL) == DD_OK );
  DDCHECK( (*BW::BWDATA_PrimarySurface)->SetPalette(*BW::BWDATA_PrimaryPalette) == DD_OK );
  if ( (*BW::BWDATA_PrimarySurface)->Lock(NULL, &surfaceDesc, DDLOCK_WAIT, NULL) != DD_OK )
  {
    memset(&surfaceDesc, 0, sizeof(DDSURFACEDESC));
    surfaceDesc.dwSize          = sizeof(DDSURFACEDESC);
    surfaceDesc.dwFlags         = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    surfaceDesc.ddsCaps.dwCaps  = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
    surfaceDesc.dwWidth         = width;
    surfaceDesc.dwHeight        = height;

    DDCHECK( (*BW::BWDATA_DDInterface)->CreateSurface(&surfaceDesc, BW::BWDATA_BackSurface, NULL) == DD_OK );
  }
  else
  {
    (*BW::BWDATA_PrimarySurface)->Unlock(&surfaceDesc);
  }
  SDrawManualInitialize(ghMainWnd, *BW::BWDATA_DDInterface, *BW::BWDATA_PrimarySurface, NULL, NULL, *BW::BWDATA_BackSurface, *BW::BWDATA_PrimaryPalette, NULL);
}
/*
void BlitToBitmap(DWORD dwOffset, int height, BYTE *pbBuffer, BYTE *pbData)
{
  for ( int i = height; i; --i )
  {
    DWORD _dwDrawW = gdwDrawWdth;
    if ( gdwDrawWdth + dwOffset >= (480-32)*(640+32) )
    {
      if ( dwOffset < (480-32)*(640+32) )
      {
        _dwDrawW = gdwDrawWdth + dwOffset - (480-32)*(640+32);
        DWORD dwFixDrawCopySize = (gdwDrawWdth - _dwDrawW) & 0xFFFFFFFC;
        memcpy(pbBuffer, pbData, dwFixDrawCopySize);
        pbData   += dwFixDrawCopySize;
        pbBuffer += dwFixDrawCopySize;
      }
      pbData   -= (480-32)*(640+32);
      dwOffset -= (480-32)*(640+32);
    }
    DWORD dwDrawCopySize = _dwDrawW & 0xFFFFFFFC;
    memcpy(pbBuffer, pbData, dwDrawCopySize);
    dwOffset += 640 + 32;
    pbData   = &pbData  [dwDrawCopySize + 640 + 32] - gdwDrawWdth;
    pbBuffer = &pbBuffer[dwDrawCopySize + 640] - gdwDrawWdth;
  }
}*/

void __stdcall GameUpdate(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  bool bitEnabled = BW::BWDATA_ScreenLayers[5].bits & 1;
  if ( bitEnabled )
  {
    // map tiles 1
  }
  else
  {
    // map tiles 2
  }
  // sprites
  // Space tileset stars
  // draw game text
  // draw drag Select box
  // draw thingys
}
