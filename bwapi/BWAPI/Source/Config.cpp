#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <storm.h>
#include <cctype>

#include "Config.h"

#include "WMode.h"
#include "Util/Convenience.h"

std::string screenshotFmt;

bool isCorrectVersion = true;
bool showWarn         = true;
bool serverEnabled    = true;

unsigned gdwProcNum = 1;

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
  GetPrivateProfileStringA(pszKey, pszItem, pszDefault ? pszDefault : "", buffer, MAX_PATH, configPath().c_str());
  return std::string(buffer);
}
// this version uppercase result string after loading, should be used for the most of enum-like strings
std::string LoadConfigStringUCase (const char *pszKey, const char *pszItem, const char *pszDefault)
{
  return toUpper(LoadConfigString (pszKey, pszItem, pszDefault));
}
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault)
{
  return GetPrivateProfileIntA(pszKey, pszItem, iDefault, configPath().c_str());
}
std::string LoadRegString(const char *pszKeyName, const char *pszValueName)
{
  char szTemp[MAX_PATH] = { 0 };
  SRegLoadString(pszKeyName, pszValueName, SREG_NONE, szTemp, MAX_PATH);
  return std::string(szTemp);
}

void InitPrimaryConfig()
{
  static bool isPrimaryConfigInitialized = false;
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
  showWarn = LoadConfigStringUCase("config", "show_warnings", "YES") == "YES";

  // Check if shared memory should be enabled
  serverEnabled = LoadConfigStringUCase("config", "shared_memory", "ON") == "ON";

  // Get process count
  gdwProcNum = getProcessCount("StarCraft.exe");

  // ------------------------- WMODE CONFIG OPTIONS ----------------------------------
  // Load windowed mode position and fullscreen setting
  windowRect.left   = LoadConfigInt("window", "left");
  windowRect.top    = LoadConfigInt("window", "top");
  windowRect.right  = LoadConfigInt("window", "width");
  windowRect.bottom = LoadConfigInt("window", "height");
  switchToWMode     = LoadConfigStringUCase("window", "windowed", "OFF") == "ON";

  // Limit minimum w-mode size
  if ( windowRect.right < WMODE_MIN_WIDTH )
    windowRect.right = WMODE_MIN_WIDTH;
  if ( windowRect.bottom < WMODE_MIN_HEIGHT )
    windowRect.bottom = WMODE_MIN_HEIGHT;

}

