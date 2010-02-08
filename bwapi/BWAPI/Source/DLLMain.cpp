#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>

#include <Util/FileLogger.h>
#include <Util/Gnu.h>

#include "BW/Offsets.h"
#include "BWAPI/GameImpl.h"
#include "BWAPI/UnitImpl.h"
#include "BWAPI/PlayerImpl.h"
#include "BWAPI.h"
#include "BWAPI/DLLMain.h"
#include "BWAPI/Shape.h"

#include "NewHackUtil.h"

DWORD eaxSave, ebxSave, ecxSave, edxSave, esiSave, ediSave, espSave, ebpSave;

//--------------------------------------------- ON UNIT DEATH ------------------------------------------------
void __fastcall _clearUnitTarget(BW::Unit *deadUnit)
{
  BWAPI::BroodwarImpl.onUnitDestroy(deadUnit);
  BW::BWFXN_clearUnitTarget(deadUnit);
}

//----------------------------------------------- ON GAME END ------------------------------------------------
BOOL __stdcall _SCodeDelete(HANDLE *handle)
{
  BWAPI::BroodwarImpl.onGameEnd();
  return BW::SCodeDelete(handle);
}

//--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
int __cdecl nextFrameHook()
{
  BWAPI::BroodwarImpl.update();
  return *BW::BWDATA_NextLogicFrameData;
}

//--------------------------------------------- MENU FRAME HOOK ----------------------------------------------
void __stdcall menuFrameHook(int flag)
{
  BWAPI::BroodwarImpl.onMenuFrame();
  BW::BWFXN_videoLoop(flag);
}

//---------------------------------------------- SEND TEXT HOOKS ---------------------------------------------
int __stdcall _SStrCopy(char *dest, const char *source, size_t size)
{
  if (size == 0x7FFFFFFF)
  {
    /* onSend Game */
    BWAPI::BroodwarImpl.interceptedMessages.push_back(std::string(source));
    dest[0] = 0;
    return 0;
  }
  else if (size == 120)
  {
    /* onSend Lobby */
  }
  return BW::SStrCopy(dest, source, size);
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
  for(shape_i = 0; shape_i < BWAPI::BroodwarImpl.shapes.size(); shape_i++)
    BWAPI::BroodwarImpl.shapes[shape_i]->draw();

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

void drawBox(int _x, int _y, int _w, int _h, int color, int ctype)
{
  *BW::BWDATA_DrawColor = (u8)color;
  x = _x;
  y = _y;
  w = _w;
  h = _h;
  if (ctype == 2)
  {
    x -= *(BW::BWDATA_ScreenX);
    y -= *(BW::BWDATA_ScreenY);
  }
  else if (ctype == 3)
  {
    LPPOINT cursorPoint = NULL;
    GetCursorPos(cursorPoint);
    x += cursorPoint->x;
    y += cursorPoint->y;
  }
  if (x + w <= 0 || y + h <= 0 || x >= 639 || y >= 479)
    return;
  if (x + w > 639) w = 639 - x;
  if (y + h > 479) h = 479 - y;
  if (x < 0) {w += x; x = 0;}
  if (y < 0) {h += y; y = 0;}

  BW::BWFXN_DrawBox((s16)x, (s16)y, (u16)w, (u16)h);
}

void drawDot(int _x, int _y, int color, int ctype)
{
  *BW::BWDATA_DrawColor = (u8)color;
  x = _x;
  y = _y;
  w = 1;
  h = 1;
  if (ctype == 2)
  {
    x -= *(BW::BWDATA_ScreenX);
    y -= *(BW::BWDATA_ScreenY);
  }
  else if (ctype == 3)
  {
    LPPOINT cursorPoint = NULL;
    GetCursorPos(cursorPoint);
    x += cursorPoint->x;
    y += cursorPoint->y;
  }
  if (x + 1 <= 0 || y + 1 <= 0 || x >= 638 || y >= 478)
    return;

  BW::BWFXN_DrawBox((s16)x, (s16)y, (u16)w, (u16)h);
}

void drawText(int _x, int _y, const char* ptext, int ctype)
{
  if (ctype == 2)
  {
    _x -= *(BW::BWDATA_ScreenX);
    _y -= *(BW::BWDATA_ScreenY);
  }
  else if (ctype == 3)
  {
    LPPOINT cursorPoint = NULL;
    GetCursorPos(cursorPoint);
    _x += cursorPoint->x;
    _y += cursorPoint->y;
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

  //decide if we should let the command go through
  if ( BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput)
       || !BWAPI::BroodwarImpl.isOnStartCalled()
       //If user input is disabled, only allow the following commands to go through:
       || commandID == 0x00 // Game Chat
       || commandID == 0x05 // Keep Alive
       || commandID == 0x06 // Save Game
       || commandID == 0x07 // Load Game
       || commandID == 0x08 // Restart Game
       || commandID == 0x09 // Select
       || commandID == 0x0A // Shift Select
       || commandID == 0x10 // Pause Game
       || commandID == 0x11 // Resume Game
       || commandID == 0x37 // Game Hash
       || commandID == 0x3C // Start Game
       || commandID == 0x3D // Map Download %
       || commandID == 0x3E // Game Slot Modification
       || commandID == 0x3F // Unknown
       || commandID == 0x40 // Join Game
       || commandID == 0x41 // Race Change
       || commandID == 0x42 // Melee Force Change
       || commandID == 0x43 // UMS   Force Change
       || commandID == 0x44 // Slot Change
       || commandID == 0x45 // Swap Players
       || commandID == 0x48 // Game Init (Random Seed)
       || commandID == 0x49 // Info Request
       || commandID == 0x4A // Force Data Transfer
       || commandID == 0x4B // Force Name Transfer
       || commandID == 0x4C // Lobby Chat
       || commandID == 0x4E // Boot Player
       || commandID == 0x4F // Map Transfer
       || commandID == 0x54 // Mission Briefing Start
       || commandID == 0x55 // Set Latency
       || commandID == 0x56 // Change Replay Speed
       || commandID == 0x57 // Leave Game
       || commandID == 0x58 // Minimap Ping
       || commandID == 0x5B // Make Game Public
       || commandID == 0x5C // Replay Game Chat
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
    __asm retn
  }
  else
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
      retn
    }
  }
}

//--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
DWORD WINAPI CTRT_Thread(LPVOID)
{
  delete Util::Logger::globalLog;
  GetPrivateProfileStringA("paths", "log_path", "NULL", logPath, MAX_PATH, "bwapi-data\\bwapi.ini");
  
  logging = false;
  char logging_str[MAX_PATH];
  GetPrivateProfileStringA("config", "logging", "NULL", logging_str, MAX_PATH, "bwapi-data\\bwapi.ini");
  if (std::string(logging_str) == "on")
    logging = true;

  if (_strcmpi(logPath, "NULL") == 0)
  {
    FILE* f = fopen("bwapi-error.txt", "a+");
    fprintf(f, "Could not find log_path under paths in \"bwapi-data\\bwapi.ini\".\n");
    fclose(f);
  }

  if (logging)
  {
    Util::Logger::globalLog = new Util::FileLogger(std::string(logPath) + "\\global", Util::LogLevel::MicroDetailed);
    Util::Logger::globalLog->log("BWAPI initialisation started");
  }
  else
  {
    Util::Logger::globalLog = new Util::FileLogger(std::string(logPath) + "\\global", Util::LogLevel::DontLog);
  }
  HackUtil::CallPatch(BW::BWFXN_NextLogicFrame,  &nextFrameHook);
  HackUtil::CallPatch(BW::BWFXN_NextMenuFrame,   &menuFrameHook);
  HackUtil::CallPatch(BW::BWFXN_DestroyUnitHook, &_clearUnitTarget);
  HackUtil::JmpPatch(BW::BWFXN_DrawHigh,         &onDrawHigh);
  HackUtil::JmpPatch(BW::BWFXN_Refresh,          &onRefresh);
  HackUtil::JmpPatch(BW::BWFXN_OldIssueCommand,  &onIssueCommand);

  char zero = 0;
  HackUtil::WriteNops(BW::BWDATA_MenuLoadHack, 11);            // menu load
  HackUtil::WriteMem(BW::BWDATA_MenuInHack, &zero, 1);         // menu in
  HackUtil::WriteMem(BW::BWDATA_MenuOutHack, &zero, 1);        // menu out
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack, &zero, 1);    // Battle.net Server Select
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack2, &zero, 1);   // Battle.net Server Select
  HackUtil::WriteMem(BW::BWDATA_OpponentStartHack, &zero, 1);  // Start without an opponent

  *(FARPROC*)&BW::SStrCopy = HackUtil::GetImport("storm.dll", 501);
  HackUtil::PatchImport("storm.dll", 501, &_SStrCopy);

  *(FARPROC*)&BW::SCodeDelete = HackUtil::GetImport("storm.dll", 332);
  HackUtil::PatchImport("storm.dll", 332, &_SCodeDelete);
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
  }
  return true;
}
