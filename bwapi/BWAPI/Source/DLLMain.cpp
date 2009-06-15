#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>

#include <Util/Dictionary.h> 
#include <Util/FileLogger.h>
#include <Util/Types.h>

#include "BW/Offsets.h"
#include "BWAPI/Globals.h"
#include "BWAPI/GameImpl.h"
#include "BWAPI/UnitImpl.h"

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
    call [BW::BWFXN_RemoveUnitTarget]
  }
  {
    #pragma warning(push)
    #pragma warning(disable:4312)
    BWAPI::BroodwarImpl.onRemoveUnit((BW::Unit*) removedUnit);
    #pragma warning(pop)
  }
   __asm
  {
    jmp [BW::BWFXN_RemoveUnitBack]
  }
}

//----------------------------------------------- ON GAME END ------------------------------------------------
void __declspec(naked) onGameEnd()
{
  {
    //launchedStart = false;
    BWAPI::BroodwarImpl.onGameEnd();
  }
  __asm
  {
    call [BW::BWFXN_GameEndTarget]
    jmp [BW::BWFXN_GameEndBack]
  }
}
DWORD frameHookEax;
//--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
void __declspec(naked)  nextFrameHook()
{
 __asm
  {
    call [BW::BWFXN_NextLogicFrameTarget]
    mov frameHookEax, eax
  }
  {
    BWAPI::BroodwarImpl.update();
  }
  __asm
  {
    mov eax, frameHookEax
    jmp [BW::BWFXN_NextLogicFrameBack]
  }
}

//---------------------------------------------- SEND TEXT HOOKS ---------------------------------------------
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
    mov text, esi
  }
  sendToBW = true;
  sendToBW &= !BWAPI::BroodwarImpl.onSendText(text);
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
      call [BW::BWFXN_SendPublicCallTarget]
      jmp [BW::BWFXN_SendPublicCallBack]
      }
  __asm
  {
    jmp [BW::BWFXN_SendPublicCallBack]
  }
}
void __declspec(naked) onSendLobby()
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
    mov text, edi
  }
  sendToBW = true;
  sendToBW &= !BWAPI::BroodwarImpl.onSendText(text);
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
      call [BW::BWFXN_SendLobbyCallTarget]
      jmp [BW::BWFXN_SendLobbyCallBack]
      }
  __asm
  {
    jmp [BW::BWFXN_SendLobbyCallBack]
  }
}

//---------------------------------------------- DRAW HOOKS --------------------------------------------------
u16 i, h, w, x, y;
//s32 ;
u8 c, l;
/*
void __declspec(naked) drawBoxCall()
{
//  if(x+w < 640 && x > 0 && y+h < 480 && y > 0 && h > 0 && w > 0)
//  {
    *BW::BWDATA_DrawColor = 6;
    __asm
    {
      mov eax, eaxSave
      mov ebx, ebxSave
      mov ecx, ecxSave
      mov edx, edxSave
      mov esi, esiSave
      mov edi, ediSave
      mov esp, espSave
      push 80
      push 80
      push 20
      push 20
      call [BW::BWFXN_DrawBox]
    }
//  }
}
*/
void __declspec(naked) onDrawHigh()
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
  }

  for (i = 0; i < 8; i++)
  {
    c = drawQueueBoxFilled[i].c;
    x = drawQueueBoxFilled[i].x;
    y = drawQueueBoxFilled[i].y;
    w = drawQueueBoxFilled[i].w;
    h = drawQueueBoxFilled[i].h;
    l = drawQueueBoxFilled[i].l;
    if (l == 1)
    {
      *BW::BWDATA_DrawColor = 6;
      __asm
      {
        mov eax, eaxSave
        mov ebx, ebxSave
        mov ecx, ecxSave
        mov edx, edxSave
        mov esi, esiSave
        mov edi, ediSave
        mov esp, espSave
        push 80
        push 80
        push 20
        push 20
        call [BW::BWFXN_DrawBox]
      }
    }
/*    else if (l == 2)
    {
      x -= BWAPI::Broodwar->getScreenX();
      y -= BWAPI::Broodwar->getScreenY();
      drawBoxCall();
    }
    else if (l == 3)
    {
      x += BWAPI::Broodwar->getMouseX();
      y += BWAPI::Broodwar->getMouseY();
      drawBoxCall();
    }*/
  }

  __asm
  {
    mov eax, eaxSave
    mov ebx, ebxSave
    mov ecx, ecxSave
    mov edx, edxSave
    mov esi, esiSave
    mov edi, ediSave
    mov esp, espSave
  }

  __asm
  {
    call [BW::BWFXN_DrawHighTarget]
    jmp [BW::BWFXN_DrawHighBack]
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

  JmpCallPatch(nextFrameHook, BW::BWFXN_NextLogicFrame, 0);
  JmpCallPatch(onGameEnd, BW::BWFXN_GameEnd, 0);
  JmpCallPatch(onRemoveUnit, BW::BWFXN_RemoveUnit, 0);
  JmpCallPatch(onSendText, BW::BWFXN_SendPublicCall, 0);
  JmpCallPatch(onSendLobby, BW::BWFXN_SendLobbyCall, 0);
  JmpCallPatch(onDrawHigh, BW::BWFXN_DrawHigh, 0);

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