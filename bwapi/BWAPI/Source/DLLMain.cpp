#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>

#include <Util/Dictionary.h> 
#include <Util/FileLogger.h>
#include <Util/Types.h>

#include "BW/Offsets.h"
#include "BWAPI/Globals.h"
#include "BWAPI/Game.h"
#include "BWAPI/Globals.h"
#include "BWAPI/Player.h"

#include "../../BWAI/Source/BWAI/AI.h"
#include "../../BWAI/Source/BWAI/Globals.h"

bool aiStartCalled = false;
DWORD onCancelTrain_edx;
DWORD onCancelTrain_ecx;
DWORD removedUnit;
//bool launchedStart = false;
DWORD eaxSave,ebxSave,ecxSave,edxSave,esiSave,ediSave, espSave, ebpSave;
//--------------------------------------------- ON COMMAND ORDER ---------------------------------------------
void __declspec(naked) onRemoveUnit()
{
  __asm
  {
    mov removedUnit, esi
  }
  __asm
  {
    call [BW::BWXFN_RemoveUnitTarget]
  }
  {
    #pragma warning(push)
    #pragma warning(disable:4312)
    BWAPI::Broodwar.onRemoveUnit((BW::Unit*) removedUnit);
    BWAI::ai->onRemoveUnit((BW::Unit*) removedUnit);
    #pragma warning(pop)
  }
   __asm
  {
    jmp [BW::BWXFN_RemoveUnitBack]
  }
}

//--------------------------------------------- ON COMMAND ORDER ---------------------------------------------
void __declspec(naked) onCancelTrainByEscape()
{
  __asm
  {
    mov onCancelTrain_edx, edx
    mov onCancelTrain_ecx, ecx
  }
  {
    BWAPI::Broodwar.onCancelTrain();
    BWAI::ai->onCancelTrain();
  }
  __asm
  {
    mov edx, onCancelTrain_edx
    mov ecx, onCancelTrain_ecx
    call [BW::BWXFN_CancelTrainByEscapeTarget]
    jmp [BW::BWXFN_CancelTrainByEscapeBack]
  }
}
//--------------------------------------------- ON COMMAND ORDER ---------------------------------------------
void __declspec(naked) onCancelTrainByClickInTheQueue()
{
  __asm
  {
    mov onCancelTrain_edx, edx
    mov onCancelTrain_ecx, ecx
  }
  {
    BWAPI::Broodwar.onCancelTrain();
    BWAI::ai->onCancelTrain();
  }
  __asm
  {
    mov edx, onCancelTrain_edx
    mov ecx, onCancelTrain_ecx
    call [BW::BWXFN_CancelTrainByClickInTheQueueTarget]
    jmp [BW::BWXFN_CancelTrainByClickInTheQueueBack]
  }
}
//---------------------------------------------- ON GAME START -----------------------------------------------
void __declspec(naked) onGameStart()
{
  {
    BWAPI::Broodwar.onGameStart();
  }
  __asm
  {
    call [BW::BWXFN_GameStartTarget]
    jmp [BW::BWXFN_GameStartBack]
  }
}
//----------------------------------------------- ON GAME END ------------------------------------------------
void __declspec(naked) onGameEnd()
{
  {
    aiStartCalled = false;
    //launchedStart = false;
    BWAPI::Broodwar.onGameEnd();
    BWAI::ai->onEnd();
  }
  __asm
  {
    call [BW::BWXFN_GameEndTarget]
    jmp [BW::BWXFN_GameEndBack]
  }
}
DWORD frameHookEax;
//--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
void __declspec(naked)  nextFrameHook()
{
 __asm
  {
    call [BW::BWXFN_NextLogicFrameTarget]
    mov frameHookEax, eax
  }
  {
    BWAPI::Broodwar.update();
    BWAI::ai->update();    
    if (!aiStartCalled)
    {
      if (BWAPI::Broodwar.BWAPIPlayer != NULL && BWAPI::Broodwar.opponent != NULL && !*(BW::BWFXN_InReplay))
      {
        BWAI::ai->onStart(BWAPI::Broodwar.BWAPIPlayer, BWAPI::Broodwar.opponent);
        aiStartCalled = true;
      }
    }
    BWAI::ai->onFrame();
  }
  __asm
  {
    mov eax, frameHookEax
    jmp [BW::BWXFN_NextLogicFrameBack]
  }
}

//---------------------------------------------- SEND TEXT HOOK ----------------------------------------------
char* text;
bool sendToBW;
void __declspec(naked) onSendText()
{
 __asm
  {
    mov eaxSave, eax
    mov ebxSave, ebx
    mov ecxSave, ecx
    mov edxSave, edx
    mov esiSave, esi
    mov ediSave, edi
    mov espSave, esp
    mov ebpSave, ebp
    mov text, edx;
  }
  sendToBW = true;
  sendToBW &= !BWAPI::Broodwar.onSendText(text);
  if (sendToBW)
    sendToBW &= !BWAI::ai->onSendText(text);
  if (sendToBW)
    __asm
    {
      mov eax, eaxSave
      mov ebx, ebxSave
      mov ecx, ecxSave
      mov edx, edxSave
      mov esi, esiSave
      mov edi, ediSave
      mov esp, espSave
      mov ebp, ebpSave
      call [BW::BWXFN_SendPublicCallTarget]
      jmp [BW::BWXFN_SendPublicCallBack]
      }
  __asm
  {
    jmp [BW::BWXFN_SendPublicCallBack]
  }
}
//------------------------------------------------ JMP PATCH -------------------------------------------------
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
void JmpCallPatch(void *pDest, int pSrc, int nNops = 0)
{
  DWORD OldProt = 0;
  VirtualProtect((LPVOID)pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
  unsigned char jmp = 0xE9;
  memcpy((LPVOID)pSrc, &jmp, 1);
  DWORD address = (DWORD)pDest - (DWORD)pSrc - 5;
  memcpy((LPVOID)(pSrc + 1), &address, 4); 
  for (int i = 0; i < nNops; ++i)
    *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
  VirtualProtect((LPVOID)pSrc, 5 + nNops, OldProt, &OldProt);
}
#pragma warning(pop)
//--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
DWORD WINAPI CTRT_Thread( LPVOID lpThreadParameter )
{
  if (config == NULL)
    return 1;

  delete Util::Logger::globalLog;
  Util::Logger::globalLog = new Util::FileLogger(config->get("log_path") + "\\global", Util::LogLevel::MicroDetailed);
  Util::Logger::globalLog->log("BWAPI initialisation started");

  BWAI::ai = new BWAI::AI();
  int sleepTime = atoi(config->get("sleep_before_initialize_hooks").c_str());

  Sleep(sleepTime);
  JmpCallPatch(nextFrameHook, BW::BWXFN_NextLogicFrame, 0);
  JmpCallPatch(onGameStart, BW::BWXFN_GameStart, 0);
  JmpCallPatch(onGameEnd, BW::BWXFN_GameEnd, 0);
  JmpCallPatch(onCancelTrainByClickInTheQueue, BW::BWXFN_CancelTrainByClickInTheQueue, 0);
  JmpCallPatch(onCancelTrainByEscape, BW::BWXFN_CancelTrainByEscape, 0);
  JmpCallPatch(onRemoveUnit, BW::BWXFN_RemoveUnit, 0);
  JmpCallPatch(onSendText, BW::BWXFN_SendPublicCall, 0);

  
 /* for ever
  {
    if (!BWAPI::Broodwar.isInGame())
    {
      BWAPI::Broodwar.changeSlot(BW::Orders::ChangeSlot::Computer, 1);
      BWAPI::Broodwar.changeRace(BW::Orders::ChangeRace::Zerg, 1);
      BWAPI::Broodwar.changeRace(BW::Orders::ChangeRace::Terran, 0); 
      BWAPI::Broodwar.IssueCommand((PBYTE)&BW::Orders::StartGame(),sizeof(BW::Orders::StartGame));
      //launchedStart = true;
    }
    Sleep(6000);
  }*/
  return 0;
}
//------------------------------------------------- DLL MAIN -------------------------------------------------
BOOL APIENTRY DllMain( HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
   switch (ul_reason_for_call)
   {
     case DLL_PROCESS_ATTACH: 
     {
       CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CTRT_Thread, NULL, 0, NULL);
        return true;
     }
     case DLL_THREAD_ATTACH:
     case DLL_THREAD_DETACH:
     case DLL_PROCESS_DETACH:
      break;
   }
   return true;
}