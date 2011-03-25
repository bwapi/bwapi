#include "CodePatch.h"
#include "NewHackUtil.h"

#include "BW/Offsets.h"
#include "BWAPI/GameImpl.h"
#include "Detours.h"
#include "Resolution.h"
#include "WMode.h"

#include "../../Debug.h"

void ApplyCodePatches()
{
  /* Create function-level hooks */
  HackUtil::CallPatch(BW::BWFXN_NextLogicFrame, &_nextFrameHook);
  HackUtil::JmpPatch(BW::BWFXN_QueueCommand,    &CommandFilter);
  HackUtil::JmpPatch(HackUtil::GetImport("storm.dll", 251), &_SFileAuthenticateArchive);
  HackUtil::JmpPatch(BW::BWFXN_DDrawDestroy,    &DDrawDestroy);
  HackUtil::JmpPatch(BW::BWFXN_NetSelectReturnMenu, &_SelectReturnMenu);

  /* Perform code patches */
  char zero = 0;
  HackUtil::WriteNops(BW::BWDATA_MenuLoadHack, 11);            // main menu load timer
  HackUtil::WriteMem(BW::BWDATA_MenuInHack, &zero, 1);         // menu in speed
  HackUtil::WriteMem(BW::BWDATA_MenuOutHack, &zero, 1);        // menu out speed
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack, &zero, 1);    // BNET Server menu in speed
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack2, &zero, 1);   // BNET Server menu out speed
  HackUtil::WriteMem(BW::BWDATA_OpponentStartHack, &zero, 1);  // Start without an opponent

  /* Create import detours */
  HackUtil::PatchImport("storm.dll", 119, &_SNetLeaveGame);
  HackUtil::PatchImport("storm.dll", 121, &_SNetReceiveMessage);
  HackUtil::PatchImport("storm.dll", 128, &_SNetSendTurn);
  HackUtil::PatchImport("storm.dll", 266, &_SFileOpenArchive);
  HackUtil::PatchImport("storm.dll", 267, &_SFileOpenFile);
  HackUtil::PatchImport("storm.dll", 268, &_SFileOpenFileEx);
  HackUtil::PatchImport("storm.dll", 342, &_SDrawCaptureScreen);
  HackUtil::PatchImport("storm.dll", 401, &_SMemAlloc);
  HackUtil::PatchImport("storm.dll", 501, &_SStrCopy);

  // wmode/drawing
  HackUtil::PatchImport("user32.dll", "GetCursorPos", &_GetCursorPos);
  HackUtil::PatchImport("user32.dll", "SetCursorPos", &_SetCursorPos);
  HackUtil::PatchImport("user32.dll", "ClipCursor", &_ClipCursor);
  HackUtil::PatchImport("storm.dll", 350, &_SDrawLockSurface);
  HackUtil::PatchImport("storm.dll", 354, &_SDrawRealizePalette);
  HackUtil::PatchImport("storm.dll", 356, &_SDrawUnlockSurface);
  HackUtil::PatchImport("storm.dll", 357, &_SDrawUpdatePalette);
}

//----------------------------------------- NET-MODE RETURN MENU ---------------------------------------------
void _SelectReturnMenu()
{
  switch ( *BW::BWDATA_NetMode )
  {
  case 'BNET':
    *BW::BWDATA_glGluesMode = 4;  // battle.net
    break;
  case 'IPXN':
  case 'ATLK':
  case 'IPXX':
  case 'UDPN':
  case 'LUDP':
  case 'LPIP':
  case 'DRIP':
  case 'SMEM':
    *BW::BWDATA_glGluesMode = 10; // game select
    break;
  case 'MDMX':
  case 'MODM':
    *BW::BWDATA_glGluesMode = 20; // modem
    break;
  case 'SCBL':
    *BW::BWDATA_glGluesMode = 21; // direct connect
    break;
  default:
    *BW::BWDATA_glGluesMode = 0;  // main menu
    break;
  }
}
