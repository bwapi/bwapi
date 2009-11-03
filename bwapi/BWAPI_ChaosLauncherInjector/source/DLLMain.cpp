#include <stdio.h>
#include <windows.h>
#include <string>
#include <assert.h>
#include <shellapi.h>
#include "..\\..\\svnrev.h"

#define BWLAPI 4
#define STARCRAFTBUILD 13

struct ExchangeData
{
  int iPluginAPI;
  int iStarCraftBuild;
  BOOL bNotSCBWmodule;                //Inform user that closing BWL will shut down your plugin
  BOOL bConfigDialog;                 //Is Configurable
};

BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}

// GET Functions for BWLauncher
//
extern "C" __declspec(dllexport) void GetPluginAPI(ExchangeData& Data)
{
  //BWL Gets version from Resource - VersionInfo
  Data.iPluginAPI = BWLAPI;
  Data.iStarCraftBuild = STARCRAFTBUILD;
  Data.bConfigDialog = TRUE;
  Data.bNotSCBWmodule = FALSE;
}

extern "C" __declspec(dllexport) void GetData(char* name, char* description, char* updateurl)
{
  char newDescription[512];
  sprintf_s(newDescription, 512, "Injects BWAPI.dll into the Broodwar process.\r\n\r\nRevision %s.\r\nCheck for updates at http://bwapi.googlecode.com/ \r\n\r\nCreated by the BWAPI Project Team", SVN_REV_STR);
  
  strcpy(name, "BWAPI Injector (1.16.1)");
  strcpy(description, newDescription);
  strcpy(updateurl, "http://bwapi.googlecode.com/files/");
}

// Functions called by BWLauncher
//
extern "C" __declspec(dllexport) bool OpenConfig()
{
  if (ShellExecuteA(NULL, "open", "..\\bwapi-data\\bwapi.ini", NULL, NULL, SW_SHOWNORMAL) == 0)
    return false;
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE, DWORD)
{
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE hProcess, DWORD)
{
  const DWORD ENV_BUFFER_SIZE = 512;
  char envBuffer[512];

  DWORD result = GetEnvironmentVariable("ChaosDir", envBuffer, ENV_BUFFER_SIZE);
  if (result == 0)
    result = GetCurrentDirectory(ENV_BUFFER_SIZE, envBuffer);
  assert(result != 0);

  std::string dllFileName(envBuffer);
  dllFileName.append("\\BWAPI.dll");

  LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
  assert(NULL != loadLibAddress);

  void* pathAddress = VirtualAllocEx(hProcess, NULL, dllFileName.size() + 1, MEM_COMMIT, PAGE_READWRITE);
  assert(NULL != pathAddress);

  SIZE_T bytesWritten;

  BOOL success = WriteProcessMemory(hProcess, pathAddress, dllFileName.c_str(), dllFileName.size() + 1, &bytesWritten);
  assert(success && bytesWritten == dllFileName.size() + 1);

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
  assert(NULL != hThread);

  WaitForSingleObject(hThread, INFINITE);

  DWORD hLibModule; // Base address of the loaded module
  GetExitCodeThread(hThread, &hLibModule);
  assert(0 != hLibModule);

  VirtualFreeEx(hProcess, pathAddress, dllFileName.size() + 1, MEM_RELEASE);
  CloseHandle(hThread);

  return true; //everything OK
}
