#include "Resolution.h"
#include "WMode.h"
#include "DLLMain.h"
#include "BW/Offsets.h"
#include "NewHackUtil.h"

void SetResolution(int width, int height)
{
  HWND hWnd = SDrawGetFrameWindow();
  MoveWindow(hWnd, 0, 0, width, height, TRUE);

  if ( !ghMainWnd )
  {
    // re-initialize directdraw here
    DDrawDestroy();
    DWORD dwScrWid = width;
    DWORD dwScrHgt = height;
    // these hacks are only temporary, gonna rewrite the initialization
    HackUtil::WriteMem(0x0041DA43, &dwScrWid, 4);
    HackUtil::WriteMem(0x0041DB9F, &dwScrWid, 4);
    HackUtil::WriteMem(0x0041DA3E, &dwScrHgt, 4);
    HackUtil::WriteMem(0x0041DBA6, &dwScrHgt, 4);
    BW::BWFXN_DDrawInitialize();
  }

  void *newBuf = SMAlloc(width * height);
  void *oldBuf = BW::BWDATA_GameScreenBuffer->data;
  
  SMemFill(newBuf, width * height, 33);

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
