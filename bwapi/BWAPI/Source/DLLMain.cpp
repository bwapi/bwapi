#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <stdio.h>
#include <tlhelp32.h>

#include <Util/FileLogger.h>
#include <Util/Gnu.h>
#include <Util/Foreach.h>

#include "BW/Offsets.h"
#include "BW/Sprite.h"
#include "BW/Image.h"
#include "BWAPI/GameImpl.h"
#include "BWAPI.h"
#include "BWAPI/DLLMain.h"
#include "BWAPI/Shape.h"
#include "BWAPI/UnitImpl.h"

#include "NewHackUtil.h"

DWORD dwProcNum = 0;

//--------------------------------------------- GET PROC COUNT -----------------------------------------------
// Found/modified this from some random help board
DWORD getProcessCount(const char *pszProcName)
{
  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);

  DWORD dwCount = 0;
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if ( Process32First(hSnapshot, &pe32) )
  {
    do
    {
      if( _strcmpi(pe32.szExeFile, pszProcName) == 0 )
        ++dwCount;
    } while( Process32Next(hSnapshot, &pe32) );
  }
  CloseHandle(hSnapshot);
  return dwCount;
}

//----------------------------------------------- ON GAME END ------------------------------------------------
BOOL __stdcall _SCodeDelete(HANDLE *handle)
{
  BWAPI::BroodwarImpl.onGameEnd();
  return SCodeDelete(handle);
}

//--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
int __cdecl nextFrameHook()
{
  BWAPI::BroodwarImpl.update();
  return *BW::BWDATA_NextLogicFrameData;
}

//------------------------------------------------- SEND TEXT ------------------------------------------------
int __stdcall _SStrCopy(char *dest, const char *source, size_t size)
{
  if ( strlen(source) > 0 )
  {
    if ( size == 0x7FFFFFFF && *BW::BWDATA_gwGameMode == 3 )
    {
      if ( dest == BW::BWDATA_SaveGameFile )
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
    else if ( size == 120 )
    {
      /* onSend Lobby */
    }
  }
  return SStrCopy(dest, source, size);
}

//----------------------------------------------- RECEIVE TEXT -----------------------------------------------
BOOL __stdcall _SNetReceiveMessage(int *senderplayerid, u8 **data, int *databytes)
{
  BOOL rval = SNetReceiveMessage(senderplayerid, (char**)data, databytes);
  if ( rval && *databytes > 2 && (*data)[0] == 0)
    BWAPI::BroodwarImpl.onReceiveText(*senderplayerid, std::string((char*)&(*data)[2]) );

  return rval;
}

//---------------------------------------------- DRAW HOOKS --------------------------------------------------
bool wantRefresh = false;
void __stdcall DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  if ( wantRefresh )
  {
    wantRefresh = false;
    memset(BW::BWDATA_RefreshRegions, 1, 1200);
  }

  if ( BW::pOldDrawGameProc )
    BW::pOldDrawGameProc(pSurface, pBounds);

  unsigned int numShapes = BWAPI::BroodwarImpl.shapes.size();
  for( unsigned int i = 0; i < numShapes; ++i )
    BWAPI::BroodwarImpl.shapes[i]->draw();
}

void __stdcall DrawDialogHook(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  if ( BW::pOldDrawDialogProc )
    BW::pOldDrawDialogProc(pSurface, pBounds);

  if ( *BW::BWDATA_gwGameMode == 4 )
    BWAPI::BroodwarImpl.onMenuFrame();

  //click the menu dialog that pops up when you win/lose a game
  if ( BWAPI::BroodwarImpl.autoMenuMode != "" && BWAPI::BroodwarImpl.autoMenuMode != "OFF" )
  {
    BW::dialog *endDialog = BW::FindDialogGlobal("LMission");
    if ( !endDialog )
      endDialog = BW::FindDialogGlobal("WMission");
    if ( endDialog )
      endDialog->findIndex(-2)->activate();
  }
}

void drawBox(int _x, int _y, int _w, int _h, int color, int ctype)
{
  RECT box = { _x, _y, _w, _h };
  switch ( ctype )
  {
  case 2:
    box.left -= *(BW::BWDATA_ScreenX);
    box.top  -= *(BW::BWDATA_ScreenY);
    break;
  case 3:
    box.left += BW::BWDATA_Mouse->x;
    box.top  += BW::BWDATA_Mouse->y;
    break;
  }

  int right  = box.left + box.right;
  int bottom = box.top  + box.bottom;
  if (right    <= 0   ||
      bottom   <= 0   ||
      box.left >= BW::BWDATA_GameScreenBuffer->wid - 1 ||
      box.top  >= BW::BWDATA_GameScreenBuffer->ht  - 1)
    return;
  if ( right > BW::BWDATA_GameScreenBuffer->wid - 1 )
    box.right = (BW::BWDATA_GameScreenBuffer->wid - 1) - box.left;
  if ( bottom > BW::BWDATA_GameScreenBuffer->ht - 1 )
    box.bottom = (BW::BWDATA_GameScreenBuffer->ht - 1) - box.top;
  if ( box.left < 0 )
  { 
    box.right += box.left; 
    box.left  =  0;
  }
  if ( box.top < 0 )
  {
    box.bottom  += box.top;
    box.top     =  0;
  }

  u8 *data = BW::BWDATA_GameScreenBuffer->data;
  if ( data )
  {
    for ( int iy = box.top; iy < box.top + box.bottom; iy++ )
      for ( int ix = box.left; ix < box.left + box.right; ix++ )
        data[iy * BW::BWDATA_GameScreenBuffer->wid + ix] = (u8)color;
  }
  wantRefresh = true;
}

void drawDot(int _x, int _y, int color, int ctype)
{
  POINT pt = { _x, _y };
  switch ( ctype )
  {
  case 2:
    pt.x -= *(BW::BWDATA_ScreenX);
    pt.y -= *(BW::BWDATA_ScreenY);
    break;
  case 3:
    pt.x += BW::BWDATA_Mouse->x;
    pt.y += BW::BWDATA_Mouse->y;
    break;
  }
  if ( pt.x + 1 <= 0 ||
       pt.y + 1 <= 0 ||
       pt.x >= BW::BWDATA_GameScreenBuffer->wid - 2 ||
       pt.y >= BW::BWDATA_GameScreenBuffer->ht - 2)
    return;

  u8 *data = BW::BWDATA_GameScreenBuffer->data;
  if ( data )
    data[pt.y * BW::BWDATA_GameScreenBuffer->wid + pt.x] = (u8)color;
  wantRefresh = true;
}

void drawText(int _x, int _y, const char* ptext, int ctype, char size)
{
  POINT pt = { _x, _y };
  switch ( ctype )
  {
  case 2:
    pt.x -= *(BW::BWDATA_ScreenX);
    pt.y -= *(BW::BWDATA_ScreenY);
    break;
  case 3:
    pt.x += BW::BWDATA_Mouse->x;
    pt.y += BW::BWDATA_Mouse->y;
    break;
  }
  if (pt.x + BW::GetTextWidth(ptext, size)  < 0 || 
      pt.y + BW::GetTextHeight(ptext, size) < 0 || 
      pt.x > BW::BWDATA_GameScreenBuffer->wid   || 
      pt.y > BW::BWDATA_GameScreenBuffer->ht)
    return;

  BW::BlitText(ptext, BW::BWDATA_GameScreenBuffer, pt.x, pt.y, size);
  wantRefresh = true;
}

int   lastHotkey;
DWORD lastHotkeyTime;
//--------------------------------------------- ON ISSUE COMMAND ---------------------------------------------
void __fastcall QueueGameCommand(BYTE *buffer, DWORD length)
{
  // Do our own center view on hotkeys, since BWAPI introduces a bug that destroys this
  if ( length >= 3 && buffer[0] == 0x13 && buffer[1] == 1 ) // Recall Hotkey
  {
    DWORD thisHotkeyTime = GetTickCount();
    if ( lastHotkey == buffer[2] && (thisHotkeyTime - lastHotkeyTime) < 800 )
    {
      // do center view here
      BWAPI::BroodwarImpl.moveToSelected();
      lastHotkeyTime = 0;
      lastHotkey     = -1;
    }
    else
    {
      lastHotkeyTime = thisHotkeyTime;
      lastHotkey     = buffer[2];
    }
  }

  if ( length + *BW::BWDATA_sgdwBytesInCmdQueue <= *BW::BWDATA_MaxTurnSize )
  {
    // Copy data to primary turn buffer
    memcpy(&BW::BWDATA_TurnBuffer[*BW::BWDATA_sgdwBytesInCmdQueue], buffer, length);
    *BW::BWDATA_sgdwBytesInCmdQueue += length;
    return;
  }
  
  // Verify game mode
  if ( *BW::BWDATA_gwGameMode == 4 )
    return;

  int turns;
  if ( SNetGetTurnsInTransit(&turns) ) // Buffer is full
  {
    // This statement will probably never be hit, but just in case
    if ( turns >= 16 - *BW::BWDATA_dword_57F090 )
      return;

    // Send the turn and fill the new buffer
    BW::BWFXN_sendTurn();
    memcpy(BW::BWDATA_TurnBuffer, buffer, length); // Originally an array index, but sgdwBytesInCmdQueue should always be 0 here
    *BW::BWDATA_sgdwBytesInCmdQueue = length;
  }
  // assume no error, would fatal anyway
}

void __fastcall CommandFilter(BYTE *buffer, DWORD length)
{
  /* Filter commands using BWAPI rules */
  if ( BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) || 
       !BWAPI::BroodwarImpl.onStartCalled ||
       buffer[0] <= 0x0B ||
       (buffer[0] >= 0x0F && buffer[0] <= 0x12) ||
       (buffer[0] == 0x13 && buffer[1] == 1)    || // Hotkey (select only)
       (buffer[0] >= 0x37 && buffer[0] <= 0x59) ||
       buffer[0] >= 0x5B )
  {
    
    //reload the unit selection states (so that the user doesn't notice any changes in selected units in the Starcraft GUI.
    if ( (buffer[0] >= 0x0A && buffer[0] <= 0x0C) ||
         (buffer[0] == 0x13 && !(buffer[1] & 1)) ||
         buffer[0] == 0x14 ||
         buffer[0] == 0x15 ||
         (buffer[0] >= 0x18 && buffer[0] <= 0x36) ||
         buffer[0] == 0x5A )
    {
      BWAPI::BroodwarImpl.loadSelected();
    }
    QueueGameCommand(buffer, length);
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
  /* Always return a successful check to bypass our custom SNP module authentication */
  if ( dwReturnVal )
    *dwReturnVal = 5;
  return TRUE;
}

std::string lastFile;
BOOL __stdcall _SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile)
{
  /* Store the name of the last-opened file to retrieve the pointer once it's allocated */
  lastFile = szFileName;
  return SFileOpenFileEx(hMpq, szFileName, dwSearchScope, phFile);
}

void *__stdcall _SMemAlloc(int amount, char *logfilename, int logline, int defaultValue)
{
  /* Call the original function */
  void *rval = SMemAlloc(amount, logfilename, logline, defaultValue);

  /* Save the allocated string table pointer */
  if ( lastFile == "rez\\stat_txt.tbl" )
  {
    BW::BWDATA_StringTableOff = (char*)rval;
    lastFile = "";
  }

  /* Save the allocated fog of war pointer */
  if ( amount == 0x40000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 606 )
    BW::BWDATA_MapFogOfWar = (u32*)rval;

  /* Save the allocated mini-tile flags pointer */
  if ( lastFile.find(".vf4") != std::string::npos )
  {
    BW::BWDATA_MiniTileFlags = (BW::MiniTileMaps_type*)rval;
    lastFile = "";
  }

  /* Save the allocated creep pointer */
  if ( strcmpi(logfilename, "Starcraft\\SWAR\\MapComn\\creep.cpp") == 0 && logline == 420 )
    BW::BWDATA_ZergCreepArray = (u16*)rval;

  /* Save the allocated SAI_Paths pointer */
  if ( strcmpi(logfilename, "Starcraft\\SWAR\\lang\\sai_PathCreate.cpp") == 0 && logline == 210 )
    BW::BWDATA_SAIPathing = (BW::SAI_Paths*)rval;

  /* Save the allocated tileset pointer */
  if ( lastFile.find(".cv5") != std::string::npos )
  {
    BW::BWDATA_TileSet    = (BW::TileType*)rval;
    BW::BWDATA_DoodatSet  = (BW::DoodatType*)rval;
    lastFile = "";
  }

  /* Save the allocated map tile array pointer */
  if ( amount == 0x20000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 603 )
    BW::BWDATA_MapTileArray = (u16*)rval;

  return rval;
}

DWORD lastTurnTime;
DWORD lastTurnFrame;
BOOL __stdcall _SNetSendTurn(char *data, unsigned int databytes)
{
  /* Save tick/frame counts for getRemainingLatency*  */
  lastTurnTime  = GetTickCount();
  lastTurnFrame = BWAPI::BroodwarImpl.getFrameCount();
  return SNetSendTurn(data, databytes);
}

//--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
DWORD WINAPI CTRT_Thread(LPVOID)
{
  delete Util::Logger::globalLog;
  GetPrivateProfileString("paths", "log_path", "", logPath, MAX_PATH, "bwapi-data\\bwapi.ini");
  
  logging = false;
  char logging_str[MAX_PATH];
  GetPrivateProfileString("config", "logging", "", logging_str, MAX_PATH, "bwapi-data\\bwapi.ini");
  if ( std::string( strupr(logging_str) ) == "ON" )
    logging = true;

  if ( logPath[0] != '\0' )
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
  char temptest[12];
  void *pPlayIscript;
  sprintf_s(temptest, 32, "%p", &BW::Image::CImage::_PlayIscript);
  sscanf(temptest, "%p", &pPlayIscript);

  /* Create function-level hooks */
  HackUtil::CallPatch(BW::BWFXN_NextLogicFrame, &nextFrameHook);
  HackUtil::CallPatch(BW::BWFXN_IscriptHook,    pPlayIscript);
  HackUtil::JmpPatch(BW::BWFXN_QueueCommand,    &CommandFilter);
  HackUtil::JmpPatch(HackUtil::GetImport("storm.dll", 251), &_SFileAuthenticateArchive);

  /* Perform code patches */
  char zero = 0;
  HackUtil::WriteNops(BW::BWDATA_MenuLoadHack, 11);            // main menu load timer
  HackUtil::WriteMem(BW::BWDATA_MenuInHack, &zero, 1);         // menu in speed
  HackUtil::WriteMem(BW::BWDATA_MenuOutHack, &zero, 1);        // menu out speed
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack, &zero, 1);    // BNET Server menu in speed
  HackUtil::WriteMem(BW::BWDATA_MultiplayerHack2, &zero, 1);   // BNET Server menu out speed
  HackUtil::WriteMem(BW::BWDATA_OpponentStartHack, &zero, 1);  // Start without an opponent

  /* Create import detours */
  HackUtil::PatchImport("storm.dll", 128, &_SNetSendTurn);
  HackUtil::PatchImport("storm.dll", 121, &_SNetReceiveMessage);
  HackUtil::PatchImport("storm.dll", 332, &_SCodeDelete);
  HackUtil::PatchImport("storm.dll", 268, &_SFileOpenFileEx);
  HackUtil::PatchImport("storm.dll", 401, &_SMemAlloc);
  HackUtil::PatchImport("storm.dll", 501, &_SStrCopy);
  return 0;
}

DWORD WINAPI PersistentPatch(LPVOID)
{
  for ever
  {
    Sleep(250);

    if ( BW::BWDATA_ScreenLayers[2].pUpdate != DrawDialogHook && BW::BWDATA_ScreenLayers[2].pUpdate != NULL )
    {
      BW::pOldDrawDialogProc = BW::BWDATA_ScreenLayers[2].pUpdate;
      BW::BWDATA_ScreenLayers[2].pUpdate = DrawDialogHook;
    }

    if ( BW::BWDATA_ScreenLayers[5].pUpdate != DrawHook && BW::BWDATA_ScreenLayers[5].pUpdate != NULL )
    {
      BW::pOldDrawGameProc = BW::BWDATA_ScreenLayers[5].pUpdate;
      BW::BWDATA_ScreenLayers[5].pUpdate = DrawHook;
    }

  } //loop
}

//------------------------------------------------- DLL MAIN -------------------------------------------------
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
      dwProcNum = getProcessCount("StarCraft_MultiInstance.exe");
      BWAPI::BWAPI_init();
      CTRT_Thread(NULL);
      CreateThread(NULL, 0, &PersistentPatch, NULL, 0, NULL);
      return TRUE;
  }
  return TRUE;
}
