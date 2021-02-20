#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <storm.h>

#include "StringUtil.h"

#include "Config.h"

std::string screenshotFmt;

bool isCorrectVersion = true;
bool showWarn         = true;

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
std::string envKeyName(const char *pszKey, const char *pszItem)
{
  return "BWAPI_CONFIG_" + Util::to_upper_copy(pszKey) + "__" + Util::to_upper_copy(pszItem);
}
std::string LoadConfigStringFromFile(const char *pszKey, const char *pszItem, const char *pszDefault)
{
  char buffer[MAX_PATH];
  GetPrivateProfileStringA(pszKey, pszItem, pszDefault ? pszDefault : "", buffer, MAX_PATH, configPath().c_str());
  return std::string(buffer);
}
std::string LoadConfigString(const char *pszKey, const char *pszItem, const char *pszDefault)
{
  std::string envKey = envKeyName(pszKey, pszItem);
  if (char* v = std::getenv(envKey.c_str()))
    return v;
  else
    return LoadConfigStringFromFile(pszKey, pszItem, pszDefault);
}
// this version uppercase result string after loading, should be used for the most of enum-like strings
std::string LoadConfigStringUCase (const char *pszKey, const char *pszItem, const char *pszDefault)
{
  return Util::to_upper_copy(LoadConfigString(pszKey, pszItem, pszDefault));
}
int LoadConfigInt(const char *pszKey, const char *pszItem, const int iDefault)
{
  std::string envKey = envKeyName(pszKey, pszItem);
  if (char* v = std::getenv(envKey.c_str()))
    return std::stoi(v);
  else
    return GetPrivateProfileIntA(pszKey, pszItem, iDefault, configPath().c_str());
}
void WriteConfig(const char *pszKey, const char *pszItem, const std::string& value)
{
  // avoid writing unless the value is actually different, because writing causes
  // an annoying popup when having the file open in e.g. notepad++
  if (LoadConfigStringFromFile(pszKey, pszItem, "_NULL") != value)
    WritePrivateProfileStringA(pszKey, pszItem, value.c_str(), configPath().c_str());
}
void WriteConfig(const char *pszKey, const char *pszItem, int value)
{
  WriteConfig(pszKey, pszItem, std::to_string(value));
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

  // Get process count
  gdwProcNum = getProcessCount("StarCraft.exe");
}

