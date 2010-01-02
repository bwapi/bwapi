#include "DLLMain.h"

#include "BW/Hook.h"

#include "BWAPI/Engine.h"

/* TODO: where does this belong?
#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>
*/

//------------------------------------------------- DLL MAIN -------------------------------------------------
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    {
      try
      {
        BWAPI::Engine::onDllLoad();
        // DLLMain is limited, but I think memory transactions will be allowed
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CTRT_Thread, NULL, 0, NULL);
        BW::installHooks();
      }
      catch(...)
      {
        return false;
      }
      return true;
    }
  }
  return true;
}
