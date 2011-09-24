#include <stdio.h>
#include <windows.h>
#include <string>
#include <assert.h>

#include "../../svnrev.h"
#include "../../starcraftver.h"

#include "../../Debug.h"

#ifdef _DEBUG
#define MODULE "BWAPId.dll"
#else
#define MODULE "BWAPI.dll"
#endif

struct ExchangeData
{
  int  iPluginAPI;
  int  iStarCraftBuild;
  BOOL bNotSCBWmodule;                //Inform user that closing BWL will shut down your plugin
  BOOL bConfigDialog;                 //Is Configurable
};

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

bool BWAPIError(DWORD dwErrCode, const char *format, ...)
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

bool BWAPIError(const char *format, ...)
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
  MessageBox(NULL, buffer, "Error", MB_OK | MB_ICONERROR );
  return false;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
  return TRUE;
}

// GET Functions for BWLauncher
//
extern "C" __declspec(dllexport) void GetPluginAPI(ExchangeData& Data)
{
  //BWL Gets version from Resource - VersionInfo
  Data.iPluginAPI      = 4;
  Data.iStarCraftBuild = -1;
  Data.bConfigDialog   = TRUE;
  Data.bNotSCBWmodule  = FALSE;
}

extern "C" __declspec(dllexport) void GetData(char* name, char* description, char* updateurl)
{
  char newDescription[512];
  sprintf_s(newDescription, 512, "Injects " MODULE " into the Broodwar process.\r\n\r\nRevision %s.\r\nCheck for updates at http://bwapi.googlecode.com/ \r\n\r\nCreated by the BWAPI Project Team", SVN_REV_STR);
  strcpy(name, "BWAPI Injector (" STARCRAFT_VER ") " BUILD_STR);
  strcpy(description, newDescription);
  strcpy(updateurl, "http://bwapi.googlecode.com/files/");
}

// Functions called by BWLauncher
//
extern "C" __declspec(dllexport) bool OpenConfig()
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
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE, DWORD)
{
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE hProcess, DWORD)
{
  MessageBox(NULL, "", 0, 0);
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
  DWORD dwDllSize = strlen(envBuffer)+1;

  LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
  if ( !loadLibAddress )
    return BWAPIError(GetLastError(), "Could not get Proc Address for LoadLibraryA.");

  // @TODO: Suspend thread?
  void* pathAddress = VirtualAllocEx(hProcess, NULL, dwDllSize, MEM_COMMIT, PAGE_READWRITE);
  if ( !pathAddress )
    return BWAPIError(GetLastError(), "Could not allocate memory for DLL path.");

  SIZE_T bytesWritten;
  BOOL success = WriteProcessMemory(hProcess, pathAddress, envBuffer, dwDllSize, &bytesWritten);
  if ( !success )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    return BWAPIError(GetLastError(), "Unable to write process memory.");
  }
  if ( bytesWritten != dwDllSize )
    BWAPIError("WriteToProcessMemory bytesWritten is not the expected value.");

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
  if ( !hThread )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    return BWAPIError(GetLastError(), "Unable to create remote thread.");
  }

  if ( WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    CloseHandle(hThread);
    return BWAPIError(GetLastError(), "WaitForSingleObject failed.");
  }

  DWORD dwExitCode = NULL;
  if ( !GetExitCodeThread(hThread, &dwExitCode) )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    CloseHandle(hThread);
    return BWAPIError(GetLastError(), "GetExitCodeThread failed.");
  }

  if ( !dwExitCode )
    BWAPIError(GetLastError(), "Injection failed.\nThis is caused when BWAPI crashes before injecting completely.");

  VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
  CloseHandle(hThread);
  return true; //everything OK
}
