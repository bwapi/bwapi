
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>

#include "./Types.h"
#include "./BWAPI/Globals.h"
#include "./BWAPI/Game.h"
#include "./BWAPI/Globals.h"
#include "./BW/Offsets.h"

//----------------------------- ON GAME START ---------------------------------
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
//------------------------------ ON GAME END ----------------------------------
void __declspec(naked) onGameEnd()
{
  {
    BWAPI::Broodwar.onGameEnd();
  }
  __asm
  {
    call [BW::BWXFN_GameEndTarget]
    jmp [BW::BWXFN_GameEndBack]
  }
}
//------------------------------- UPDATE ---------------------------------------
int lastEcx;
void __declspec(naked)  hookTest()
{
  if (lastEcx >= 0)
  {
    BWAPI::Broodwar.update();
    BWAPI::Broodwar.test();
  }
  __asm
  {
    call [BW::BWXFN_NextFrameHelperFunctionTarget]
    mov lastEcx, ecx
    jmp [BW::BWXFN_NextFrameHelperFunctionBack]
  }
}
//----------------------------- JMP PATCH --------------------------------------
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
//------------------------- CTRT THREAD MAIN -----------------------------------
DWORD WINAPI CTRT_Thread( LPVOID lpThreadParameter )
{
  Sleep(2000);
  JmpCallPatch(hookTest, BW::BWXFN_NextFrameHelperFunction, 0);
  JmpCallPatch(onGameStart, BW::BWXFN_GameStart, 0);
  JmpCallPatch(onGameEnd, BW::BWXFN_GameEnd, 0);
  for ever
  {
    if (!BWAPI::Broodwar.isInGame())
    {
      BWAPI::Broodwar.changeSlot(BW::Orders::ChangeSlot::Computer, 1);
      BWAPI::Broodwar.changeRace(BW::Orders::ChangeRace::Zerg, 1);
      BWAPI::Broodwar.changeRace(BW::Orders::ChangeRace::Terran, 0); 
      BWAPI::Broodwar.IssueCommand((PBYTE)&BW::Orders::StartGame(),sizeof(BW::Orders::StartGame));
    }
    Sleep(10000);
  }
  return 0;
}

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