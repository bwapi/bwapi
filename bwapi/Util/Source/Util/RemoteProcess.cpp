#include "RemoteProcess.h"

namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  RemoteProcess::RemoteProcess()
    : processHandle(INVALID_HANDLE_VALUE)
  {
  }

  RemoteProcess::~RemoteProcess()
  {
    release();
  }
  //----------------------- GET HANDLE ---------------------------------
  HANDLE RemoteProcess::getHandle()
  {
    return this->processHandle;
  }
  //----------------------- AQUIRE -------------------------------------
  bool RemoteProcess::acquire(RemoteProcessID remoteProcessID, bool moreAccess)
  {
    this->processHandle = OpenProcess(
      moreAccess ? 
      PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION :
      PROCESS_DUP_HANDLE,
      NULL,
      remoteProcessID);

    if(this->processHandle == NULL)
    {
      this->processHandle = INVALID_HANDLE_VALUE;
      return false;
    }

    return true;
  }
  //----------------------- RELEASE ------------------------------------
  void RemoteProcess::release()
  {
    if(this->processHandle != INVALID_HANDLE_VALUE)
    {
      CloseHandle(this->processHandle);
      this->processHandle = INVALID_HANDLE_VALUE;
    }
  }
  //----------------------- RELEASE ------------------------------------
  int RemoteProcess::exportOwnHandle()
  {
    int retval;
    if(!::DuplicateHandle(
      ::GetCurrentProcess(),
      ::GetCurrentProcess(),
      this->processHandle,
      (HANDLE*)&retval,
      PROCESS_DUP_HANDLE,
      false,
      NULL))
    {
      return 0;
    }
    return retval;
  }
  //----------------------- IMPORT HANDLE ------------------------------
  void RemoteProcess::importHandle(int handle)
  {
    this->processHandle = (HANDLE)handle;
  }
  //----------------------- IS VALID -----------------------------------
  bool RemoteProcess::isValid()
  {
    return this->processHandle != INVALID_HANDLE_VALUE;
  }
}
