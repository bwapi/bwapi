#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <storm.h>

#include "Config.h"

#include "WMode.h"

std::string screenshotFmt;

bool isCorrectVersion = true;
bool showWarn         = true;
bool serverEnabled    = true;

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
      if( _strcmpi(pe32.szExeFile, pszProcName) == 0 )
        ++dwCount;
    } while( Process32Next(hSnapshot, &pe32) );
  }
  CloseHandle(hSnapshot);
  return dwCount;
}

//----------------------------- LOAD CONFIG FXNS ------------------------------------------
std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault)
{
  char buffer[MAX_PATH];
  GetPrivateProfileString(pszKey, pszItem, pszDefault ? pszDefault : "", buffer, MAX_PATH, configPath.c_str());
  return std::string(buffer);
}
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault)
{
  return GetPrivateProfileInt(pszKey, pszItem, iDefault, configPath.c_str());
}
std::string LoadRegString(const char *pszKeyName, const char *pszValueName)
{
  char szTemp[MAX_PATH] = { 0 };
  SRegLoadString(pszKeyName, pszValueName, SREG_NONE, szTemp, MAX_PATH);
  return std::string(szTemp);
}

bool isPrimaryConfigInitialized = false;
void InitPrimaryConfig()
{
  // Return if already initialized
  if ( isPrimaryConfigInitialized )
    return;
  isPrimaryConfigInitialized = true;

  // ------------------------- GENERAL/GLOBAL CONFIG OPTIONS ----------------------------------
  // Get screenshot format
  screenshotFmt = LoadConfigString("starcraft", "screenshots", "gif");
  if ( !screenshotFmt.empty() )
    screenshotFmt.insert(0, ".");

  // Check if warning dialogs should be shown
  showWarn = LoadConfigString("config", "show_warnings", "YES") == "YES";

  // Check if shared memory should be enabled
  serverEnabled = LoadConfigString("config", "shared_memory", "ON") == "ON";

/*  // Check if it's time for a holiday
  gdwHoliday = 0;
  if ( LoadConfigString("config", "holiday", "ON") != "OFF" )
  {
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    // Christmas
    if ( sysTime.wMonth == 12 && sysTime.wDay >= 18 && sysTime.wDay <= 28 )
      gdwHoliday = 1;
  }
  */
  // Get process count
  gdwProcNum = getProcessCount("StarCraft_MultiInstance.exe");

  // ------------------------- WMODE CONFIG OPTIONS ----------------------------------
  // Load windowed mode position and fullscreen setting
  windowRect.left   = LoadConfigInt("window", "left");
  windowRect.top    = LoadConfigInt("window", "top");
  windowRect.right  = LoadConfigInt("window", "width");
  windowRect.bottom = LoadConfigInt("window", "height");
  switchToWMode     = LoadConfigString("window", "windowed", "OFF") == "ON";

  // Limit minimum w-mode size
  if ( windowRect.right < WMODE_MIN_WIDTH )
    windowRect.right = WMODE_MIN_WIDTH;
  if ( windowRect.bottom < WMODE_MIN_HEIGHT )
    windowRect.bottom = WMODE_MIN_HEIGHT;

  // Shift the position of w-mode
  if ( gdwProcNum > 0 )
  {
    std::string wmodeCfg = installPath + "wmode.ini";

    DWORD dwWmodeConfigExists = GetFileAttributes(wmodeCfg.c_str());
    if ( dwWmodeConfigExists != INVALID_FILE_ATTRIBUTES && 
         !(dwWmodeConfigExists & FILE_ATTRIBUTE_DIRECTORY) )
    {
      // Get window location and screen dimensions
      int wx = GetPrivateProfileInt("W-MODE", "WindowClientX", 0, wmodeCfg.c_str());
      int wy = GetPrivateProfileInt("W-MODE", "WindowClientY", 0, wmodeCfg.c_str());
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
      sprintf(szScrOutput, "%d", wx);
      WritePrivateProfileString("W-MODE", "WindowClientX", szScrOutput, wmodeCfg.c_str());
      sprintf(szScrOutput, "%d", wy);
      WritePrivateProfileString("W-MODE", "WindowClientY", szScrOutput, wmodeCfg.c_str());
    } // file exists
  } // is multi-instance


}

