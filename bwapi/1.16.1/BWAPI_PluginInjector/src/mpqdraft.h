#pragma once
#include <Windows.h>

#include "QDPlugin.h"

class MPQDraftPluginInterface : public IMPQDraftPlugin
{
public:
  BOOL WINAPI Identify(LPDWORD pluginID);
  BOOL WINAPI GetPluginName(LPSTR pPluginName, DWORD namebufferlength);
  BOOL WINAPI CanPatchExecutable(LPCSTR exefilename);
  BOOL WINAPI Configure(HWND parentwindow);
  BOOL WINAPI ReadyForPatch();
  BOOL WINAPI GetModules(MPQDRAFTPLUGINMODULE* pluginmodules, LPDWORD nummodules);
  BOOL WINAPI InitializePlugin(IMPQDraftServer* server);
  BOOL WINAPI TerminatePlugin();
};
