#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

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

#include "../../Debug.h"

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

//---------------------------------------------- QUEUE COMMAND -----------------------------------------------
void __fastcall QueueGameCommand(void *pBuffer, DWORD dwLength)
{
  CAPS caps;
  caps.dwSize = sizeof(CAPS);
  SNetGetProviderCaps(&caps);

  DWORD dwMaxBuffer = caps.maxmessagesize;
  if ( dwMaxBuffer > 512 )
    dwMaxBuffer = 512;

  if ( dwLength + *BW::BWDATA_sgdwBytesInCmdQueue <= dwMaxBuffer )
  {
    // Copy data to primary turn buffer
    memcpy(&BW::BWDATA_TurnBuffer[*BW::BWDATA_sgdwBytesInCmdQueue], pBuffer, dwLength);
    *BW::BWDATA_sgdwBytesInCmdQueue += dwLength;
    return;
  }
  
  // Verify game mode
  if ( *BW::BWDATA_gwGameMode == 4 )
    return;

  int turns;
  if ( SNetGetTurnsInTransit(&turns) ) // Buffer is full
  {
    int callDelay = 1;
    if ( *BW::BWDATA_NetMode )
    {
      callDelay = caps.dwCallDelay;
      if ( callDelay > 8 )
        callDelay = 8;
      else if ( callDelay < 2 )
        callDelay = 2;
    }
    // This statement will probably never be hit, but just in case
    if ( turns >= 16 - callDelay )
      return;

    // Send the turn and fill the new buffer
    BW::BWFXN_sendTurn();
    memcpy(&BW::BWDATA_TurnBuffer[*BW::BWDATA_sgdwBytesInCmdQueue], pBuffer, dwLength);
    *BW::BWDATA_sgdwBytesInCmdQueue += dwLength;
  }
  // assume no error, would be fatal in Starcraft anyway
}

int getFileType(const char *szFileName)
{
  if ( !szFileName )
    return 0;

  int rVal = 0;
  HANDLE hMPQ;
  HANDLE hFile;
  // Open archive for map checking
  if ( SFileOpenArchive(szFileName, 0, 0, &hMPQ) && hMPQ )
  {
    // Open scenario.chk file
    if ( SFileOpenFileEx(hMPQ, "staredit\\scenario.chk", SFILE_FROM_MPQ, &hFile) && hFile )
    {
      rVal = 1;
      SFileCloseFile(hFile);
    }
    // Close archive
    SFileCloseArchive(hMPQ);
  }
  else if ( SFileOpenFileEx(NULL, szFileName, SFILE_FROM_ABSOLUTE, &hFile) && hFile )
  {
    DWORD dwRead = 0;
    char tbuff[16];
    DWORD dwSize = SFileGetFileSize(hFile, 0);
    // Read file data to check if it's a replay
    if ( dwSize > 16 && SFileReadFile(hFile, &tbuff, 16, &dwRead, 0) && dwRead == 16 && *(DWORD*)&tbuff[12] == 'SRer' )
      rVal = 2;
    // Close file
    SFileCloseFile(hFile);
  }
  return rVal;
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
  free(buffer);
}

void BWAPIError(DWORD dwErrCode, const char *format, ...)
{
  char *buffer;
  vstretchyprintf(buffer, format);

  char szErrString[256];
  SErrGetErrorStr(dwErrCode, szErrString, 256);
  BWAPIError("%s    %s", szErrString, buffer);
  free(buffer);
}

char logPath[MAX_PATH];
//--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
DWORD WINAPI CTRT_Thread(LPVOID)
{
  /* Initialize logging options */
  GetPrivateProfileString("paths", "log_path", "bwapi-data\\logs", logPath, MAX_PATH, szConfigPath);

  char szBuffer[32];
  windowRect.left   = GetPrivateProfileInt("window", "left",   0, szConfigPath);
  windowRect.top    = GetPrivateProfileInt("window", "top",    0, szConfigPath);
  windowRect.right  = GetPrivateProfileInt("window", "width",  0, szConfigPath);
  windowRect.bottom = GetPrivateProfileInt("window", "height", 0, szConfigPath);
  GetPrivateProfileString("window", "windowed", "OFF", szBuffer, 32, szConfigPath);
  if ( std::string( strupr(szBuffer) ) == "ON" )
    switchToWMode = true;

  if ( windowRect.right < WMODE_MIN_WIDTH )
    windowRect.right = WMODE_MIN_WIDTH;
  if ( windowRect.bottom < WMODE_MIN_HEIGHT )
    windowRect.bottom = WMODE_MIN_HEIGHT;

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

  BWAPI::BroodwarImpl.loadAutoMenuData();
  ApplyCodePatches();
  return 0;
}

DWORD WINAPI PersistentPatch(LPVOID)
{
  for ever
  {
    Sleep(250);

    // dialog/menu layer
    if ( BW::BWDATA_ScreenLayers[2].pUpdate != DrawDialogHook && 
         BW::BWDATA_ScreenLayers[2].pUpdate != NULL )
    {
      BW::pOldDrawDialogProc = BW::BWDATA_ScreenLayers[2].pUpdate;
      BW::BWDATA_ScreenLayers[2].pUpdate = DrawDialogHook;
    }

    // game layer
    if ( BW::BWDATA_ScreenLayers[5].pUpdate != DrawHook && BW::BWDATA_ScreenLayers[5].pUpdate != NULL )
    {
      BW::pOldDrawGameProc = BW::BWDATA_ScreenLayers[5].pUpdate;
      BW::BWDATA_ScreenLayers[5].pUpdate = DrawHook;
    }

    // Only grab this info if we are not currently detouring the CreateWindowEx procedure
    if ( !detourCreateWindow )
    {
      if ( !ghMainWnd )
        ghMainWnd = SDrawGetFrameWindow();

      if ( ghMainWnd )
      {
        WNDPROC thisProc = (WNDPROC)GetWindowLong(ghMainWnd, GWLP_WNDPROC);
        if ( thisProc != &WindowProc )
        {
          wOriginalProc = thisProc;
          SetWindowLong(ghMainWnd, GWLP_WNDPROC, (LONG)&WindowProc);
        }
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
#ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
        /* Retrieve the Starcraft path */
        if ( SRegLoadString("Starcraft", "InstallPath", SREG_NONE, szInstallPath, MAX_PATH) )
          SStrNCat(szInstallPath, "\\", MAX_PATH);

        /* Create the config path */
        SStrCopy(szConfigPath, szInstallPath, MAX_PATH);
        SStrNCat(szConfigPath, "bwapi-data\\bwapi.ini", MAX_PATH);

        /* Get screenshot format */
        GetPrivateProfileString("Starcraft", "screenshots", "gif", gszScreenshotFormat, 4, szConfigPath);

        /* Get process count */
        gdwProcNum = getProcessCount("StarCraft_MultiInstance.exe");

        /* Get display warnings */
        bool showWarn  = true;
        char szTmp[16];
        GetPrivateProfileString("Config", "show_warnings", "YES", szTmp, sizeof(szTmp), szConfigPath);
        if ( strcmpi(szTmp, "YES") != 0 )
          showWarn = false;

        /* Get revision/build automatically */
        char szDllPath[MAX_PATH];
        char szKeyName[MAX_PATH];

        SStrCopy(szKeyName, "ai", MAX_PATH);
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
          // Tokenize and retrieve correct path for the instance number
          char *pszDll = strtok(szDllPath, ",");
          for ( unsigned int i = 0; i < gdwProcNum-1; ++i )
          {
            char *pszNext = strtok(NULL, ",");
            if ( !pszNext )
              break;
            pszDll = pszNext;
          }
          // Retrieve revision info if it exists
          char *pszLoadRevCheck = strchr(pszDll, ':');
          if ( pszLoadRevCheck )
          {
            pszLoadRevCheck[0] = 0;
            ++pszLoadRevCheck;
            sscanf(pszLoadRevCheck, "%u", &dwDesiredRevision);
          }

          // Remove spaces
          while ( isspace(pszDll[0]) )
            ++pszDll;

          // Open File
          HANDLE hFile = NULL;
          if ( !SFileOpenFileEx(NULL, pszDll, SFILE_FROM_ABSOLUTE, &hFile) || !hFile)
          {
              BWAPIError("Could not load module \"%s\" for revision identification.", pszDll);
          }
          else
          {
            // Obtain file size
            DWORD dwFileSize = SFileGetFileSize(hFile, 0);

            // Allocate memory
            char *pbBuffer = (char*)SMAlloc(dwFileSize);
            if ( !pbBuffer )
            {
                BWAPIError("Unable to allocate enough memory for module \"%s\" for revision identification.", pszDll);
            }
            else
            {
              // Read file
              DWORD dwBytesRead = 0;
              SFileReadFile(hFile, pbBuffer, dwFileSize, &dwBytesRead, 0);
              for ( u32 i = 0; i < dwBytesRead && (dwDesiredRevision == 0 || dwDesiredBuild == 0); ++i )
              {
                if ( dwDesiredRevision == 0 && memcmp(&pbBuffer[i], "XBWAPIXREVISIONXSTAMPX", 22) == 0 )
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

          /* Do revision checking */
          if ( dwDesiredRevision > 0 && dwDesiredRevision != SVN_REV )
          {
            // revision that ai_dll_# for multiple instances was introduced
            if ( gdwProcNum && dwDesiredRevision < 2753 && showWarn )
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
              if ( showWarn )
              {
                char msg[MAX_PATH+32];
                char szLoadedName[MAX_PATH];
                GetModuleFileName(hLib, szLoadedName, MAX_PATH);
                sprintf_s(msg, MAX_PATH+32, "Loaded \"%s\" instead.", szLoadedName);
                MessageBox(NULL, msg, "Success", MB_OK | MB_ICONINFO);
              }
              return TRUE;
            }

            if ( showWarn )
            {
              char err[512];
              sprintf(err, "Couldn't find revision module \"%s\" of which the AI DLL was compiled for. Do you want to try using the current revision instead?", szRevModule);
              BWAPIError("%s", err);
              if ( MessageBox(NULL, err, "Error", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1 | MB_TASKMODAL) == IDNO )
                return TRUE;
            }
          } // specified rev is not this one
          else if ( dwDesiredBuild && BUILD_DEBUG + 1 != dwDesiredBuild )
          {
            char envBuffer[MAX_PATH];
            if ( !GetEnvironmentVariable("ChaosDir", envBuffer, MAX_PATH) )
              if ( !GetCurrentDirectory(MAX_PATH, envBuffer) && showWarn )
                BWAPIError("Could not find ChaosDir or current directory for build identification.");

            SStrNCat(envBuffer, "\\BWAPI", MAX_PATH);
            if ( dwDesiredBuild == 2 )
              SStrNCat(envBuffer, "d", MAX_PATH);
            SStrNCat(envBuffer, ".dll", MAX_PATH);

            HMODULE hLib = LoadLibrary(envBuffer);
            if ( hLib )
            {
              if ( showWarn )
              {
                char msg[MAX_PATH+32];
                sprintf_s(msg, MAX_PATH+32, "Loaded \"%s\" instead.", envBuffer);
                MessageBox(NULL, msg, "Success", MB_OK | MB_ICONINFO);
              }
              return TRUE;
            }

            if ( showWarn )
            {
              char err[512];
              sprintf(err, "Couldn't find build module \"%s\" of which the AI DLL was compiled for. Do you want to try using the current build instead?", envBuffer);
              BWAPIError("%s", err);
              if ( MessageBox(NULL, err, "Error", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1 | MB_TASKMODAL) == IDNO )
                return TRUE;
            }
            return TRUE;
          }
        } // module str was found

        /* Check if it's time for a holiday */
        gdwHoliday = 0;
        char szHolidayMods[16];
        GetPrivateProfileString("config", "holiday", "ON", szHolidayMods, 16, szConfigPath);
        if ( SStrCmpI(szHolidayMods, "OFF", 16) != 0 )
        {
          SYSTEMTIME sysTime;
          GetSystemTime(&sysTime);
          if ( sysTime.wMonth == 12 && sysTime.wDay >= 18 && sysTime.wDay <= 28 )
            gdwHoliday = 1;
        }

        CTRT_Thread(NULL);
        BWAPI::BWAPI_init();
        CreateThread(NULL, 0, &PersistentPatch, NULL, 0, NULL);
        return TRUE;
      }
  }
  return TRUE;
}
