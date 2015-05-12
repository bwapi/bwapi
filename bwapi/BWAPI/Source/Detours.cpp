#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <storm.h>

#include "WMode.h"
#include "DLLMain.h"
#include "Resolution.h"
#include "Thread.h"
#include "Config.h"
#include "NewHackUtil.h"
#include "Detours.h"
#include "Util/Convenience.h"
#include "BWAPI/GameImpl.h"
#include "BWAPI/PlayerImpl.h"

#include <BW/Offsets.h>
#include <BW/TriggerEngine.h>
#include <BW/MenuPosition.h>
#include <BW/Dialog.h>
#include <BW/OrderTypes.h>

#include "../../Debug.h"

std::string gDesiredReplayName;

void *leakUIClassLoc;
void *leakUIGrpLoc;

DECL_OLDFXN(SNetLeaveGame);
DECL_OLDFXN(SStrCopy);
DECL_OLDFXN(SNetReceiveMessage);
DECL_OLDFXN(SFileOpenFileEx);
DECL_OLDFXN(SFileOpenFile);
DECL_OLDFXN(SMemAlloc);
DECL_OLDFXN(SNetSendTurn);
DECL_OLDFXN(SDrawCaptureScreen);
DECL_OLDFXN(FindFirstFileA);
DECL_OLDFXN(DeleteFileA);
DECL_OLDFXN(GetFileAttributesA);
DECL_OLDFXN(CreateFileA);
DECL_OLDFXN(CreateWindowExA);
DECL_OLDFXN(Sleep);
DECL_OLDFXN(CreateThread);
DECL_OLDFXN(CreateEventA);
DECL_OLDFXN(GetSystemTimeAsFileTime);

//------------------------------------------------ RANDOM RACE --------------------------------------------------
u8 savedRace[BW::PLAYABLE_PLAYER_COUNT];
int mappedIndex[BW::PLAYABLE_PLAYER_COUNT];
void _RandomizePlayerRaces()    // before
{
  // iterate each player
  for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
  {
    // Save the player's initial race
    savedRace[i] = BW::BWDATA::Players[i].nRace;

    // Give computer players a unique storm id
    if ( BW::BWDATA::Players[i].dwStormId == -1 )
      BW::BWDATA::Players[i].dwStormId -= i;

    // Save the ID so that we can map the saved race later
    mappedIndex[i] = BW::BWDATA::Players[i].dwStormId;
  }

  // Call original fxn
  BW::BWFXN_RandomizePlayerRaces();
}

int getMappedIndex(int stormID)
{
  for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
  {
    if ( mappedIndex[i] == stormID )
      return i;
  }
  return -1;
}

void _InitializePlayerConsole()   // after
{
  for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
  {
    // Retrieve the original race value before randomization occurred from the mapped index
    int mapID = getMappedIndex(BW::BWDATA::Players[i].dwStormId);
    BWAPI::BroodwarImpl.lastKnownRaceBeforeStart[i] = (mapID == -1) ? BWAPI::Races::None : BWAPI::Race( savedRace[mapID] );

    // Reset the computer player's storm ID
    if ( BW::BWDATA::Players[i].dwStormId < 0 )
      BW::BWDATA::Players[i].dwStormId = -1;
  }

  // Call original fxn
  BW::BWFXN_InitializePlayerConsole();
}

//------------------------------------------------ TRIGGERS --------------------------------------------------
void __stdcall ExecuteGameTriggers(DWORD dwMillisecondsPerFrame)
{
  dwMillisecondsPerFrame = BW::OriginalSpeedModifiers[BW::BWDATA::GameSpeed];
  BW::BWFXN_ExecuteGameTriggers(dwMillisecondsPerFrame);
}

//--------------------------------------- GetSystemTimeAsFileTime --------------------------------------------
void WINAPI _GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
  auto replacementSeed = BWAPI::BroodwarImpl.seedOverride;
  if (lpSystemTimeAsFileTime != nullptr && replacementSeed != std::numeric_limits<decltype(replacementSeed)>::max())
  {
    // https://support.microsoft.com/kb/167296
    auto ll = Int32x32To64(replacementSeed, 10000000) + 116444736000000000;
    lpSystemTimeAsFileTime->dwLowDateTime = (DWORD)ll;
    lpSystemTimeAsFileTime->dwHighDateTime = ll >> 32;
    return;
  }
  auto GetSystemTimeAsFileTimeProc = _GetSystemTimeAsFileTimeOld ? _GetSystemTimeAsFileTimeOld : &GetSystemTimeAsFileTime;
  GetSystemTimeAsFileTimeProc(lpSystemTimeAsFileTime);
}

//--------------------------------------------- CREATE EVENT -------------------------------------------------
HANDLE WINAPI _CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName)
{
  if ( lpName != nullptr && std::string(lpName) == "Starcraft Check For Other Instances" )
  {
    SetLastError(ERROR_SUCCESS);
    return 0;
  }
  auto CreateEventProc = _CreateEventAOld ? _CreateEventAOld : &CreateEventA;
  return CreateEventProc(lpEventAttributes, bManualReset, bInitialState, lpName);
}

//--------------------------------------------- CREATE THREAD ------------------------------------------------
HANDLE WINAPI _CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize,LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
  auto CreateThreadProc = _CreateThreadOld ? _CreateThreadOld : &CreateThread;

  DWORD dwThreadId = 0;   // Local thread ID for thread labelling
  HANDLE rval = CreateThreadProc(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, &dwThreadId);

  // Register the created thread
  if ( rval != nullptr )
    RegisterThreadName("Starcraft Broodwar", dwThreadId );

  // Perform the expected behaviour if lpThreadId was provided
  if ( lpThreadId )
    *lpThreadId = dwThreadId;

  return rval;
}

//------------------------------------------------ SLEEP ----------------------------------------------------
VOID WINAPI _Sleep(DWORD dwMilliseconds)
{
  if ( dwMilliseconds == 1500 ) // Main menu timer
    return;

  auto SleepProc = _SleepOld ? _SleepOld : &Sleep;
  SleepProc(dwMilliseconds);
}

//------------------------------------------- DIRECT DRAW INIT -----------------------------------------------
void DDInit()
{
  DDrawInitialize(640, 480);
}
//--------------------------------------------- CREATE WINDOW ------------------------------------------------
bool detourCreateWindow = false;
HWND WINAPI _CreateWindowEx(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
  auto CreateWindowExProc = _CreateWindowExAOld ? _CreateWindowExAOld : &CreateWindowExA;

  HWND hWndReturn = NULL;
  if ( strcmp(lpClassName, "SWarClass") == 0 )
  {
    std::stringstream newWindowName;
    newWindowName << lpWindowName;
    if (gdwProcNum > 1)
      newWindowName << " Instance " << gdwProcNum;

    detourCreateWindow = true;
    if ( switchToWMode )
    {
#ifndef SHADOW_BROODWAR
      HackUtil::CallPatch(BW::BWDATA::DDrawInitCallPatch, &DDInit);
#endif
      hWndReturn = CreateWindowExProc(dwExStyle,
                                        lpClassName,
                                        newWindowName.str().c_str(),
                                        dwStyle | WS_OVERLAPPEDWINDOW,
                                        windowRect.left,
                                        windowRect.top,
                                        windowRect.right,
                                        windowRect.bottom,
                                        hWndParent,
                                        hMenu,
                                        hInstance,
                                        lpParam);
      ghMainWnd = hWndReturn;
      SetWMode(windowRect.right, windowRect.bottom, true);
    }
    else
    {
      hWndReturn = CreateWindowExProc(dwExStyle, lpClassName, newWindowName.str().c_str(), dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
      ghMainWnd = hWndReturn;
    }
    switchToWMode = false;
    // Obtain/hack WndProc
    wOriginalProc = (WNDPROC)GetWindowLong(hWndReturn, GWLP_WNDPROC);
    SetWindowLong(ghMainWnd, GWLP_WNDPROC, (LONG)&WindowProc);
  }
  else
  {
    hWndReturn = CreateWindowExProc(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
  }
  return hWndReturn;
}

//----------------------------------------------- FILE HOOKS -------------------------------------------------
HANDLE WINAPI _FindFirstFile(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
  lpFileName = BWAPI::BroodwarImpl.autoMenuManager.interceptFindFirstFile(lpFileName);

  auto FindFirstFileProc = _FindFirstFileAOld ? _FindFirstFileAOld : &FindFirstFileA;
  return FindFirstFileProc(lpFileName, lpFindFileData);
}
std::string &getReplayName(std::string &sInFilename)
{
  // If it's an automatic replay save
  if ( sInFilename.find("LastReplay.rep") != std::string::npos )
  {
    // If we're replacing the name
    if ( !gDesiredReplayName.empty() )
      sInFilename = gDesiredReplayName;

    // If we have multiple instances, so no write conflicts
    if (gdwProcNum > 1)
    {
      // Add the instance number before .rep
      std::stringstream ss;
      ss << sInFilename.substr(0, sInFilename.find(".rep") );
      ss << '[' << gdwProcNum << ']' << ".rep";
      sInFilename = ss.str();
    }
  }
  return sInFilename;
}

BOOL WINAPI _DeleteFile(LPCSTR lpFileName)
{
  std::string fileName(lpFileName);

  // call the original function
  auto DeleteFileProc = _DeleteFileAOld ? _DeleteFileAOld : &DeleteFileA;
  return DeleteFileProc( getReplayName(fileName).c_str() );
}
DWORD WINAPI _GetFileAttributes(LPCSTR lpFileName)
{
  std::string fileName(lpFileName);

  // call the original function
  auto GetFileAttributesProc = _GetFileAttributesAOld ? _GetFileAttributesAOld : GetFileAttributesA;
  return GetFileAttributesProc( getReplayName(fileName).c_str() );
}
HANDLE WINAPI _CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  std::string fileName(lpFileName);
  // @TODO: Check for read/write attributes

  // call the original function
  auto CreateFileProc = _CreateFileAOld ? _CreateFileAOld : &CreateFileA;
  return CreateFileProc( getReplayName(fileName).c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}
//--------------------------------------------- CAPTURE SCREEN -----------------------------------------------
BOOL STORMAPI _SDrawCaptureScreen(const char *pszOutput)
{
  if ( !pszOutput )
    return FALSE;

  std::string newScreenFilename(pszOutput);

  // Change screenshot extension
  if ( !screenshotFmt.empty() ) // If an extension replacement was specified
  {
    size_t tmp = newScreenFilename.find_last_of("./\\");
    if ( tmp != std::string::npos && newScreenFilename[tmp] == '.' )  // If extension is found
      newScreenFilename.replace(tmp, std::string::npos, screenshotFmt);
    else
      newScreenFilename.append(screenshotFmt);
  }

  // Save the screenshot in w-mode
  if ( wmode && pBits && isCorrectVersion )
  {
    // Create compatible palette
    PALETTEENTRY pal[256];
    for ( int i = 0; i < 256; ++i )
    {
      pal[i].peRed    = wmodebmp.bmiColors[i].rgbRed;
      pal[i].peGreen  = wmodebmp.bmiColors[i].rgbGreen;
      pal[i].peBlue   = wmodebmp.bmiColors[i].rgbBlue;
      pal[i].peFlags  = 0;
    }
    return SBmpSaveImage(newScreenFilename.c_str(), pal, pBits, BW::BWDATA::GameScreenBuffer.width(), BW::BWDATA::GameScreenBuffer.height());
  }
  // Call the old fxn
  auto SDrawCaptureScreenProc = _SDrawCaptureScreenOld ? _SDrawCaptureScreenOld : &SDrawCaptureScreen;
  return SDrawCaptureScreenProc(newScreenFilename.c_str());
}

//----------------------------------------------- ON GAME END ------------------------------------------------
BOOL __stdcall _SNetLeaveGame(int type)
{
  BWAPI::BroodwarImpl.onGameEnd();
  auto SNetLeaveGameProc = _SNetLeaveGameOld ? _SNetLeaveGameOld : &SNetLeaveGame;
  return SNetLeaveGameProc(type);
}

//--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
// Broodwar isGamePaused()
int __cdecl _nextFrameHook()
{
  BWAPI::BroodwarImpl.update();
  return BW::BWDATA::isGamePaused;
}

//------------------------------------------------- SEND TEXT ------------------------------------------------
int __stdcall _SStrCopy(char *dest, const char *source, int size)
{
  if ( source[0] && isCorrectVersion )
  {
    if ( size == 0x7FFFFFFF && BW::BWDATA::gwGameMode == BW::GAME_RUN )
    {
      if ( dest == BW::BWDATA::SaveGameFile.data() )
      {
        // onSaveGame
        BWAPI::BroodwarImpl.onSaveGame(source);
      }
      else
      {
        // onSend Game
        BWAPI::BroodwarImpl.queueSentMessage(source);
        if (size > 0) dest[0] = '\0';
        if (size > 1) dest[1] = '\0';
        return 0;
      }
    }
    else if ( size == 120 && BW::BWDATA::gwGameMode != BW::GAME_RUN )
    {
      // onSend Lobby
    }
  }
  auto SStrCopyProc = _SStrCopyOld ? _SStrCopyOld : &SStrCopy;
  return SStrCopyProc(dest, source, size);
}

//----------------------------------------------- RECEIVE TEXT -----------------------------------------------
BOOL __stdcall _SNetReceiveMessage(int *senderplayerid, char **data, int *databytes)
{
  auto SNetReceiveMessageProc = _SNetReceiveMessageOld ? _SNetReceiveMessageOld : &SNetReceiveMessage;
  BOOL rval = SNetReceiveMessageProc(senderplayerid, data, databytes);

  if ( rval && *databytes > 2 && (*data)[0] == 0)
    BWAPI::BroodwarImpl.onReceiveText(*senderplayerid, std::string((char*)&(*data)[2]) );

  return rval;
}

//----------------------------------------------- DRAW HOOK --------------------------------------------------
void __stdcall DrawHook(BW::Bitmap *pSurface, BW::bounds *pBounds)
{
  static bool wantRefresh = false;
  if (wantRefresh)
  {
    wantRefresh = false;
    // Calling this function forces a game layer refresh without adding any new offsets
    // This is done to improve compatibility with Hellinsect's resolution expander hack
    BW::BWFXN_UpdateScreenPosition();
  }

  //GameUpdate(pSurface, pBounds);
  if ( BW::pOldDrawGameProc )
    BW::pOldDrawGameProc(pSurface, pBounds);

  if ( BW::BWDATA::GameScreenBuffer.isValid() )
  {
    //if ( gdwHoliday )
      //DrawHoliday();

    if (BWAPI::BroodwarImpl.drawShapes())
    {
      wantRefresh = true;
    }
  }
}
//------------------------------------------------- MENU HOOK ------------------------------------------------
bool nosound = false;
void __stdcall DrawDialogHook(BW::Bitmap *pSurface, BW::bounds *pBounds)
{
  if ( BW::pOldDrawDialogProc )
    BW::pOldDrawDialogProc(pSurface, pBounds);

  if ( BW::BWDATA::gwGameMode == BW::GAME_GLUES )
    BWAPI::BroodwarImpl.onMenuFrame();

  BW::dialog *timeout = BW::FindDialogGlobal("TimeOut");
  if ( timeout )
  {
    BW::dialog *dropbtn = timeout->findIndex(2);
    if ( !dropbtn->isDisabled() && BWAPI::BroodwarImpl.wantDropPlayers )
      BWAPI::BroodwarImpl.dropPlayers();
  }

  // NOSOUND config option
  if ( !nosound )
  {
    nosound = true;
    if ( LoadConfigStringUCase("starcraft", "sound", "ON") == "OFF" )
      BW::BWFXN_DSoundDestroy();
  }

  // WMODE config option
  if ( switchToWMode && ghMainWnd )
  {
    switchToWMode = false;
    SetWMode(windowRect.right, windowRect.bottom, true);
  }

  //click the menu dialog that pops up when you win/lose a game
  BW::dialog *endDialog = BW::FindDialogGlobal("LMission");
  if ( !endDialog )
    endDialog = BW::FindDialogGlobal("WMission");
  if ( endDialog )
    endDialog->findIndex(-2)->activate();
}

//------------------------------------------- AUTH ARCHIVE HOOK ----------------------------------------------
BOOL __stdcall _SFileAuthenticateArchive(HANDLE /*hArchive*/, DWORD *dwReturnVal)
{
  /* Always return a successful check to bypass our custom SNP module authentication */
  if ( dwReturnVal )
    *dwReturnVal = 5;
  return TRUE;
}

//--------------------------------------------- OPEN FILE HOOK -----------------------------------------------
std::string lastFile;
BOOL __stdcall _SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile)
{
  // Store the name of the last-opened file to retrieve the pointer once it's allocated
  lastFile = szFileName;

  if ( !phFile )
    return FALSE;

  if ( !SFileOpenFileEx(nullptr, szFileName, SFILE_FROM_ABSOLUTE | SFILE_FROM_RELATIVE, phFile) || !(*phFile) )
  {
    auto SFileOpenFileExProc = _SFileOpenFileExOld ? _SFileOpenFileExOld : &SFileOpenFileEx;
    return SFileOpenFileExProc(hMpq, szFileName, dwSearchScope, phFile);
  }
  return TRUE;
}

BOOL __stdcall _SFileOpenFile(const char *filename, HANDLE *phFile)
{
  lastFile = filename;
  if ( !phFile )
    return FALSE;

  if ( !SFileOpenFileEx(nullptr, filename, SFILE_FROM_ABSOLUTE | SFILE_FROM_RELATIVE, phFile) || !(*phFile) )
  {
    auto SFileOpenFileProc = _SFileOpenFileOld ? _SFileOpenFileOld : &SFileOpenFile;
    return SFileOpenFileProc(filename, phFile);
  }
  return TRUE;
}

//--------------------------------------------- MEM ALLOC HOOK -----------------------------------------------
void *__stdcall _SMemAlloc(size_t amount, char *logfilename, int logline, char defaultValue)
{
  // Call the original function
  auto SMemAllocProc = _SMemAllocOld ? _SMemAllocOld : &SMemAlloc;
  void *rval = SMemAllocProc(amount, logfilename, logline, defaultValue);

#ifndef SHADOW_BROODWAR
  if ( isCorrectVersion )
  {
    if ( lastFile == "dlgs\\protoss.grp" ||
         lastFile == "dlgs\\terran.grp"  ||
         lastFile == "dlgs\\zerg.grp" )
    {
      if ( _strcmpi(logfilename, ".?AU_DLGGRP@@") == 0 )
      {
        if ( leakUIClassLoc )
          SMFree(leakUIClassLoc);
        leakUIClassLoc = rval;
        BW::BWDATA::customList_UIDlgData[0] = &BW::BWDATA::customList_UIDlgData;  // list with custom allocator?
        BW::BWDATA::customList_UIDlgData[1] = (void*)~(u32)&BW::BWDATA::customList_UIDlgData;
      }
      else if ( _strcmpi(logfilename, "Starcraft\\SWAR\\lang\\game.cpp") == 0 )
      {
        if ( leakUIGrpLoc )
          SMFree(leakUIGrpLoc);
        leakUIGrpLoc = rval;
        BW::BWDATA::customList_UIDlgData[0] = &BW::BWDATA::customList_UIDlgData;  // list with custom allocator?
        BW::BWDATA::customList_UIDlgData[1] = (void*)~(u32)&BW::BWDATA::customList_UIDlgData;
      }
    }
  } // isCorrectVer
#endif
  return rval;
}

//--------------------------------------------- SEND TURN HOOK -----------------------------------------------
DWORD lastTurnTime;
DWORD lastTurnFrame;
BOOL __stdcall _SNetSendTurn(char *data, unsigned int databytes)
{
  /* Save tick/frame counts for getRemainingLatency*  */
  lastTurnTime  = GetTickCount();
  lastTurnFrame = BWAPI::BroodwarImpl.getFrameCount();
  auto SNetSendTurnProc = _SNetSendTurnOld ? _SNetSendTurnOld : &SNetSendTurn;
  return SNetSendTurnProc(data, databytes);
}

//---------------------------------------- USER ISSUE COMMAND HOOK -------------------------------------------
int   lastHotkey;
DWORD lastHotkeyTime;
void __fastcall CommandFilter(BYTE *buffer, DWORD length)
{
  if ( !buffer || !length )
    return;

  // Filter commands using BWAPI rules
  if ( BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) ||
       !BWAPI::BroodwarImpl.onStartCalled ||
       buffer[0] <= 0x0B ||
       (buffer[0] >= 0x0F && buffer[0] <= 0x12) ||
       (length >= 3 && buffer[0] == 0x13 && buffer[1] == 1)    || // Hotkey (select only)
       (buffer[0] >= 0x37 && buffer[0] <= 0x59) ||
       buffer[0] >= 0x5B )
  {
    // Custom select code
    if ( buffer[0] == 0x09 ||
         buffer[0] == 0x0A ||
         buffer[0] == 0x0B ||
         (length >= 3 && buffer[0] == 0x13 && buffer[1] == 1) ) // Select Units
    {
      // Do our own center view on hotkeys, since BWAPI introduces a bug that destroys this
      if ( length >= 3 && buffer[0] == 0x13 && buffer[1] == 1 ) // Recall Hotkey
      {
        DWORD thisHotkeyTime = GetTickCount();
        if ( lastHotkey == buffer[2] && (thisHotkeyTime - lastHotkeyTime) < 800 )
        {
          // do center view here
          BWAPI::BroodwarImpl.moveToSelectedUnits();
          lastHotkeyTime = 0;
          lastHotkey     = -1;
        }
        else
        {
          lastHotkeyTime = thisHotkeyTime;
          lastHotkey     = buffer[2];
        }
      }
      BWAPI::BroodwarImpl.wantSelectionUpdate = true;
      return;
    } // selections

    if ( buffer[0] == 0x0C ||
         (buffer[0] == 0x13 && !(buffer[1] & 1)) ||
         buffer[0] == 0x14 ||
         buffer[0] == 0x15 ||
         (buffer[0] >= 0x18 && buffer[0] <= 0x36) ||
         buffer[0] == 0x5A )
    {
      //reload the unit selection states (so that the user doesn't notice any changes in selected units in the Starcraft GUI.
      BW::Orders::Select sel(BW::BWDATA::ClientSelectionCount, const_cast<const BW::CUnit**>(BW::BWDATA::ClientSelectionGroup.data()));
      QueueGameCommand(&sel, sel.size());
    } // user select
    QueueGameCommand(buffer, length);
  }
}
