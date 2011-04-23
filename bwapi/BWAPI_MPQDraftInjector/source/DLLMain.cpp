#include "MPQDraftPlugin.h"
#include <windows.h>
#include <stdio.h>

#include "../../svnrev.h"
#include "../../starcraftver.h"

#include "../../Debug.h"

#define THEPLUGINID 0x10001000

#ifdef _DEBUG
#define MODULE "BWAPId.dll"
#else
#define MODULE "BWAPI.dll"
#endif

const char* plugin_name = "BWAPI Injector (" STARCRAFT_VER ") r" SVN_REV_STR " (" BUILD_STR ")";

DWORD GetRegString(HKEY hBaseKey, const char *pszSubKey, const char *pszValueName, char *pszOutput, DWORD *dwOutSize)
{
  HKEY hKey = NULL;;
  DWORD dwErrCode = RegOpenKeyEx(hBaseKey, pszSubKey, 0, KEY_QUERY_VALUE, &hKey);
  if ( dwErrCode != ERROR_SUCCESS )
    return dwErrCode;

  dwErrCode = RegQueryValueEx(hKey, pszValueName, NULL, NULL, (LPBYTE)pszOutput, dwOutSize);
  RegCloseKey(hKey);
  return dwErrCode;
}

BOOL BWAPIError(DWORD dwErrCode, const char *format, ...)
{
  char buffer[MAX_PATH];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(buffer, MAX_PATH, MAX_PATH, format, ap);
  va_end(ap);

  char szErrMsg[256];
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrCode, 0, szErrMsg, MAX_PATH, NULL);

  SYSTEMTIME time;
  GetSystemTime(&time);
  FILE* f = fopen("bwapi-error.txt", "a+");
  if ( f )
  {
    fprintf(f, "[%u/%02u/%02u - %02u:%02u:%02u] %s - %s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, buffer, szErrMsg);
    fclose(f);
  }

  char buffer2[MAX_PATH*2];
  sprintf(buffer2, "%s\n%s", buffer, szErrMsg);
  MessageBox(NULL, buffer2, "Error", MB_OK | MB_ICONERROR);
  return FALSE;
}

BOOL BWAPIError(const char *format, ...)
{
  char buffer[MAX_PATH];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(buffer, MAX_PATH, MAX_PATH, format, ap);
  va_end(ap);

  SYSTEMTIME time;
  GetSystemTime(&time);
  FILE* f = fopen("bwapi-error.txt", "a+");
  if ( f )
  {
    fprintf(f, "[%u/%02u/%02u - %02u:%02u:%02u] %s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, buffer);
    fclose(f);
  }
  MessageBox(NULL, buffer, "Error", MB_OK | MB_ICONERROR);
  return FALSE;
}

class MPQDraftPluginInterface : public IMPQDraftPlugin
{
    HINSTANCE hInstance;
  public:
    BOOL WINAPI Identify(LPDWORD pluginID)
    {
      if (!pluginID)
        return FALSE;

      *pluginID = THEPLUGINID;
      return TRUE;
    }
    BOOL WINAPI GetPluginName(LPSTR pPluginName, DWORD namebufferlength)
    {
      if ( !pPluginName || namebufferlength < strlen(plugin_name) )
        return FALSE;

      strcpy(pPluginName, plugin_name);
      return TRUE;
    }
    BOOL WINAPI CanPatchExecutable(LPCSTR exefilename)
    {
      //Generally you check if the version is right here
      return TRUE;
    }
    BOOL WINAPI Configure(HWND parentwindow)
    {
      char szBwPath[MAX_PATH];
      DWORD dwPathSize = MAX_PATH;

      DWORD dwErrCode = GetRegString(HKEY_CURRENT_USER, "SOFTWARE\\Blizzard Entertainment\\Starcraft", "InstallPath", szBwPath, &dwPathSize);
      if ( dwErrCode != ERROR_SUCCESS )
      {
        dwErrCode = GetRegString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Blizzard Entertainment\\Starcraft", "InstallPath", szBwPath, &dwPathSize);
        if ( dwErrCode != ERROR_SUCCESS )
          return BWAPIError(dwErrCode, "An error occured when retrieving the registry key.");
      }

      // Load the config file
      char szExecPath[MAX_PATH*2];
      strcpy(szExecPath, szBwPath);
      strcat(szExecPath, "\\bwapi-data\\bwapi.ini");
      if ( ShellExecute(NULL, "open", szExecPath, NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32 )
        return BWAPIError("Unable to open BWAPI config file.");
      return TRUE;
    }
    BOOL WINAPI ReadyForPatch()
    {
      //Right before it opens up SC, checks for panic?
      return TRUE;
    }
    BOOL WINAPI GetModules(MPQDRAFTPLUGINMODULE* pluginmodules, LPDWORD nummodules)
    {
      //Weird shit, i doubt you'll use it
      if (!nummodules)
        return FALSE;
      *nummodules = 0;
      return TRUE;
    }
    BOOL WINAPI InitializePlugin(IMPQDraftServer* server)
    {
      char envBuffer[MAX_PATH];
      bool envFailed = false;
      if ( !GetEnvironmentVariable("ChaosDir", envBuffer, MAX_PATH) )
      {
        envFailed = true;
        if ( !GetCurrentDirectory(MAX_PATH, envBuffer) )
          return BWAPIError(GetLastError(), "Could not find ChaosDir or CurrentDirectory.");
      }

      strcat(envBuffer, "\\" MODULE);
      DWORD dwFileAttribs = GetFileAttributes(envBuffer);
      if ( dwFileAttribs == INVALID_FILE_ATTRIBUTES || dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY )
      {
        if ( !envFailed && !GetCurrentDirectory(MAX_PATH, envBuffer) )
          return BWAPIError(GetLastError(), "Could not find CurrentDirectory.");
        strcat(envBuffer, "\\" MODULE);
        dwFileAttribs = GetFileAttributes(envBuffer);
        if ( dwFileAttribs == INVALID_FILE_ATTRIBUTES || dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY )
          return BWAPIError(GetLastError(), "Could not find file \"%s\".", envBuffer);
      }

      if ( !LoadLibrary(envBuffer) )
        return BWAPIError(GetLastError(), "Could not load \"%s\".", envBuffer);
      return TRUE;
    }
    BOOL WINAPI TerminatePlugin()
    {
      //Called when starcraft closes
      return TRUE;
    }
    void WINAPI SetInstance(HINSTANCE hInst)
    {
      hInstance = hInst;
    }
};

MPQDraftPluginInterface thePluginInterface;

__declspec(dllexport) BOOL APIENTRY DllMain( HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
  {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    thePluginInterface.SetInstance(hInstance);
  }
  return TRUE;
}

BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin** lppMPQDraftPlugin)
{
  *lppMPQDraftPlugin = &thePluginInterface;
  return TRUE;
}

