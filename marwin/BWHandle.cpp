#include "BWHandle.h"
BWHandle::BWHandle(void)
 {
  HWND hwnd;
  DWORD pid;

  hwnd = FindWindow(NULL, "Brood War");
  GetWindowThreadProcessId(hwnd, &pid);

  if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
   {
    if (GetLastError() == ERROR_NO_TOKEN)
     {
      if (!ImpersonateSelf(SecurityImpersonation))
        throw Exception("Imprisonate self error");
      if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
        throw Exception("Open thread token error");
     }
    else
      throw Exception("Open Thread token error");
   }

  if(!this->setPrivilege(hToken, SE_DEBUG_NAME, true))
   {
    CloseHandle(hToken);
    throw Exception("Set privilige failed.");
   }
  // open the process
  hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  // disable SeDebugPrivilege
  this->setPrivilege(hToken, SE_DEBUG_NAME, FALSE);
  if (hProcess == 0)
    throw Exception("Broodwar not loaded.");
 }
//-------------------------------- DESTRUCTOR ----------------------------------
BWHandle::~BWHandle(void)
 {
  CloseHandle(hToken);
  CloseHandle(hProcess);
 }
//------------------------------ SET PRIVILEGE ---------------------------------
bool BWHandle::setPrivilege(HANDLE hToken, LPCTSTR Privilege, bool bEnablePrivilege)
 {
  TOKEN_PRIVILEGES tp;
  LUID luid;
  TOKEN_PRIVILEGES tpPrevious;
  DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

  if(!LookupPrivilegeValue( NULL, Privilege, &luid ))
    return false;

  //
  // first pass.  get current privilege setting
  //
  tp.PrivilegeCount           = 1;
  tp.Privileges[0].Luid       = luid;
  tp.Privileges[0].Attributes = 0;

  AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tp,
            sizeof(TOKEN_PRIVILEGES),
            &tpPrevious,
            &cbPrevious
            );

  if (GetLastError() != ERROR_SUCCESS)
    return false;

  //
  // second pass.  set privilege based on previous setting
  //
  tpPrevious.PrivilegeCount       = 1;
  tpPrevious.Privileges[0].Luid   = luid;

  if(bEnablePrivilege)
    tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
  else
   {
    tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
    tpPrevious.Privileges[0].Attributes);
   }
  AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tpPrevious,
            cbPrevious,
            NULL,
            NULL
            );
  if (GetLastError() != ERROR_SUCCESS)
    return false;
  return true;
 }
//------------------------------------------------------------------------------
void BWHandle::readMemory(LPVOID address, void* data, int size)
 {
  ReadProcessMemory(hProcess, address, data, size, NULL);
 }
//------------------------------------------------------------------------------
void BWHandle::writeMemory(LPVOID address, void* data, int size)
 {
  WriteProcessMemory(hProcess, address, data, size, NULL);
 }
//------------------------------------------------------------------------------

