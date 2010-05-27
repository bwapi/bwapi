#include "MPQDraftPlugin.h"
#include <windows.h>
#include <string>
#include <assert.h>
#include <stdio.h>

#include "../../svnrev.h"

//cl /LD qdp.cpp ..\QDPlugin.def /linkUser32.lib
#define THEPLUGINID 0x10001000
#define ENV_BUFFER_SIZE 512

const char* plugin_name = "BWAPI Injector (1.16.1) r";

void BWAPIError(const char *format, ...)
{
  char buffer[MAX_PATH];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(buffer, MAX_PATH, MAX_PATH, format, ap);
  va_end(ap);

  FILE* f = fopen("bwapi-error.txt", "a+");
  fprintf(f, "%s\n", buffer);
  fclose(f);
  MessageBoxA(NULL, buffer, "Error", MB_OK);
}

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
      strcat(pPluginName, SVN_REV_STR);
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
      if (!ShellExecuteA(NULL, "open", "..\\bwapi-data\\bwapi.ini", NULL, NULL, SW_SHOWNORMAL))
        return FALSE;
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
      char envBuffer[ENV_BUFFER_SIZE];
      if ( !GetEnvironmentVariableA("ChaosDir", envBuffer, ENV_BUFFER_SIZE) )
        if ( !GetCurrentDirectoryA(ENV_BUFFER_SIZE, envBuffer) )
          BWAPIError("Could not find ChaosDir or current directory.");

      strcat(envBuffer, "\\BWAPI.dll");
      if (!LoadLibraryA(envBuffer))
        BWAPIError("Could not find and/or load BWAPI.dll.");
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

