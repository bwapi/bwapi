#include "remotethread.h"

#include <Windows.h>

RemoteThread::RemoteThread(HANDLE hProcess, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
  : hThread( CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, lpParameter, 0, NULL) )
{}

RemoteThread::~RemoteThread()
{
  if ( this->hThread )
    CloseHandle(this->hThread);
}

bool RemoteThread::Wait() const
{
  return WaitForSingleObject(this->hThread, INFINITE) != WAIT_FAILED;
}
DWORD RemoteThread::GetExitCode() const
{
  DWORD dwExitCode = NULL;
  GetExitCodeThread(this->hThread, &dwExitCode);
  return dwExitCode;
}

RemoteThread::operator bool() const
{
  return this->hThread != NULL;
}
