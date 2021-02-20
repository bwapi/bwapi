#pragma once
#include <Windows.h>

class VAlloc
{
public:
  VAlloc(HANDLE hProcess, SIZE_T dwSize);

  ~VAlloc();

  bool Write(LPCVOID lpBuffer, SIZE_T nSize);
  LPVOID GetAddress() const;

  operator bool() const;
private:
  HANDLE hTargetProcess;
  LPVOID pAddress;
};
