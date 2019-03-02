#include "CodePatch.h"
#include "NewHackUtil.h"

#include "BW/Offsets.h"
#include "BW/MenuPosition.h"
#include "BWAPI/GameImpl.h"
#include "Detours.h"
#include "Assembly.h"

#include "Config.h"

#include "../../starcraftver.h"
#include "../../Debug.h"
#include <windows.h>

void ApplyCodePatches()
{
  // ---------------------------------- VERSION DEPENDENT ----------------------------------------------------
  // Only apply if version is correct
  if ( isCorrectVersion )
  {
    // Create function-level hooks
    HackUtil::CallPatch(BW::BWFXN_P_IsGamePaused, &_nextFrameHook);
    HackUtil::CallPatch(BW::BWFXN_ExecuteGameTrigsCallPatch, &ExecuteGameTriggers);
    HackUtil::WriteNops(BW::BWFXN_SpendRepair, 7);
    HackUtil::JmpPatch(BW::BWFXN_SpendRepair, &_repairHook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMinerals, &_refundMineralsHook);
    HackUtil::JmpPatch(BW::BWFXN_RefundGas, &_refundGasHook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMinAndGas2, &_refundMinAndGas2Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMin3, &_refundMin3Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundGas3, &_refundGas3Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMin4, &_refundMin4Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundGas4, &_refundGas4Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMinAndGas5, &_refundMinAndGas5Hook);
    HackUtil::JmpPatch(BW::BWDATA::BWFXN_QueueCommand, &CommandFilter);
    HackUtil::CallPatch(BW::BWFXN_RandomizeRacePatch, &_RandomizePlayerRaces);
    HackUtil::CallPatch(BW::BWFXN_InitPlayerConsolePatch, &_InitializePlayerConsole);
  }
  // ---------------------------------- VERSION INDEPENDENT --------------------------------------------------
  // Storm detours
  _SNetLeaveGameOld       = HackUtil::PatchImport("storm.dll", 119, &_SNetLeaveGame);
  _SNetReceiveMessageOld  = HackUtil::PatchImport("storm.dll", 121, &_SNetReceiveMessage);
  _SNetSendTurnOld        = HackUtil::PatchImport("storm.dll", 128, &_SNetSendTurn);
  _SFileOpenFileOld       = HackUtil::PatchImport("storm.dll", 267, &_SFileOpenFile);
  _SFileOpenFileExOld     = HackUtil::PatchImport("storm.dll", 268, &_SFileOpenFileEx);
  _SMemAllocOld           = HackUtil::PatchImport("storm.dll", 401, &_SMemAlloc);
  _SStrCopyOld            = HackUtil::PatchImport("storm.dll", 501, &_SStrCopy);
  
  // wmode/drawing detours
  _CreateWindowExAOld      = HackUtil::PatchImport("user32.dll", "CreateWindowExA", &_CreateWindowEx);
  
  // Other Detours
  _FindFirstFileAOld     = HackUtil::PatchImport("kernel32.dll", "FindFirstFileA", &_FindFirstFile);
  _CreateThreadOld       = HackUtil::PatchImport("kernel32.dll", "CreateThread", &_CreateThread);
  _CreateEventAOld       = HackUtil::PatchImport("kernel32.dll", "CreateEventA", &_CreateEvent);
  _GetSystemTimeAsFileTimeOld = HackUtil::PatchImport("kernel32.dll", "GetSystemTimeAsFileTime", &_GetSystemTimeAsFileTime);
  _GetCommandLineAOld    = HackUtil::PatchImport("kernel32.dll", "GetCommandLineA", &_GetCommandLineA);
}
