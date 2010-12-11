#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

#include <Util/FileLogger.h>
#include <Util/Gnu.h>
#include <Util/Foreach.h>

#include "../../svnrev.h"

#include "BW/Offsets.h"
#include "BWAPI/GameImpl.h"
#include "DLLMain.h"
#include "BWAPI/Shape.h"

#include "Detours.h"
#include "WMode.h"
#include "Holiday/Holiday.h"
#include "CodePatch.h"

char szConfigPath[MAX_PATH];
char szInstallPath[MAX_PATH];

DWORD gdwProcNum = 0;

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
      if( SStrCmpI(pe32.szExeFile, pszProcName, MAX_PATH) == 0 )
        ++dwCount;
    } while( Process32Next(hSnapshot, &pe32) );
  }
  CloseHandle(hSnapshot);
  return dwCount;
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
  for ( int iy = box.top; iy < box.top + box.bottom; iy++ )
    for ( int ix = box.left; ix < box.left + box.right; ix++ )
      data[iy * BW::BWDATA_GameScreenBuffer->wid + ix] = (u8)color;
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
  data[pt.y * BW::BWDATA_GameScreenBuffer->wid + pt.x] = (u8)color;
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
}

//---------------------------------------------- QUEUE COMMAND -----------------------------------------------
void __fastcall QueueGameCommand(BYTE *buffer, DWORD length)
{
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
    if ( turns >= 16 - *BW::BWDATA_LatencyCalls )
      return;

    // Send the turn and fill the new buffer
    BW::BWFXN_sendTurn();
    memcpy(&BW::BWDATA_TurnBuffer[*BW::BWDATA_sgdwBytesInCmdQueue], buffer, length);
    *BW::BWDATA_sgdwBytesInCmdQueue += length;
  }
  // assume no error, would fatal anyway
}

//------------------------------------------------ BWAPI ERROR -----------------------------------------------
void BWAPIError(const char *format, ...)
{
  char *buffer;
  vstretchyprintf(buffer, format);

  BWAPI::BroodwarImpl.printf( "\x06" "ERROR: %s", buffer);

  char path[MAX_PATH];
  SStrCopy(path, logPath, MAX_PATH);
  SStrNCat(path, "\\bwapi-error.txt", MAX_PATH);

  SYSTEMTIME time;
  GetSystemTime(&time);
  FILE* f = fopen(path, "a+");
  if ( f )
  {
    fprintf(f, "[%u/%02u/%02u - %02u:%02u:%02u] %s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, buffer);
    fclose(f);
  }
  delete buffer;
}

void BWAPIError(DWORD dwErrCode, const char *format, ...)
{
  char *buffer;
  vstretchyprintf(buffer, format);

  char szErrString[256];
  SErrGetErrorStr(dwErrCode, szErrString, 256);
  BWAPIError("%s    %s", szErrString, buffer);
  delete buffer;
}

char logPath[MAX_PATH];
bool logging;
//--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
DWORD WINAPI CTRT_Thread(LPVOID)
{
  /* Initialize logging options */
  delete Util::Logger::globalLog;
  GetPrivateProfileString("paths", "log_path", "bwapi-data\\logs", logPath, MAX_PATH, szConfigPath);
  
  logging = false;
  char logging_str[MAX_PATH];
  GetPrivateProfileString("config", "logging", "OFF", logging_str, MAX_PATH, szConfigPath);
  if ( std::string( strupr(logging_str) ) == "ON" )
    logging = true;

  /* Shift the position of w-mode */
  if ( gdwProcNum > 0 )
  {
    char szWModeConfig[MAX_PATH];
    sprintf(szWModeConfig, "%s\\wmode.ini", szInstallPath);

    FILE *test = fopen(szWModeConfig, "r");
    if ( test )
    {
      // Get window location and screen dimensions
      int wx = GetPrivateProfileInt("W-MODE", "WindowClientX", 0, szWModeConfig);
      int wy = GetPrivateProfileInt("W-MODE", "WindowClientY", 0, szWModeConfig);
      int cx = GetSystemMetrics(SM_CXSCREEN);
      int cy = GetSystemMetrics(SM_CYSCREEN);

      // Shift window location
      wx += 40;
      wy += 40;
      if ( wx + 640 >= cx )
        wx -= cx - 640;
      if ( wy + 480 >= cy )
        wy -= cy - 480;

      if ( wx < 0 )
        wx = 0;
      if ( wy < 0 )
        wy = 0;

      // Write new window location
      char szScrOutput[16];
      sprintf(szScrOutput, "%u", wx);
      WritePrivateProfileString("W-MODE", "WindowClientX", szScrOutput, szWModeConfig);
      sprintf(szScrOutput, "%u", wy);
      WritePrivateProfileString("W-MODE", "WindowClientY", szScrOutput, szWModeConfig);
    } // file exists
  } // is multi-instance

  /* create log handles */
  if (logging)
  {
    BWAPI::BroodwarImpl.commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::MicroDetailed);
    BWAPI::BroodwarImpl.newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::MicroDetailed);
  }
  else
  {
    BWAPI::BroodwarImpl.commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::DontLog);
    BWAPI::BroodwarImpl.newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::DontLog);
  }
  BWAPI::BroodwarImpl.loadAutoMenuData();

  if (logging)
  {
    Util::Logger::globalLog = new Util::FileLogger(std::string(logPath) + "\\global", Util::LogLevel::MicroDetailed);
    Util::Logger::globalLog->log("BWAPI initialisation started");
  }
  else
  {
    Util::Logger::globalLog = new Util::FileLogger(std::string(logPath) + "\\global", Util::LogLevel::DontLog);
  }
  ApplyCodePatches();
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

    if ( !ghMainWnd )
      ghMainWnd = SDrawGetFrameWindow();

    if ( ghMainWnd )
    {
      WNDPROC thisProc = (WNDPROC)GetWindowLong(ghMainWnd, GWL_WNDPROC);
      if ( thisProc != &WindowProc )
      {
        wOriginalProc = thisProc;
        SetWindowLong(ghMainWnd, GWL_WNDPROC, (LONG)&WindowProc);
      }
    }

  } //loop
}

//------------------------------------------------- DLL MAIN -------------------------------------------------
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
      {
        /* Retrieve the Starcraft path */
        if ( SRegLoadString("Starcraft", "InstallPath", SREG_LOCAL_MACHINE, szInstallPath, MAX_PATH) )
          SStrNCat(szInstallPath, "\\", MAX_PATH);

        /* Create the config path */
        SStrCopy(szConfigPath, szInstallPath, MAX_PATH);
        SStrNCat(szConfigPath, "bwapi-data\\bwapi.ini", MAX_PATH);

        /* Get process count */
        gdwProcNum = getProcessCount("StarCraft_MultiInstance.exe");
        gdwHoliday = 0;

        /* Get revision/build automatically */
        char szDllPath[MAX_PATH];
        char szKeyName[MAX_PATH];

        SStrCopy(szKeyName, "ai_dll", MAX_PATH);
        if ( gdwProcNum )
        {
          char tst[16];
          sprintf_s(tst, 16, "_%u", gdwProcNum);
          SStrNCat(szKeyName, tst, MAX_PATH);
        }
#ifdef _DEBUG
          SStrNCat(szKeyName, "_dbg", MAX_PATH);
#endif
        DWORD dwDesiredRevision = 0;
        DWORD dwDesiredBuild    = 0; // 0 = undefined, 1 = release, 2 = debug
        GetPrivateProfileString("ai", szKeyName, "NULL", szDllPath, MAX_PATH, szConfigPath);
        if ( SStrCmpI(szDllPath, "NULL", MAX_PATH) == 0)
        {
          BWAPIError("Could not find %s under ai in \"%s\" for revision identification.", szKeyName, szConfigPath);
        }
        else
        {
          // Open File
          HANDLE hFile = NULL;
          if ( !SFileOpenFileEx(NULL, szDllPath, SFILE_FROM_ABSOLUTE, &hFile) || !hFile)
          {
            BWAPIError("Could not load module \"%s\" for revision identification.", szDllPath);
          }
          else
          {
            // Obtain file size
            DWORD dwFileSize = SFileGetFileSize(hFile, 0);

            // Allocate memory
            char *pbBuffer = (char*)SMAlloc(dwFileSize);
            if ( !pbBuffer )
            {
              BWAPIError("Unable to allocate enough memory for module \"%s\" for revision identification.", szDllPath);
            }
            else
            {
              // Read file
              DWORD dwBytesRead = 0;
              SFileReadFile(hFile, pbBuffer, dwFileSize, &dwBytesRead, 0);
              for ( u32 i = 0; i < dwBytesRead && (dwDesiredRevision == 0 || dwDesiredBuild == 0); ++i )
              {
                if ( memcmp(&pbBuffer[i], "XBWAPIXREVISIONXSTAMPX", 22) == 0 )
                {
                  i += 22;
                  sscanf(&pbBuffer[i], "%u", &dwDesiredRevision);
                  i += 5;
                }  // if REVISION
                if ( memcmp(&pbBuffer[i], "XBWAPIXBUILDXSTAMPX", 19) == 0 )
                {
                  i += 19;
                  if ( strcmp(&pbBuffer[i], "DEBUG") == 0 )
                  {
                    dwDesiredBuild = 2;
                    i += 6;
                  }
                  else if ( strcmp(&pbBuffer[i], "RELEASE") == 0 )
                  {
                    dwDesiredBuild = 1;
                    i += 8;
                  }
                } // if BUILD
              } // for (iterate file)
              // Free memory and close file
              SMFree(pbBuffer);
              SFileCloseFile(hFile);
            } // buffer was allocated
          } // file was opened
        } // module str was found

        /* Get revision from config */
        if ( dwDesiredRevision == 0 )
        {
          char szKeyName[128];
          SStrCopy(szKeyName, "ai_dll_rev", 128);
          if ( gdwProcNum )
            sprintf(szKeyName, "ai_dll_%d_rev", gdwProcNum);
          dwDesiredRevision = GetPrivateProfileInt("ai", szKeyName, SVN_REV, szConfigPath);
        }
        if ( dwDesiredRevision != SVN_REV )
        {
          // revision that ai_dll_# for multiple instances was introduced
          if ( gdwProcNum && dwDesiredRevision < 2753 )
          {
            char err[512];
            sprintf(err, "Revision %u is not compatible with multiple instances.\nExpecting revision 2753 (BWAPI Beta 3.1) or greater. If you proceed, the older revision of BWAPI will attempt to load its module from ai_dll instead of the multi-instance specification. Do you want to continue anyway?", dwDesiredRevision);
            BWAPIError("%s", err);
            if ( MessageBox(NULL, err, "Error", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1 | MB_TASKMODAL) == IDNO )
              return TRUE;
          } // revision is old

          if ( dwDesiredBuild == 0 )
            dwDesiredBuild = BUILD_DEBUG + 1;
          char szRevModule[MAX_PATH];
          sprintf_s(szRevModule, MAX_PATH, "%sbwapi-data\\revisions\\%u%s.dll", szInstallPath, dwDesiredRevision, dwDesiredBuild == 2 ? "d" : "");
          HMODULE hLib = LoadLibrary(szRevModule);
          if ( hLib )
          {
            char msg[MAX_PATH+32];
            char szLoadedName[MAX_PATH];
            GetModuleFileName(hLib, szLoadedName, MAX_PATH);
            sprintf_s(msg, MAX_PATH+32, "Loaded \"%s\" instead.", szLoadedName);
            MessageBox(NULL, msg, "Success", MB_OK | MB_ICONINFO);
            return TRUE;
          }

          char err[512];
          sprintf(err, "Couldn't find revision module \"%s\" of which the AI DLL was compiled for. Do you want to try using the current revision instead?", szRevModule);
          BWAPIError("%s", err);
          if ( MessageBox(NULL, err, "Error", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1 | MB_TASKMODAL) == IDNO )
            return TRUE;
        } // specified rev is not this one
        else if ( dwDesiredBuild && BUILD_DEBUG + 1 != dwDesiredBuild )
        {
          char envBuffer[MAX_PATH];
          if ( !GetEnvironmentVariable("ChaosDir", envBuffer, MAX_PATH) )
            if ( !GetCurrentDirectory(MAX_PATH, envBuffer) )
              BWAPIError("Could not find ChaosDir or current directory for build identification.");

          SStrNCat(envBuffer, "\\BWAPI", MAX_PATH);
          if ( dwDesiredBuild == 2 )
            SStrNCat(envBuffer, "d", MAX_PATH);
          SStrNCat(envBuffer, ".dll", MAX_PATH);

          HMODULE hLib = LoadLibrary(envBuffer);
          if ( hLib )
          {
            char msg[MAX_PATH+32];
            sprintf_s(msg, MAX_PATH+32, "Loaded \"%s\" instead.", envBuffer);
            MessageBox(NULL, msg, "Success", MB_OK | MB_ICONINFO);
            return TRUE;
          }

          char err[512];
          sprintf(err, "Couldn't find build module \"%s\" of which the AI DLL was compiled for. Do you want to try using the current build instead?", envBuffer);
          BWAPIError("%s", err);
          if ( MessageBox(NULL, err, "Error", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1 | MB_TASKMODAL) == IDNO )
            return TRUE;
          return TRUE;
        }

        SYSTEMTIME sysTime;
        GetSystemTime(&sysTime);
        if ( sysTime.wMonth == 12 && sysTime.wDay >= 18 && sysTime.wDay <= 27 )
          gdwHoliday = 1;

        CTRT_Thread(NULL);
        BWAPI::BWAPI_init();
        CreateThread(NULL, 0, &PersistentPatch, NULL, 0, NULL);
        return TRUE;
      }
  }
  return TRUE;
}
