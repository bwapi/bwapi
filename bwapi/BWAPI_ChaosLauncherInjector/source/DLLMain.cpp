#include <stdio.h>
#include <windows.h>
#include <string>
#include <assert.h>

#include "../../svnrev.h"
#include "../../starcraftver.h"

#ifdef _DEBUG
#define BUILD "DEBUG"
#define MODULE "BWAPId.dll"
#elif NDEBUG
#define BUILD "RELEASE"
#define MODULE "BWAPI.dll"
#endif

struct ExchangeData
{
  int  iPluginAPI;
  int  iStarCraftBuild;
  BOOL bNotSCBWmodule;                //Inform user that closing BWL will shut down your plugin
  BOOL bConfigDialog;                 //Is Configurable
};

void BWAPIError(const char *format, ...)
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
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
  return TRUE;
}

// GET Functions for BWLauncher
//
extern "C" __declspec(dllexport) void GetPluginAPI(ExchangeData& Data)
{
  //BWL Gets version from Resource - VersionInfo
  Data.iPluginAPI      = 4;
  Data.iStarCraftBuild = 13;
  Data.bConfigDialog   = TRUE;
  Data.bNotSCBWmodule  = FALSE;
}

extern "C" __declspec(dllexport) void GetData(char* name, char* description, char* updateurl)
{
  char newDescription[512];
  sprintf_s(newDescription, 512, "Injects " MODULE " into the Broodwar process.\r\n\r\nRevision %s.\r\nCheck for updates at http://bwapi.googlecode.com/ \r\n\r\nCreated by the BWAPI Project Team", SVN_REV_STR);
  strcpy(name, "BWAPI Injector (" STARCRAFT_VER ") " BUILD);
  strcpy(description, newDescription);
  strcpy(updateurl, "http://bwapi.googlecode.com/files/");
}

// Functions called by BWLauncher
//
extern "C" __declspec(dllexport) bool OpenConfig()
{
  char szBwPath[MAX_PATH];
  DWORD dwPathSize = MAX_PATH;

  LSTATUS dwErrCode = RegQueryValueEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Blizzard Entertainment\\Starcraft\\InstallPath", NULL, NULL, (LPBYTE)szBwPath, &dwPathSize);
  if ( dwErrCode != ERROR_SUCCESS )
    BWAPIError("Failed to load registry value properly: 0x%p", dwErrCode);

  char szExecPath[MAX_PATH*2];
  strcpy(szExecPath, szBwPath);
  strcat(szExecPath, "\\bwapi-data\\bwapi.ini");
  if ( !ShellExecute(NULL, "open", szExecPath, NULL, NULL, SW_SHOWNORMAL) )
    BWAPIError("Unable to open BWAPI config file.");
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE, DWORD)
{
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE hProcess, DWORD)
{
  char envBuffer[MAX_PATH];
  if ( !GetEnvironmentVariable("ChaosDir", envBuffer, MAX_PATH) )
    if ( !GetCurrentDirectory(MAX_PATH, envBuffer) )
      BWAPIError("Could not find ChaosDir or current directory.");

  std::string dllFileName(envBuffer);
  dllFileName.append("\\" MODULE);

  LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
  if ( !loadLibAddress )
    BWAPIError("Could not get Proc Address for LoadLibraryA.");

  void* pathAddress = VirtualAllocEx(hProcess, NULL, dllFileName.size() + 1, MEM_COMMIT, PAGE_READWRITE);
  if ( !pathAddress )
    BWAPIError("Could not allocate memory for DLL path.");

  SIZE_T bytesWritten;
  BOOL success = WriteProcessMemory(hProcess, pathAddress, dllFileName.c_str(), dllFileName.size() + 1, &bytesWritten);
  if ( !success || bytesWritten != dllFileName.size() + 1)
    BWAPIError("Unable to write process memory.");

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
  if ( !hThread )
    BWAPIError("Unable to create remote thread.");

  WaitForSingleObject(hThread, INFINITE);

  DWORD hLibModule = NULL;
  GetExitCodeThread(hThread, &hLibModule);

  if ( !hLibModule )
    BWAPIError("Injection failed.\nThis may be caused by mixing DEBUG and RELEASE builds.");

  VirtualFreeEx(hProcess, pathAddress, dllFileName.size() + 1, MEM_RELEASE);
  CloseHandle(hThread);
  return true; //everything OK
}
