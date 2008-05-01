
#define _CRT_SECURE_NO_DEPRECATE
#include "windows.h"
#include "assert.h"
#include <string>
#pragma warning(disable : 4311)

#define BWLAPI 4
#define STARCRAFTBUILD 10

/*  STARCRAFTBUILD
-1   All
0   1.04
1   1.08b
2   1.09b
3   1.10
4   1.11b
5   1.12b
6   1.13f
7   1.14
8   1.15
9   1.15.1
10  1.15.2
*/

struct ExchangeData
{
   int iPluginAPI;
   int iStarCraftBuild;
   bool bConfigDialog;                 //Is Configurable
   bool bNotSCBWmodule;                //Inform user that closing BWL will shut down your plugin
};

BOOL APIENTRY DllMain( HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
   //Is this DLL also StarCraft module?

   switch (ul_reason_for_call)
   {
   case DLL_PROCESS_ATTACH: 
      return TRUE;
   case DLL_THREAD_ATTACH:
   case DLL_THREAD_DETACH:
   case DLL_PROCESS_DETACH:
      break;
   }

   return TRUE;
}
//
// Functions for BWLauncher
//

extern "C" __declspec(dllexport) void GetPluginAPI(ExchangeData &Data)
{
   //BWL Gets version from Resource - VersionInfo
   Data.iPluginAPI = BWLAPI;
   Data.iStarCraftBuild = STARCRAFTBUILD;
   Data.bConfigDialog = false;
   Data.bNotSCBWmodule = false;
}

extern "C" __declspec(dllexport) void GetData(char *name, char *description, char *updateurl)
{
   //if necessary you can add Initialize function here
   //possibly check CurrentCulture (CultureInfo) to localize your DLL due to system settings
   strcpy(name,      "CreateThreadRemoteTest");
   strcpy(description, "CreateThreadRemoteTest Description");
   strcpy(updateurl,   "");
}

extern "C" __declspec(dllexport) bool OpenConfig()
{
   return true;
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE hProcess, DWORD dwProcessID)
{
   return true;
}

extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE hProcess, DWORD dwProcessID)
{
   std::string dllFileName("C:\\Starcraft\\ChaosLauncher\\CTRT_Inject.dll");

   LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
   assert(NULL != loadLibAddress);

   void* pathAddress = VirtualAllocEx(hProcess, NULL, dllFileName.size()+1, MEM_COMMIT, PAGE_READWRITE);
   assert(NULL != pathAddress);

   SIZE_T bytesWritten;
   BOOL success = WriteProcessMemory(hProcess, pathAddress, dllFileName.c_str(), dllFileName.size()+1, &bytesWritten);
   assert(success && bytesWritten == dllFileName.size()+1);

   HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
   assert(NULL != hThread);

   WaitForSingleObject(hThread, INFINITE);

   DWORD hLibModule; // Base address of the loaded module
   GetExitCodeThread(hThread, &hLibModule);
   assert(0 != hLibModule);

   VirtualFreeEx(hProcess, pathAddress, dllFileName.size()+1, MEM_RELEASE);
   CloseHandle(hThread);

   return true;
}


