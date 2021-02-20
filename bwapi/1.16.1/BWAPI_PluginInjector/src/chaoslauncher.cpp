#include <filesystem>
#include <sstream>
#include <string>

#include <Windows.h>

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
  Data.iStarCraftBuild = 13;
  Data.bConfigDialog   = TRUE;
  Data.bNotSCBWmodule  = FALSE;
}

extern "C" __declspec(dllexport) void GetData(char* name, char* description, char* updateurl)
{
  std::stringstream ss_desc;
  ss_desc << "Injects " << MODULE << " into the Broodwar process.\r\n\r\n"
     << "For Starcraft: BroodWar 1.16.1.\r\n"
     << "Check for updates at https://bwapi.github.io/ \r\n\r\n"
     << "Created by the BWAPI Project Team.";

  strcpy(name, GetPluginName().substr(0, 63).c_str());
  strcpy(description, ss_desc.str().substr(0, 511).c_str());
  strcpy(updateurl, "");
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
  std::string bwapiDllPath = GetBWAPITarget();

  if ( !std::filesystem::exists(bwapiDllPath) )
    return BWAPIError("Unable to find %s.", bwapiDllPath.c_str());

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
  if ( !alloc.Write(bwapiDllPath.c_str(), bwapiDllPath.size() + 1) )
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
