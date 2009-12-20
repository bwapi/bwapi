#include "MPQDraftPlugin.h"
#include <windows.h>
#include <string.h>
#include <assert.h>

//cl /LD qdp.cpp ..\QDPlugin.def /linkUser32.lib
#define THEPLUGINID 0x10001000

const char* plugin_name = "BWAPI Injector (1.16.1)";

void mBox(const char* s)
{
  MessageBox(0, s, "!", 0);
}

class MPQDraftPluginInterface : public IMPQDraftPlugin
{
    HINSTANCE hInstance;
  public:
    BOOL WINAPI Identify(LPDWORD pluginID)
    {
      if (!pluginID)
      {
        mBox("Wtf? no space for pluginId?");
        return false;
      }
      *pluginID = THEPLUGINID;
      return true;
    }
    BOOL WINAPI GetPluginName(LPSTR pPluginName, DWORD namebufferlength)
    {
      if (!pPluginName)
      {
        mBox("No buffer :O");
      }
      if (namebufferlength < strlen(plugin_name))
      {
        mBox("Name buffer too short!");
        return false;
      }
      strcpy(pPluginName, plugin_name);
      return true;
    }
    BOOL WINAPI CanPatchExecutable(LPCSTR exefilename)
    {
      //Generally you check if the version is right here
      return TRUE;
    }
    BOOL WINAPI Configure(HWND parentwindow)
    {
      //Goes here when they hit Configure
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
      {
        return false;
        mBox("NumModules null?");
      }
      *nummodules = 0;
      return true;
    }
    BOOL WINAPI InitializePlugin(IMPQDraftServer* server)
    {
      const DWORD ENV_BUFFER_SIZE = 512;
      char envBuffer[512];

      DWORD result = GetEnvironmentVariable("ChaosDir", envBuffer, ENV_BUFFER_SIZE);
      if (result == 0)
        result = GetCurrentDirectory(ENV_BUFFER_SIZE, envBuffer);
      assert(result != 0);

      char* dllFileName = strcat(envBuffer, "\\BWAPI.dll\0");
      HANDLE hProcess = GetCurrentProcess();

      LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
      assert(NULL != loadLibAddress);

      void* pathAddress = VirtualAllocEx(hProcess, NULL, strlen(dllFileName) + 1, MEM_COMMIT, PAGE_READWRITE);
      assert(NULL != pathAddress);

      SIZE_T bytesWritten;

      BOOL success = WriteProcessMemory(hProcess, pathAddress, dllFileName, strlen(dllFileName) + 1, &bytesWritten);
      assert(success && bytesWritten == strlen(dllFileName) + 1);

      HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
      assert(NULL != hThread);

      WaitForSingleObject(hThread, INFINITE);

      DWORD hLibModule; // Base address of the loaded module
      GetExitCodeThread(hThread, &hLibModule);
      assert(0 != hLibModule);

      VirtualFreeEx(hProcess, pathAddress, strlen(dllFileName) + 1, MEM_RELEASE);
      CloseHandle(hThread);

      return true;
    }
    BOOL WINAPI TerminatePlugin()
    {
      //Called when starcraft closes
      return true;
    }
    void WINAPI SetInstance(HINSTANCE hInst)
    {
      hInstance = hInst;
    }
};

MPQDraftPluginInterface thePluginInterface;

__declspec(dllexport) BOOL APIENTRY DllMain( HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
      thePluginInterface.SetInstance(hInstance);
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      break;
  }

  return TRUE;
}

BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin** lppMPQDraftPlugin)
{
  *lppMPQDraftPlugin = &thePluginInterface;
  return TRUE;
}

