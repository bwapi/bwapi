#include "Resolution.h"
#include "WMode.h"
#include "DLLMain.h"

#include <BWAPI/CoordinateType.h>

#include "BW/Offsets.h"
#include "BW/Sprite.h"
#include "BW/Image.h"

#include "NewHackUtil.h"

#include "../../Debug.h"

void SetBitmap(BW::bitmap *Bitmap, int width, int height, void *data)
{
  if ( !Bitmap )
    return;
  Bitmap->wid   = (WORD)width;
  Bitmap->ht    = (WORD)height;
  Bitmap->data  = (BYTE*)data;
}

void SetResolution(int width, int height)
{
  if ( !isCorrectVersion )
    return;

  // Resize game screen data buffer
  void *newBuf = SMAlloc(width * height);
  void *oldBuf = BW::BWDATA_GameScreenBuffer->data;
  SetBitmap(BW::BWDATA_GameScreenBuffer, width, height, newBuf);
  if ( oldBuf )
    SMFree(oldBuf);

  // Set new screen limits
  BW::BWDATA_ScreenLayers[5].width  = (WORD)width;
  BW::BWDATA_ScreenLayers[5].height = (WORD)height;
  SetRect(BW::BWDATA_ScrLimit, 0, 0, width - 1, height - 1);
  SetRect(BW::BWDATA_ScrSize,  0, 0, width,     height);

  // Resize game screen console (HUD) buffer
  newBuf = SMAlloc(width * height);
  oldBuf = BW::BWDATA_GameScreenConsole->data;
  SetBitmap(BW::BWDATA_GameScreenConsole, width, height, newBuf);
  if ( oldBuf )
    SMFree(oldBuf);

  // Recreate STrans thingy
  BW::BlizzVectorEntry<BW::TransVectorEntry> *transEntry = BW::BWDATA_TransMaskVector->begin;
  if ( (u32)transEntry && (u32)transEntry != (u32)&BW::BWDATA_TransMaskVector->begin )
  {
    HANDLE oldTrans = transEntry->container.hTrans;
    SetRect(&transEntry->container.info, 0, 0, width, height);
    STransCreateE(newBuf, width, height, 8, 0, 0, &transEntry->container.hTrans);
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
  SDrawManualInitialize(ghMainWnd);
  if ( *BW::BWDATA_PrimaryPalette )
    (*BW::BWDATA_PrimaryPalette)->Release();
  *BW::BWDATA_PrimaryPalette = NULL;

  if ( *BW::BWDATA_PrimarySurface > (LPDIRECTDRAWSURFACE)1 )
    (*BW::BWDATA_PrimarySurface)->Release();
  *BW::BWDATA_PrimarySurface = NULL;

  if ( *BW::BWDATA_BackSurface )
    (*BW::BWDATA_BackSurface)->Release();
  *BW::BWDATA_BackSurface = NULL;

  if ( *BW::BWDATA_DDInterface )
    (*BW::BWDATA_DDInterface)->Release();
  *BW::BWDATA_DDInterface = NULL;

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
  *(FARPROC*)&_DirectDrawCreate = GetProcAddress(ddLib, "DirectDrawCreate");
  DDCHECK(_DirectDrawCreate);

  // Create and initialize DirectDrawInterface
  DDCHECK( _DirectDrawCreate(NULL, BW::BWDATA_DDInterface, NULL) == DD_OK );
  DDCHECK( (*BW::BWDATA_DDInterface)->SetCooperativeLevel(ghMainWnd, DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE) == DD_OK );
  if ( (*BW::BWDATA_DDInterface)->SetDisplayMode(width, height, 8) != DD_OK )
    DDCHECK( (*BW::BWDATA_DDInterface)->SetDisplayMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 8) == DD_OK );

  // Create DirectDrawPalette
  DDCHECK( (*BW::BWDATA_DDInterface)->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, BW::BWDATA_GamePalette, BW::BWDATA_PrimaryPalette, NULL) == DD_OK );

  DDSURFACEDESC surfaceDesc = { 0 };
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

void updateImageDrawingData()
{
  for ( int y = 0; y < BW::BWDATA_MapSize->y; ++y )
  {
    for ( BW::Sprite *s = BW::BWDATA_spriteGroups[y]; s; s = s->next )
    {
      for ( BW::Image *i = s->underlay; i; i = i->next )
        i->updateGraphicData();
    }
  }
}

void blitGameText(int line, int x, int y)
{
  if ( BW::BWDATA_Chat_GameText[line].txt[0] )
  {
    char szString[256];
    szString[0] = 0;
    if ( BW::BWDATA_Chat_ColorBytes[line] )
    {
      szString[0] = BW::BWDATA_Chat_ColorBytes[line];
      szString[1] = 0;
    }
    strncat(szString, BW::BWDATA_Chat_GameText[line].txt, sizeof(BW::_gametext));
    BW::BlitText(szString, BW::BWDATA_GameScreenBuffer, x, y, 1);
  }
}

void drawGameText()
{
  int l = *BW::BWDATA_Chat_NextLine;

  int y = 112;
  for ( int i = 11; i > 0; i-- )
  {
    blitGameText(l, 10, y);
    y += *BW::BWDATA_Chat_IncrementY;
    l = (l + 1) % 11;
  }
  blitGameText(12, 10, 295);
  blitGameText(11, 420, 24);
}

void GameUpdate(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  updateImageDrawingData();   //BW::BWFXN_updateImageData();
  BW::BWFXN_drawMapTiles();
  BW::BWFXN_blitMapTiles();

  //
  BW::BWFXN_drawAllSprites();// sprites
  // Space tileset stars
  drawGameText();
  BW::BWFXN_drawDragSelBox();
  BW::BWFXN_drawAllThingys(); // draw thingys
}
