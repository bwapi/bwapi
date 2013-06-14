#include <Windows.h>
#include <cstdio>

#include "chaoslauncher.h"
#include "common.h"

#include "valloc.h"
#include "remotethread.h"

#include "config.h"

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
  return RunConfig();
}

extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE, DWORD)
{
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE hProcess, DWORD)
{
  // Get target file name
  char szTarget[MAX_PATH];
  strncpy(szTarget, GetBWAPITarget().c_str(), MAX_PATH-1);
  szTarget[MAX_PATH-1] = '\0';

  // Check if the file exists, INVALID_FILE_ATTRIBUTES will have this bit set too
  if ( GetFileAttributes(szTarget) & FILE_ATTRIBUTE_DIRECTORY )
    return BWAPIError("Unable to find %s.", szTarget);

  // Get the address for the LoadLibrary procedure
  HMODULE hKernalModule = GetModuleHandle("Kernel32");
  if ( !hKernalModule )
    return BWAPIError("Unable to get module handle for Kernel32.");

  LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernalModule, "LoadLibraryA" );
  if ( !loadLibAddress )
    return BWAPIError("Could not get Proc Address for LoadLibraryA.");

  // Create a remote allocation
  VAlloc alloc(hProcess, MAX_PATH);
  if ( !alloc )
    return BWAPIError("Could not allocate memory for DLL path.");

  // Write the DLL path to the allocation
  if ( !alloc.Write(szTarget, MAX_PATH) )
    return BWAPIError("Write process memory failed.");

  // Create a remote thread for LoadLibrary and pass the DLL path as a parameter
  RemoteThread thread(hProcess, loadLibAddress, alloc.GetAddress());
  if ( !thread )
    return BWAPIError("Unable to create remote thread.");

  // Wait for the thread to finish
  if ( !thread.Wait() )
    return BWAPIError("WaitForSingleObject failed.");

  // The exit code for LoadLibrary is its return value, if it's NULL then loading failed
  if ( thread.GetExitCode() == NULL )
    return BWAPIError("Injection failed.\nThis is caused when BWAPI crashes before injecting completely.");

  return true; //everything OK
}
