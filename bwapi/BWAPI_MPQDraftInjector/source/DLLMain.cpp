#include "MPQDraftPlugin.h"
#include <windows.h>
#include <string>
#include <assert.h>
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
      HKEY hKey;

      char szErrString[256];
      DWORD dwErrCode = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Blizzard Entertainment\\Starcraft", 0, KEY_QUERY_VALUE, &hKey);
      if ( dwErrCode != ERROR_SUCCESS )
      {
        dwErrCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Blizzard Entertainment\\Starcraft", 0, KEY_QUERY_VALUE, &hKey);
        if ( dwErrCode != ERROR_SUCCESS )
        {
          FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrCode, 0, szErrString, 256, NULL);
          return BWAPIError("An error occured when opening the registry key:\n0x%p\n%s", dwErrCode, szErrString);
        }
      }

      if ( !hKey )
        return FALSE;

      dwErrCode = RegQueryValueEx(hKey, "InstallPath", NULL, NULL, (LPBYTE)szBwPath, &dwPathSize);
      if ( dwErrCode != ERROR_SUCCESS )
      {
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrCode, 0, szErrString, 256, NULL);
        return BWAPIError("An error occured when querying the registry value:\n0x%p\n%s", dwErrCode, szErrString);
      }
      RegCloseKey(hKey);

      // Load the config file
      char szExecPath[MAX_PATH*2];
      strcpy(szExecPath, szBwPath);
      strcat(szExecPath, "\\bwapi-data\\bwapi.ini");
      if ( !ShellExecute(NULL, "open", szExecPath, NULL, NULL, SW_SHOWNORMAL) )
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
          return BWAPIError("Could not find ChaosDir or CurrentDirectory. \nError Code: 0x%p", GetLastError());
      }

      strcat(envBuffer, "\\" MODULE);
      DWORD dwFileAttribs = GetFileAttributes(envBuffer);
      if ( dwFileAttribs == INVALID_FILE_ATTRIBUTES || dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY )
      {
        if ( !envFailed && !GetCurrentDirectory(MAX_PATH, envBuffer) )
          return BWAPIError("Could not find CurrentDirectory. \nError Code: 0x%p", GetLastError());
        strcat(envBuffer, "\\" MODULE);
        dwFileAttribs = GetFileAttributes(envBuffer);
        if ( dwFileAttribs == INVALID_FILE_ATTRIBUTES || dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY )
          return BWAPIError("Could not find file \"%s\". \nError Code: 0x%p", envBuffer, GetLastError());
      }

      if ( !LoadLibrary(envBuffer) )
        return BWAPIError("Could not load \"%s\". \nError Code: 0x%p", envBuffer, GetLastError());
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

