#include <Windows.h>
#include "mpqdraft.h"
#include "common.h"

#include "config.h"

#define THEPLUGINID 0x10001000


BOOL WINAPI MPQDraftPluginInterface::Identify(LPDWORD pluginID)
{
  if (!pluginID)
    return FALSE;

  *pluginID = THEPLUGINID;
  return TRUE;
}
  
BOOL WINAPI MPQDraftPluginInterface::GetPluginName(LPSTR pPluginName, DWORD namebufferlength)
{
  // safeguard
  if ( !pPluginName )
    return FALSE;

  // Copy the plugin name to the target
  strcpy(pPluginName, ::GetPluginName().substr(0, namebufferlength-1).c_str());
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::CanPatchExecutable(LPCSTR /*exefilename*/)
{  //Generally you check if the version is right here
  return TRUE;
}
  
BOOL WINAPI MPQDraftPluginInterface::Configure(HWND /*parentwindow*/)
{
  return RunConfig() ? TRUE : FALSE;
}

BOOL WINAPI MPQDraftPluginInterface::ReadyForPatch()
{  //Right before it opens up SC, checks for panic?
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::GetModules(MPQDRAFTPLUGINMODULE* /*pluginmodules*/, LPDWORD nummodules)
{  //Weird stuff, i doubt you'll use it
  if (!nummodules)
    return FALSE;
  *nummodules = 0;
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::InitializePlugin(IMPQDraftServer* /*server*/)
{
  std::string target = GetBWAPITarget();

  if ( !LoadLibrary(target.c_str()) )
    return BWAPIError("Could not load \"%s\".", target.c_str());
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::TerminatePlugin()
{  //Called when starcraft closes
  return TRUE;
}

// Define the interface and exported function
MPQDraftPluginInterface pluginInterface;

BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin** lppMPQDraftPlugin)
{
  *lppMPQDraftPlugin = &pluginInterface;
  return TRUE;
}
