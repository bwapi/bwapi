/*
  QDPlugin.h
  The central hive of the MPQDraft plugin system.

  In addition to the standard MPQ "adding" functionality of MPQDraft,
  MPQDraft also supports a powerful plugin system to acommodate addition
  methods of patching. These plugins are specially constructed DLLs which
  1) Export the GetMPQDraftPlugin function, and 2) Impliment the
  IMPQDraftPlugin interface. Up to 8 plugins may be loaded in one patching
  session (although 8 is a rather arbitrary number, and may be changed in
  later versions, if there is reason to do so).

  - PLUGIN MODULES -
  MPQDraft supports the creation of completely self-contained SEMPQs. But,
  this ability creates a palette of problems all its own. All of a plugin's
  data files have to be packed into the SEMPQ. This means that a special
  mechanism is needed, to marshall the data files from the plugin to the
  SEMPQ, and back to the plugin; this is the plugin module system. Each data
  file of the plugin is referred to as a "module", and identified by
  a pair of DWORDs: a plugin ID, and a module ID. The plugin ID is the
  globally unique ID of the plugin; the module ID is the plugin-specific ID
  of the module.

  [FURTHER TEXT TO BE ADDED HERE]

  - SETUP SIDE -
  There are two halves to each plugin: a setup side and a patching side. The
  setup side is when the plugin gets loaded in MPQDraft, and displayed in
  the plugin list, in either the patching wizard or SEMPQ wizard.
  Specifically, MPQDraft loads all plugins when the plugins page in the
  wizard first gets activated. The plugins will remain loaded until the
  wizard is closed. If a patch is performed, the plugins will remain loaded
  until the patch is completed. This allows plugins to get the opportunity
  to delete any temporary files that were created for the patch.

  Order and timing of calls to setup-side plugin functions:

  When the plugins wizard page is first activated, and MPQDraft is building
  its plugin database:
  1. The plugin gets loaded with LoadLibrary.
  2. The plugin's GetMPQDraftPlugin function gets called to obtain the
  plugin's IMPQDraftPlugin interface.
  3. IMPQDraftPlugin::Identify gets called.
  4. IMPQDraftPlugin::GetPluginName gets called.

  Each time the plugins page is switched to, MPQDraft determines which
  plugins to display in the plugin list:
  - IMPQDraftPlugin::CanPatchExecutable gets called for the currently
  selected executable. If the plugin can patch the executable, it will
  appear in the plugin list if it can't, then it won't appear. NOTE: this
  step does not occur when selecting plugins for use in an SEMPQ. In this
  case, ALL plugins will be available to select.

  If the "Configure" button is clicked while a plugin is selected:
  - IMPQDraftPlugin::Configure is called, with the HWND to the wizard in
  use. This allows the plugin to create a modal dialog with various plugin
  settings.

  If the "Finish" button is clicked to initiate a patch:
  1. IMPQDraftPlugin::ReadyForPatch is called to discern whether the plugin
  is properly configured, and ready to perform the patch.
  2. IMPQDraftPlugin::GetModules is called to obtain the number of plugin
  modules the plugin will require.
  3. IMPQDraftPlugin::GetModule is called again to get the plugin's modules.

  Either after the patch is completed or the wizard is cancelled:
  - FreeLibrary is called to unload the plugin. The plugin is responsible
  for cleaning up any data files it created temporarily.

  NOTE: Plugins will not be loaded in setup-side when an SEMPQ is executed.

  - PATCHING SIDE -
  The patching side is when a plugin gets loaded by the MPQDraft patching
  kernel. The plugin will be loaded inside the patchee (the process being
  patched) BEFORE any patchee code gets executed, but after any DLLs the
  patchee uses are loaded and initialized (DllMain is called). This means
  that plugins will be able to modify initialized data in the patchee, but
  not uninitialized data. If the latter is necessary, a thread can be
  spawned by the plugin to wait until the data has been initialized.

  Order and timing of calls to patching-side plugin functions:

  After MPQDraft performs its own initializations, it will load each
  selected plugin (this is done before any MPQs are loaded):
  1. LoadLibrary gets called to load the plugin inside the patch target.
  2. GetMPQDraftPlugin gets called to obtain an IMPQDraftPlugin interface.
  3. IMPQDraftPlugin::InitializePlugin gets called with a pointer to the
  MPQDraft server interface, to allow the plugin to locate its data files.

  When the patchee is closing down, and MPQDraft is terminating:
  1. IMPQDraftPlugin::TerminatePlugin gets called to remove any patches the
  plugin performed.
  2. The plugin is unloaded with FreeLibrary.
*/

#ifndef QDPLUGIN_H
#define QDPLUGIN_H

#include <windows.h>

// The maximum length of a plugin module's filename. INCLUDES final NULL.
#define MPQDRAFT_MAX_PATH 264

// The maximum length of a plugin's name. INCLUDES final NULL.
#define MPQDRAFT_MAX_PLUGIN_NAME 64

/*
  MPQDRAFTPLUGINMODULE

  Structure used by IMPQDraftPlugin::GetModules to notify MPQDraft of any
  files (called plugin modules) that are to be loaded. Read description of
  that function for more information.
*/
#include <pshpack1.h>
struct MPQDRAFTPLUGINMODULE
{
  /* dwComponentID: The ID of the plugin. Should be the same value as is
  returned by IMPQDraftPlugin::Identify. Must be globally unique. */
  DWORD dwComponentID;
  /* dwModuleID: The unique ID of the plugin module. This will be used
  instead of the actual filename for identifying plugin modules. */
  DWORD dwModuleID;
  // bExecute: Used internally by MPQDraft. Must be set to FALSE.
  BOOL bExecute;
  // szModuleFileName: The absolute path of the plugin module file.
  char szModuleFileName[MPQDRAFT_MAX_PATH];
};
#include <poppack.h>

/*
  IMPQDraftServer

  Serves as a portal back to MPQDraft, allowing the plugin to
  not only be executed by MPQDraft, but also it communicate with MPQDraft.
  A plugin will be given an IMPQDraftServer pointer when MPQDraft calls
  IMPQDraftPlugin::InitializePlugin.
*/
struct IMPQDraftServer
{
  /*
    IMPQDraftServer::GetPluginModule

    Allows a plugin to locate its modules when it is loaded in patch-side.

    Return TRUE on success, and FALSE on failure.

    Parameters:
      dwPluginID [in] - The ID of the plugin who is attempting to locate
      its modules.
      dwModuleID [in] - The ID of the module to be located.
      lpszFileName [out] - Pointer to a buffer where MPQDraft will copy
      the file name of the module to. This buffer should be
      MPQDRAFT_MAX_PATH characters long.

    Behavior:
      - If lpszFileName is NULL, GetPluginModules will fail.

      - If the buffer pointer to by lpszFileName is shorter than the
      length of the module's filename (which will never be more than
      MPQDRAFT_MAX_PATH), a crash will result.
      - If no module identified by dwPluginID and dwModuleID can be
      found, GetPluginModule will fail.
      - If more than one module with identical dwPluginID and
      dwModuleID exist, MPQDraft will arbitrarily choose one to return.

      - If the desired module exists, GetPluginModule will copy the
      filename of the module to lpszFileName.
  */
  virtual BOOL WINAPI GetPluginModule(DWORD dwPluginID, DWORD dwModuleID, LPSTR lpszFileName) = 0;
};

/*
  IMPQDraftPlugin

  The primary gateway between the MPQDraft patching kernel and the plugin.
  This interface must be fully implimented by every MPQDraft plugin (or at
  least until it is superseded by IMPQDraftPlugin2, in a much later version
  of MPQDraft). MPQDraft will obtain this interface when it calls
  GetMPQDraftPlugin. It will then store the interface in its plugin
  database, and use it in all subsequent calls to the plugin.

  NOTE: The specifications for this interface are based on the recommended
  responses. In some places it may be legal to slightly depart from the
  recommended specs (i.e. functions may fail instead of asserting).
*/
struct IMPQDraftPlugin
{
  /*
    IMPQDraftPlugin::Identify

    Identifies the plugin to MPQDraft with a globally unique ID code.

    Returns TRUE on success, and FALSE on failure (which should never
    happen).

    Parameters:
      lpdwPluginID [out] - A pointer to a DWORD that will receive the
      plugin's ID. MPQDraft will supply this DWORD.

    Behavior:
      - If lpdwPluginID is NULL, Identify will assert.

      - On success, Identify will copy its ID to lpdwPluginID.
  */
  virtual BOOL WINAPI Identify(LPDWORD lpdwPluginID) = 0;

  /*
    GetPluginName

    Retrieves the name of the plugin which will be displayed in the
    plugin list in either of the MPQDraft wizards. This name should also
    include the version of the plugin, i.e. "StarGraft v1.08 QD".

    Returns TRUE on success, and FALSE on failure.

    Parameters:
      lpszPluginName [out] - A pointer to a buffer which will receive
      the name of the plugin. This buffer will by provided by MPQDraft,
      and will usually be MPQDRAFT_MAX_PLUGIN_NAME chars long.
      nNameBufferLength [in] - The length of the buffer pointed to by
      lpszPluginName, including space for the final NULL.

    Behavior:
      - If lpszPluginName is null, GetPluginName will assert.

      - If nNameBufferLength is shorter than the name of the plugin,
      GetPluginName will fail.

      - On success, GetPluginName will copy the plugin name to the
      buffer pointed to by lpszPluginName.
  */
  virtual BOOL WINAPI GetPluginName(LPSTR lpszPluginName, DWORD nNameBufferLength) = 0;
  /*
    IMPQDraftPlugin::CanPatchExecutable

    Called by MPQDraft in the plugins page of the patch wizard (but not on
    the SEMPQ wizard). Its return value determines whether or not the
    plugin will appear in the list of available plugins, as MPQDraft will
    only list plugins which are compatible with the currently selected
    patch target.

    Returns TRUE if the plugin can patch the selected executable, and
    FALSE if it cannot.

    Parameters:
      lpszEXEFileName [in] - The absolute path of the currently selected
      executable.

    Behavior:
      - If lpszEXEFileName is NULL, CanPatchExecutable will assert.

      - If an error occurs (i.e. the executable cannot be opened or read
      from, CanPatchExecutable will fail.
  */
  virtual BOOL WINAPI CanPatchExecutable(LPCSTR lpszEXEFileName) = 0;
  /*
    IMPQDraftPlugin::Configure

    Called by MPQDraft from the plugin page in either of the wizards.
    Configure should present the user with settings which can be
    adjusted to change the way the plugin will function (i.e. selecting
    the PAT to use in StarGraft). If necessary, the plugin can create a
    settings dialog. It is recommended that the plugin stores the settings
    from the last time it was configured in the registry, but this is not
    mandatory. Some plugins may even not require any configuration at all,
    in which case this function would be a simple "return TRUE".

    Returns TRUE on success and FALSE on failure.

    Parameters:
      hParentWnd [in] - A handle to the wizard from which Configure was
      called. This handle is to be used exclusively to pass to
      DialogBox, to create a modal dialog. It is NOT to be used to
      attempt to modify the wizard. Such an attempt will probably crash.

    Behavior:
      - If hParentWnd is NULL, Configure will assert.

      - If an error occurs while configuring the plugin, Configure will
      fail.

      - If the configuration completed sucessfully (even if the user
      pressed the "Cancel" button on a dialog), Configure will succeed.
  */
  virtual BOOL WINAPI Configure(HWND hParentWnd) = 0;

  /*
    IMPQDraftPlugin::ReadyForPatch

    Called by MPQDraft right before a patch, to determine whether all
    plugins are properly configured, and ready to patch.

    Returns TRUE if the plugin is configured properly, FALSE if it isn't.
  */
  virtual BOOL WINAPI ReadyForPatch() = 0;

  /*
    IMPQDraftPlugin::GetModules

    Called twice by MPQDraft right before it is about to perform a patch.
    The first time, MPQDraft collects the number of modules from all
    plugins, so that it can allocate the proper amount of memory to hold
    the list of modules. The second it will be to actually retrieve the
    list of modules. These modules will be packed into an SEMPQ. Or, if
    if a straight patch is being performed, MPQDraft will just pass on the
    module list to the patching kernel.

    Returns TRUE on success, and FALSE on failure.

    Parameters:
      lpPluginModules [out] - A pointer to the memory MPQDraft has
      allocated to hold the list of modules. The plugin must  list each
      module it will require in the patching process. When MPQDraft
      first calls GetModules, lpPluginModules will be NULL.
      lpnNumModules [out] - A pointer to the number of plugin modules
      the plugin will need. This number ought not to exceed 4.

    Behavior:
      - If lpnNumModules is NULL, GetModules will assert.

      - If an error occurs, and GetModules is unable to supply the
      required information, GetModules will fail. In this case, MPQDraft
      will abort the patch.

      - If lpPluginModules is non-NULL, GetModules will copy the
      list of modules to lpPluginModules, and give the number of modules
      in lpnNumModules.
      - If lpPluginModules is NULL, GetModules will give the exact
      number of modules it will require in lpnNumModules, and succeed.
  */
  virtual BOOL WINAPI GetModules(MPQDRAFTPLUGINMODULE* lpPluginModules, LPDWORD lpnNumModules) = 0;

  /*
    IMPQDraftPlugin::InitializePlugin

    Called by MPQDraft from inside the patch target, to allow the plugin
    to perform its patch. Any patches the plugin makes should be stored,
    and undone upon TerminatePlugin.

    A return value of TRUE indicates that MPQDraft should continue with
    the patch. FALSE indicates that MPQDraft should abort the patch. The
    plugin should report any errors BEFORE it returns FALSE, as MPQDraft
    will terminate the patch silently.

    Parameters:
      lpMPQDraftServer [in] - A pointer to an IMPQDraftServer interface,
      provided by MPQDraft. This interface can be used to locate the
      plugin's modules. This pointer should be saved in case it is
      needed in future use.

    Behavior:
      - If lpMPQDraftServer is NULL, InitializePlugin will assert.

      - If the plugin was unable to perform the patch, and the the patch
      target should be terminated, InitializePlugin will display an
      error message box and return FALSE. MPQDraft will abort the patch.
      - If the plugin was unable to perform the patch, and MPQDraft
      should ignore the error and continue, InitializePlugin will
      return TRUE. MPQDraft will continue the patch.

      - If the patch was performed successfully, InitializePlugin will
      return TRUE.
  */
  virtual BOOL WINAPI InitializePlugin(IMPQDraftServer* lpMPQDraftServer) = 0;

  /*
    IMPQDraftPlugin::TerminatePlugin

    Called by MPQDraft inside the patch target during the shutdown
    process, to allow the plugin to unload any patches it made.

    Returns TRUE on success, and FALSE on failure.

    Behavior:
      - If InitializePlugin was not called previously, TerminatePlugin
      asserts.

      - If the patches performed in InitializePlugin were not
      successfully removed, TerminatePlugin fails.
      - If the patches performed in InitializePlugin were successfully
      removed, TerminatePlugin succeeds.

      - It is possible that on rare occasions InitializePlugin will be
      called, then the plugin will be unloaded before TerminatePlugin
      gets called. The plugin should check for this on
      DLL_PROCESS_DETACH, and call TerminatePlugin itself.
  */
  virtual BOOL WINAPI TerminatePlugin() = 0;
};

/*
  GetMPQDraftPlugin

  Exported by name from the plugin DLL. Called by MPQDraft to obtain the
  IMPQDraftPlugin interface of the plugin.

  Returns TRUE on success, and FALSE on failure.

  Parameters:
    lppMPQDraftPlugin [out] - A pointer to a pointer that will hold the
    IMPQDraftPlugin. The plugin must set this to point to the plugin's
    IMPQDraftPlugin interface.

  Behavior:
    - GetMPQDraftPlugin will only be called once, so it isn't necessary
    to instantiate an IMPQDraftPlugin for each call. A single global
    IMPQDraftPlugin is sufficient.
*/
BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin** lppMPQDraftPlugin);

#endif // #ifndef QDPLUGIN_H
