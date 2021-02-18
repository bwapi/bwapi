#include "valloc.h"

#include <Windows.h>

VAlloc::VAlloc(HANDLE hProcess, SIZE_T dwSize)
  : hTargetProcess(hProcess)
  , pAddress( VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE) )
{}

VAlloc::~VAlloc()
{
  if ( this->pAddress != NULL )
    VirtualFreeEx(this->hTargetProcess, this->pAddress, 0, MEM_RELEASE);
}

bool VAlloc::Write(LPCVOID lpBuffer, SIZE_T nSize)
{
  SIZE_T written;
  BOOL result = WriteProcessMemory(this->hTargetProcess, this->pAddress, lpBuffer, nSize, &written);
  return result == TRUE && nSize == written;
}

LPVOID VAlloc::GetAddress() const
{
  return this->pAddress;
}

VAlloc::operator bool() const
{
  return this->pAddress != NULL;
}

