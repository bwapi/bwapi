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
        // though DLLMain is limited, memory transactions are not a crime
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
