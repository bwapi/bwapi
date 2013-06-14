#include <windows.h>
// See chaoslauncher.cpp for chaoslauncher plugin
// See mpqdraft.cpp for mpqdraft plugin

HINSTANCE hGlobInstance;

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    hGlobInstance = hInstance;
    break;
  case DLL_PROCESS_DETACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  }
  return TRUE;
}
