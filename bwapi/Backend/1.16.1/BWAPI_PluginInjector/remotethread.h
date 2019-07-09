#pragma once
#include <Windows.h>

class RemoteThread
{
public:
  RemoteThread(HANDLE hProcess, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);
  ~RemoteThread();

  bool Wait() const;
  DWORD GetExitCode() const;

  operator bool() const;
private:
  HANDLE hThread;
};

