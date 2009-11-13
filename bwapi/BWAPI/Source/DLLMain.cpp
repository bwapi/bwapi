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

#include "BW/Offsets.h"
#include "BWAPI/Globals.h"
#include "BWAPI/GameImpl.h"
#include "BWAPI/UnitImpl.h"
#include "BWAPI.h"
#include "BWAPI/DLLMain.h"
#include "BWAPI/Shape.h"

DWORD onCancelTrain_edx;
DWORD onCancelTrain_ecx;
DWORD removedUnit;
//bool launchedStart = false;
DWORD eaxSave, ebxSave, ecxSave, edxSave, esiSave, ediSave, espSave, ebpSave;
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
  if (!BWAPI::BroodwarImpl.isSinglePlayer() && *text!='\0')
  {
    sendToBW &= !BWAPI::BroodwarImpl.onSendText(text);
  }
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
  *text='\0';
  __asm
  {
    jmp [BW::BWFXN_SendPublicCallBack]
  }
}
void __declspec(naked) onSendSingle()
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
    mov text, edx
  }
  sendToBW = true;
  if (BWAPI::BroodwarImpl.isSinglePlayer() && *text!='\0')
  {
    sendToBW &= !BWAPI::BroodwarImpl.onSendText(text);
  }
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
    call [BW::BWFXN_SendTextCallTarget]
    jmp [BW::BWFXN_SendTextCallBack]
  }
  __asm
  {
    jmp [BW::BWFXN_SendTextCallBack]
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
int i, i2, h, w, x, y, c, l;

void __declspec(naked) onRefresh()
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
    push 640
    xor eax, eax
    mov edx, 480
    xor ecx, ecx
    call [BW::BWFXN_RefreshTarget]
    mov eax, eaxSave
    mov ebx, ebxSave
    mov ecx, ecxSave
    mov edx, edxSave
    mov esi, esiSave
    mov edi, ediSave
    mov esp, espSave
    mov ebp, ebpSave
    call [BW::BWFXN_RefreshTarget]
    jmp [BW::BWFXN_RefreshBack]
  }
}
unsigned int shape_i;
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

  if(WAIT_OBJECT_0 == ::WaitForSingleObject(BWAPI::BroodwarImpl.hcachedShapesMutex, INFINITE))
  {
    for(shape_i = 0; shape_i < BWAPI::BroodwarImpl.cachedShapes.size(); shape_i++)
    {
      BWAPI::BroodwarImpl.cachedShapes[shape_i]->draw();
    }
    ::ReleaseMutex(BWAPI::BroodwarImpl.hcachedShapesMutex);
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
    call [BW::BWFXN_DrawHighTarget]
    jmp [BW::BWFXN_DrawHighBack]
  }
}

void drawBox(int _x, int _y, int _w, int _h, int color, BWAPI::CoordinateType::Enum ctype)
{
  *BW::BWDATA_DrawColor = (u8)color;
  x = _x;
  y = _y;
  w = _w;
  h = _h;
  if (ctype == 2)
  {
    x -= BWAPI::BroodwarImpl._getScreenX();
    y -= BWAPI::BroodwarImpl._getScreenY();
  }
  else if (ctype == 3)
  {
    x += BWAPI::BroodwarImpl._getMouseX();
    y += BWAPI::BroodwarImpl._getMouseY();
  }
  if (x + w <= 0 || y + h <= 0 || x >= 639 || y >= 479)
    return;
  if (x + w > 639) w = 639 - x;
  if (y + h > 479) h = 479 - y;
  if (x < 0) {w += x; x = 0;}
  if (y < 0) {h += y; y = 0;}


  __asm
  {
    mov eax, eaxSave
    mov ebx, ebxSave
    mov ecx, ecxSave
    mov edx, edxSave
    push h
    push w
    push y
    push x
    call [BW::BWFXN_DrawBox]
  }
}

void drawDot(int _x, int _y, int color, BWAPI::CoordinateType::Enum ctype)
{
  *BW::BWDATA_DrawColor = (u8)color;
  x = _x;
  y = _y;
  w = 1;
  h = 1;
  if (ctype == 2)
  {
    x -= BWAPI::BroodwarImpl._getScreenX();
    y -= BWAPI::BroodwarImpl._getScreenY();
  }
  else if (ctype == 3)
  {
    x += BWAPI::BroodwarImpl._getMouseX();
    y += BWAPI::BroodwarImpl._getMouseY();
  }
  if (x + 1 <= 0 || y + 1 <= 0 || x >= 638 || y >= 478)
    return;


  __asm
  {
    mov eax, eaxSave
    mov ebx, ebxSave
    mov ecx, ecxSave
    mov edx, edxSave
    push h
    push w
    push y
    push x
    call [BW::BWFXN_DrawBox]
  }
}

void drawText(int _x, int _y, const char* ptext, BWAPI::CoordinateType::Enum ctype)
{
  if (ctype == 2)
  {
    _x -= BWAPI::BroodwarImpl._getScreenX();
    _y -= BWAPI::BroodwarImpl._getScreenY();
  }
  else if (ctype == 3)
  {
    _x += BWAPI::BroodwarImpl._getMouseX();
    _y += BWAPI::BroodwarImpl._getMouseY();
  }
  if (_x<0 || _y<0 || _x>640 || _y>400) return;
  int temp = 0;
  DWORD temp_ptr = (DWORD)&temp;
  *BW::BWDATA_PrintXY_Unknown1 = 0x21;
  *BW::BWDATA_PrintXY_Unknown2 = 0x00D8;
  *BW::BWDATA_PrintXY_Unknown3 = 0x0013;
  *BW::BWDATA_PrintXY_PositionX2 = 640;
  *BW::BWDATA_PrintXY_PositionX = _x;
  *BW::BWDATA_PrintXY_PositionY = _y;
  *BW::BWDATA_PrintXY_Current_Font = *BW::BWDATA_Font10_Handle;
  BW::BWDATA_PrintXY_Font->tFontData = BW::BWDATA_FontData;
  BW::BWDATA_PrintXY_Font->tFontUnknown = 0x00000001;
  BW::BWDATA_PrintXY_Font->x1 = 0x00D8;
  BW::BWDATA_PrintXY_Font->y1 = 0x0000;
  BW::BWDATA_PrintXY_Font->x2 = 0x0280;
  BW::BWDATA_PrintXY_Font->y2 = 0x0000;

  __asm
  {
    mov eax, ptext
    mov ebx, 0x00000000
    mov ecx, 0x0000000D
    mov esi, 0x000000e8
    mov edi, ptext
    push temp_ptr
    call [BW::BWFXN_PrintXY]
  }
}

//------------------------------------------------ JMP PATCH -------------------------------------------------
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
void JmpCallPatch(void* pDest, int pSrc, int nNops = 0)
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
void WriteNops(void* pDest, int nNops)
{
  DWORD OldProt = 0;
  VirtualProtect(pDest, nNops, PAGE_EXECUTE_READWRITE, &OldProt);
  memset(pDest, 0x90, nNops);
  VirtualProtect(pDest, nNops, OldProt, &OldProt);
}
void WriteMem(void* pDest, void* pSource, int nSize)
{
  DWORD OldProt = 0;
  VirtualProtect(pDest, nSize, PAGE_EXECUTE_READWRITE, &OldProt);
  memcpy_s(pDest, nSize, pSource, nSize);
  VirtualProtect(pDest, nSize, OldProt, &OldProt);
}
#pragma warning(pop)
//-------------------------------------------- NEW ISSUE COMMAND ---------------------------------------------
void __declspec(naked) NewIssueCommand()
{
  //execute the part of the function that we overwrote:
  __asm
  {
    push ebp
    mov ebp, esp
    push ecx
    mov eax, dword ptr ds: [0x654AA0]
    //jump to execute the rest of the function
    jmp [BW::BWFXN_NewIssueCommand]
  }
}
//--------------------------------------------- ON ISSUE COMMAND ---------------------------------------------
u32 commandIDptr;
u8 commandID;
void __declspec(naked) onIssueCommand()
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
    mov commandIDptr, ecx;
  }
  commandID = *(u8*)commandIDptr;
  //Util::Logger::globalLog->log("command ID: 0x%x",(int)commandID);

  //decide if we should let the command go through
  if ( BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput)
       || !BWAPI::BroodwarImpl.isOnStartCalled()
       //If user input is disabled, only allow the following commands to go through:
       || commandID == 0x09 // Select
       || commandID == 0x0A // Shift Select
       || commandID == 0x10 // Pause Game
       || commandID == 0x11 // Resume Game
       || commandID == 0x37 // Game Hash
       || commandID == 0x3C // Start Game
       || commandID == 0x3D // Map Download %
       || commandID == 0x3E // Game Slot Modification
       || commandID == 0x3F // Unknown
       || commandID == 0x40 // Unknown
       || commandID == 0x41 // Race Change
       || commandID == 0x44 // Slot Change
       || commandID == 0x54 // Mission Briefing Start
       || commandID == 0x56 // Change Replay Speed
     )
  {
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
    }
    NewIssueCommand();
    __asm
    {
      retn
    }
  }
  else
  {
    //Util::Logger::globalLog->log("blocked command ID: 0x%x",(int)commandID);

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
      retn
    }
  }
}
void __declspec(naked) push0patch()
{
  __asm 
    push 0
}

//--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
DWORD WINAPI CTRT_Thread(LPVOID)
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
  JmpCallPatch(onSendSingle, BW::BWFXN_SendTextCall, 0);
  JmpCallPatch(onSendLobby, BW::BWFXN_SendLobbyCall, 0);
  JmpCallPatch(onDrawHigh, BW::BWFXN_DrawHigh, 0);
  JmpCallPatch(onRefresh, BW::BWFXN_Refresh, 0);
  JmpCallPatch(onIssueCommand, BW::BWFXN_OldIssueCommand, 4);

  WriteNops((void*)BW::BWDATA_MenuLoadHack, 11); // menu load
  WriteMem((void*)BW::BWDATA_MenuInHack, &push0patch, 2); // menu in
  WriteMem((void*)BW::BWDATA_MenuOutHack, &push0patch, 2); // menu out
  return 0;
}
//------------------------------------------------- DLL MAIN -------------------------------------------------
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    {
      BWAPI::BWAPI_init();
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
