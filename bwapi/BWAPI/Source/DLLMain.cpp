#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <Wincrypt.h>

#include <Util/FileLogger.h>
#include <Util/Gnu.h>

#include "BW/Offsets.h"
#include "BW/Sprite.h"
#include "BW/Image.h"
#include "BWAPI/GameImpl.h"
#include "BWAPI/UnitImpl.h"
#include "BWAPI/PlayerImpl.h"
#include "BWAPI.h"
#include "BWAPI/DLLMain.h"
#include "BWAPI/Shape.h"

#include "NewHackUtil.h"

DWORD eaxSave, ebxSave, ecxSave, edxSave, esiSave, ediSave, espSave, ebpSave;
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

//------------------------------------------------- SEND TEXT ------------------------------------------------
int __stdcall _SStrCopy(char *dest, const char *source, size_t size)
{
  if (strlen(source) > 0)
  {
    if (size == 0x7FFFFFFF && *BW::BWDATA_gwGameMode == 3)
    {
      if ((u32)dest == BW::BWDATA_SaveGameFile)
      {
        /* onSaveGame */
        BWAPI::BroodwarImpl.onSaveGame((char*)source);
      }
      else
      {
        /* onSend Game */
        BWAPI::BroodwarImpl.sentMessages.push_back(std::string(source));
        dest[0] = 0;
        return 0;
      }
    }
    else if (size == 120)
    {
      /* onSend Lobby */
    }
  }
  return BW::SStrCopy(dest, source, size);
}

//----------------------------------------------- RECEIVE TEXT -----------------------------------------------
BOOL __stdcall _SNetReceiveMessage(int *senderplayerid, u8 **data, int *databytes)
{
  BOOL rval = BW::SNetReceiveMessage(senderplayerid, data, databytes);
  if ( rval && *databytes > 2 && (*data)[0] == 0)
    BWAPI::BroodwarImpl.onReceiveText(*senderplayerid, std::string((char*)&(*data)[2]) );

  return rval;
}

//---------------------------------------------- DRAW HOOKS --------------------------------------------------
void drawBox(int _x, int _y, int _w, int _h, int color, int ctype)
{
  int x = _x;
  int y = _y;
  int w = _w;
  int h = _h;
  if (ctype == 2)
  {
    x -= *(BW::BWDATA_ScreenX);
    y -= *(BW::BWDATA_ScreenY);
  }
  else if (ctype == 3)
  {
    x += BW::BWDATA_Mouse->x;
    y += BW::BWDATA_Mouse->y;
  }
  if (x + w <= 0 || y + h <= 0 || x >= 639 || y >= 479)
    return;
  if (x + w > 639) w = 639 - x;
  if (y + h > 479) h = 479 - y;
  if (x < 0) {w += x; x = 0;}
  if (y < 0) {h += y; y = 0;}
  
  if ( BWAPI::BroodwarImpl.canvas )
  {
    u8 *data = BWAPI::BroodwarImpl.canvas->getSourceBuffer()->data;
    if ( data )
    {
      for ( int iy = y; iy < y + h; iy++ )
        for ( int ix = x; ix < x + w; ix++ )
          data[iy*640 + ix] = (u8)color;
    }
  }
}

void drawDot(int _x, int _y, int color, int ctype)
{
  int x = _x;
  int y = _y;
  if (ctype == 2)
  {
    x -= *(BW::BWDATA_ScreenX);
    y -= *(BW::BWDATA_ScreenY);
  }
  else if (ctype == 3)
  {
    x += BW::BWDATA_Mouse->x;
    y += BW::BWDATA_Mouse->y;
  }
  if (x + 1 <= 0 || y + 1 <= 0 || x >= 638 || y >= 478)
    return;

  if ( BWAPI::BroodwarImpl.canvas )
  {
    u8 *data = BWAPI::BroodwarImpl.canvas->getSourceBuffer()->data;
    if ( data )
      data[y*640 + x] = (u8)color;
  }
}

void drawText(int _x, int _y, const char* ptext, int ctype, char size)
{
  if (ctype == 2)
  {
    _x -= *(BW::BWDATA_ScreenX);
    _y -= *(BW::BWDATA_ScreenY);
  }
  else if (ctype == 3)
  {
    _x += BW::BWDATA_Mouse->x;
    _y += BW::BWDATA_Mouse->y;
  }
  if (_x + BW::GetTextWidth(ptext, size) < 0 || _y + BW::GetTextHeight(ptext, size) < 0 || _x > 640 || _y > 400)
    return;

  BW::BlitText(ptext, BWAPI::BroodwarImpl.canvas->getSourceBuffer(), _x, _y, size);
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
//------------------------------------------------ ON ISCRIPT ------------------------------------------------
void __thiscall BW::Image::CImage::_PlayIscript(char *header, int unk1, int unk2)
{
  if (this == this->spriteOwner->mainGraphic)   // ignore unwanted graphics
    BWAPI::BroodwarImpl.iscriptParser(this->spriteOwner, this->anim);

  BW::BWFXN_PlayIscript(this, header, unk1, unk2);
}

//------------------------------------------------ STORM HOOKS -----------------------------------------------
BOOL __stdcall _SFileAuthenticateArchive(HANDLE hArchive, DWORD *dwReturnVal)
{
  if ( dwReturnVal )
    *dwReturnVal = 5;
  return TRUE;
}

std::string lastFile;
BOOL __stdcall _SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile)
{
  lastFile = szFileName;
  return BW::SFileOpenFileEx(hMpq, szFileName, dwSearchScope, phFile);
}

void *__stdcall _SMemAlloc(int amount, char *logfilename, int logline, int defaultValue)
{
  void *rval = BW::SMemAlloc(amount, logfilename, logline, defaultValue);
  if ( lastFile == "rez\\stat_txt.tbl" )
  {
    BW::BWDATA_StringTableOff = (char*)rval;
    lastFile = "";
  }

  if ( amount == 0x40000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 606 )
    BW::BWDATA_MapFogOfWar = (u32*)rval;

  if ( lastFile.find(".vf4") != std::string::npos )
  {
    BW::BWDATA_MiniTileFlags = (BW::MiniTileMaps_type*)rval;
    lastFile = "";
  }

  if ( strcmpi(logfilename, "Starcraft\\SWAR\\MapComn\\creep.cpp") == 0 && logline == 420 )
    BW::BWDATA_ZergCreepArray = (u16*)rval;

  if ( lastFile.find(".cv5") != std::string::npos )
  {
    BW::BWDATA_TileSet    = (BW::TileType*)rval;
    BW::BWDATA_DoodatSet  = (BW::DoodatType*)rval;
    lastFile = "";
  }

  if ( amount == 0x20000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 603 )
    BW::BWDATA_MapTileArray = (u16*)rval;

  return rval;
}
//--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
DWORD WINAPI CTRT_Thread(LPVOID)
{
  delete Util::Logger::globalLog;
  GetPrivateProfileStringA("paths", "log_path", "NULL", logPath, MAX_PATH, "bwapi-data\\bwapi.ini");
  
  logging = false;
  char logging_str[MAX_PATH];
  GetPrivateProfileStringA("config", "logging", "NULL", logging_str, MAX_PATH, "bwapi-data\\bwapi.ini");
  if (std::string(logging_str) == "on" || std::string(logging_str) == "ON")
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

  /* Funny workaround for the lack of type casting */
  char temptest[32];
  void *pPlayIscript;
  sprintf_s(temptest, 32, "%08X", &BW::Image::CImage::_PlayIscript);
  sscanf_s(temptest, "%08X", &pPlayIscript);

  /* Perform code patching */
  HackUtil::CallPatch(BW::BWFXN_NextLogicFrame,  &nextFrameHook);
  HackUtil::CallPatch(BW::BWFXN_NextMenuFrame,   &menuFrameHook);
  HackUtil::CallPatch(BW::BWFXN_IscriptHook,     pPlayIscript);
  HackUtil::JmpPatch(BW::BWFXN_OldIssueCommand,  &onIssueCommand);
  HackUtil::JmpPatch(HackUtil::GetImport("storm.dll", 251), &_SFileAuthenticateArchive);

  char zero = 0;
  HackUtil::WriteNops(BW::BWDATA_MenuLoadHack, 11);            // main menu load timer
  HackUtil::WriteMem(BW::BWDATA_MenuInHack, &zero, 1);         // menu in speed
  HackUtil::WriteMem(BW::BWDATA_MenuOutHack, &zero, 1);        // menu out speed
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack, &zero, 1);    // BNET Server menu in speed
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack2, &zero, 1);   // BNET Server menu out speed
  HackUtil::WriteMem(BW::BWDATA_OpponentStartHack, &zero, 1);  // Start without an opponent

  *(FARPROC*)&BW::SStrCopy = HackUtil::GetImport("storm.dll", 501);
  HackUtil::PatchImport("storm.dll", 501, &_SStrCopy);

  *(FARPROC*)&BW::SCodeDelete = HackUtil::GetImport("storm.dll", 332);
  HackUtil::PatchImport("storm.dll", 332, &_SCodeDelete);

  *(FARPROC*)&BW::SNetReceiveMessage = HackUtil::GetImport("storm.dll", 121);
  HackUtil::PatchImport("storm.dll", 121, &_SNetReceiveMessage);

  *(FARPROC*)&BW::SMemAlloc = HackUtil::GetImport("storm.dll", 401);
  HackUtil::PatchImport("storm.dll", 401, &_SMemAlloc);

  *(FARPROC*)&BW::SFileOpenFileEx = HackUtil::GetImport("storm.dll", 268);
  HackUtil::PatchImport("storm.dll", 268, &_SFileOpenFileEx);
/* 
  // this won't work for Battle.net window
  HWND hSWnd = FindWindow("SWarClass", NULL);
  *(LONG*)&hSWndProc = GetWindowLongA(hSWnd, GWLP_WNDPROC);
  SetWindowLongA(hSWnd, GWLP_WNDPROC, (LONG)&BWAPIWndHook);*/
  return 0;
}
//------------------------------------------------- DLL MAIN -------------------------------------------------
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
      BWAPI::BWAPI_init();
      CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CTRT_Thread, NULL, 0, NULL);
      return true;
      break;
  }
  return true;
}
