#include <windows.h>
#include <string>
#include <math.h>
#include "../Storm/storm.h"

#include "WMode.h"
//#include "Resolution.h"
#include "Holiday/Holiday.h"

#include "Detours.h"
#include "BWAPI/GameImpl.h"
#include "BW/Offsets.h"
#include "DLLMain.h"

#include "../../Debug.h"

bool hideHUD;

#include <list>
struct stormAlloc
{
  void          *location;
  bool          memLeak;
  char          lastFile[MAX_PATH];
};
std::list<stormAlloc> allocations;
std::list<stormAlloc> leaks;
void *savedLoc;
bool firstStarted = false;
char gszScreenshotFormat[4];

bool memtest(const stormAlloc& value) { return (value.location == savedLoc); }

BOOL STORMAPI _SDrawCaptureScreen(const char *pszOutput)
{
  char *ext = strrchr((char*)pszOutput, '.');
  if ( ext && strlen(gszScreenshotFormat) == 3 )
  {
    ++ext;
    strncpy(ext, gszScreenshotFormat, 3);
  }
  return SDrawCaptureScreen(pszOutput);
}

//----------------------------------------------- ON GAME END ------------------------------------------------
BOOL __stdcall _SNetLeaveGame(int type)
{
  //MessageBox(0, "OnGameEnd", "", 0);
  BWAPI::BroodwarImpl.onGameEnd();
  if ( firstStarted )
  {
    // copy our leaks to a new list
    for ( std::list<stormAlloc>::iterator i = allocations.begin(); i != allocations.end(); ++i )
    {
      if ( i->memLeak )
        leaks.push_back(*i);
      else
        i->memLeak = true;
    }
    // Deal with our leaks and remove them from the old list
    for each ( stormAlloc i in leaks )
    {
      if ( !strcmpi(i.lastFile, "dlgs\\protoss.grp") || 
           !strcmpi(i.lastFile, "dlgs\\terran.grp")  ||
           !strcmpi(i.lastFile, "dlgs\\zerg.grp") )
      {
        _SMemFree(i.location, "BW Leak @BWAPI Detours.cpp", __LINE__, 0);
      }
      else
      {
        savedLoc = i.location;
        allocations.remove_if(memtest);
      }
    }
    leaks.clear();
  }
  else
    firstStarted = true;
  return SNetLeaveGame(type);
}

//--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
int __cdecl _nextFrameHook()
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
    else if ( size == 120 && *BW::BWDATA_gwGameMode != 3 )
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

//----------------------------------------------- DRAW HOOK --------------------------------------------------
bool wantRefresh = false;
DWORD dwLastAPMCount;
double botAPM_noSelect;
double botAPM_select;
void __stdcall DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  if ( wantRefresh )
  {
    wantRefresh = false;
    memset(BW::BWDATA_RefreshRegions, 1, 1200);
  }

  //GameUpdate(pSurface, pBounds);
  if ( BW::pOldDrawGameProc )
    BW::pOldDrawGameProc(pSurface, pBounds);

  if ( BW::BWDATA_GameScreenBuffer->data )
  {
    if ( gdwHoliday )
      DrawHoliday();

    if ( !BWAPI::BroodwarImpl.isPaused() )
    {
      DWORD dwThisTickCount = BWAPI::BroodwarImpl.getFrameCount()*42;
      if ( dwThisTickCount > dwLastAPMCount )
      {
        double timeDiff = dwThisTickCount - dwLastAPMCount;
        dwLastAPMCount = dwThisTickCount;
        botAPM_noSelect *= exp((double)(-timeDiff/57000)); // 0.95 * 60000
        botAPM_select *= exp((double)(-timeDiff/57000)); // 0.95 * 60000
      }
    }

    unsigned int numShapes = BWAPI::BroodwarImpl.shapes.size();
    for( unsigned int i = 0; i < numShapes; ++i )
      BWAPI::BroodwarImpl.shapes[i]->draw();
    
    if ( numShapes )
      wantRefresh = true;
  }
}
//------------------------------------------------- MENU HOOK ------------------------------------------------
bool nosound = false;
void __stdcall DrawDialogHook(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  if ( BW::pOldDrawDialogProc && !hideHUD )
    BW::pOldDrawDialogProc(pSurface, pBounds);

  if ( *BW::BWDATA_gwGameMode == 4 )
    BWAPI::BroodwarImpl.onMenuFrame();

  if ( !nosound )
  {
    nosound = true;
    char szNosound[8];
    GetPrivateProfileString("starcraft", "sound", "ON", szNosound, 8, szConfigPath);
    if ( strcmpi(szNosound, "OFF") == 0 )
      BW::BWDATA_DSoundDestroy();
  }

  //click the menu dialog that pops up when you win/lose a game
  BW::dialog *endDialog = BW::FindDialogGlobal("LMission");
  if ( !endDialog )
    endDialog = BW::FindDialogGlobal("WMission");
  if ( endDialog )
    endDialog->findIndex(-2)->activate();
}

//------------------------------------------- AUTH ARCHIVE HOOK ----------------------------------------------
BOOL __stdcall _SFileAuthenticateArchive(HANDLE hArchive, DWORD *dwReturnVal)
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
  /* Store the name of the last-opened file to retrieve the pointer once it's allocated */
  lastFile = szFileName;

  if ( !phFile )
    return FALSE;

  if ( !SFileOpenFileEx(NULL, szFileName, SFILE_FROM_ABSOLUTE | SFILE_FROM_RELATIVE, phFile) || !(*phFile) )
    return SFileOpenFileEx(hMpq, szFileName, dwSearchScope, phFile);
  return TRUE;
}

BOOL __stdcall _SFileOpenFile(const char *filename, HANDLE *phFile)
{
  lastFile = filename;
  if ( !phFile )
    return FALSE;

  if ( !SFileOpenFileEx(NULL, filename, SFILE_FROM_ABSOLUTE | SFILE_FROM_RELATIVE, phFile) || !(*phFile) )
    return SFileOpenFile(filename, phFile);
  return TRUE;
}

BOOL __stdcall _SFileOpenArchive(const char *szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE *phMpq)
{
  if ( BWAPI::BroodwarImpl.autoMenuMapPath.size() > 0 && 
       BWAPI::BroodwarImpl.autoMenuMode != ""         &&
       BWAPI::BroodwarImpl.autoMenuMode != "OFF" )
  {
    const char *scheck = strstr(szMpqName, ".scx");
    if ( !scheck )
    {
      scheck = strstr(szMpqName, ".scm");
      if ( !scheck )
        scheck = strstr(szMpqName, ".rep");
    }
    if ( scheck )
    {
      const char *pszMapPath = BWAPI::BroodwarImpl.autoMenuMapPath.c_str();
      return SFileOpenArchive(pszMapPath, dwPriority, dwFlags, phMpq);
    }
  }
  return SFileOpenArchive(szMpqName, dwPriority, dwFlags, phMpq);
}
//--------------------------------------------- MEM ALLOC HOOK -----------------------------------------------
BOOL __stdcall _SMemFree(void *location, char *logfilename, int logline, char defaultValue)
{
  savedLoc = location;
  allocations.remove_if(memtest);
  return SMemFree(location, logfilename, logline, defaultValue);
}

void *__stdcall _SMemAlloc(int amount, char *logfilename, int logline, char defaultValue)
{
  /* Call the original function */
  void *rval = SMemAlloc(amount, logfilename, logline, defaultValue);

  if ( firstStarted )
  {
    stormAlloc t;
    t.location = rval;
    t.memLeak  = false;
    strncpy(t.lastFile, lastFile.c_str(), MAX_PATH);
    allocations.push_back(t);
  }

  /* Save the allocated string table pointer */
  if ( lastFile == "rez\\stat_txt.tbl" )
  {
    BW::BWDATA_StringTableOff = (char*)rval;
    lastFile = "";
    //MessageBox(0, "BWDATA_StringTableOff", "", 0);
  }

  /* Save the allocated fog of war pointer */
  if ( amount == 0x40000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 606 )
  {
    BW::BWDATA_ActiveTileArray = (BW::activeTile*)rval;
    //MessageBox(0, "BWDATA_ActiveTileArray", "", 0);
  }

  /* Save the allocated mini-tile flags pointer */
  if ( lastFile.find(".vf4") != std::string::npos )
  {
    BW::BWDATA_MiniTileFlags = (BW::MiniTileMaps_type*)rval;
    lastFile = "";
    //MessageBox(0, "BWDATA_MiniTileFlags", "", 0);
  }

  /* Save the allocated SAI_Paths pointer */
  if ( strcmpi(logfilename, "Starcraft\\SWAR\\lang\\sai_PathCreate.cpp") == 0 && logline == 210 )
  {
    BW::BWDATA_SAIPathing = (BW::SAI_Paths*)rval;
    //MessageBox(0, "BWDATA_SAIPathing", "", 0);
  }

  /* Save the allocated tileset pointer */
  if ( lastFile.find(".cv5") != std::string::npos )
  {
    BW::BWDATA_TileSet    = (BW::TileType*)rval;
    lastFile = "";
    //MessageBox(0, "BWDATA_TileSet", "", 0);
  }

  /* Save the allocated map tile array pointer */
  if ( amount == 0x20000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 603 )
  {
    BW::BWDATA_MapTileArray = (u16*)rval;
    //MessageBox(0, "BWDATA_MapTileArray", "", 0);
  }

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
  return SNetSendTurn(data, databytes);
}

//---------------------------------------- USER ISSUE COMMAND HOOK -------------------------------------------
int   lastHotkey;
DWORD lastHotkeyTime;
void __fastcall CommandFilter(BYTE *buffer, DWORD length)
{
  /* Filter commands using BWAPI rules */
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
      BW::Orders::Select sel = BW::Orders::Select(*BW::BWDATA_ClientSelectionCount, BW::BWDATA_ClientSelectionGroup);
      QueueGameCommand((PBYTE)&sel, sel.size);
    } // user select
    QueueGameCommand(buffer, length);
  }
}
