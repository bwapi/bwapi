#include "Mutex.h"

namespace Util
{
  Mutex::Mutex()
  {
	  hMutex = CreateMutex(NULL, false, NULL);
  }

  Mutex::Mutex(const char* systemName)
  {
	  hMutex = CreateMutex(NULL, false, systemName);
  }

  Mutex::~Mutex()
  {
	  CloseHandle(hMutex);
  }


  Mutex::Lock::Lock() : target(NULL)
  {
  }

  Mutex::Lock::Lock(Mutex& fresh_target) : target(NULL)
  {
	  lock(fresh_target);
  }

  void Mutex::Lock::lock(Mutex& fresh_target)
  {
	  if(target)
		  release();
	  target = &fresh_target;
    WaitForSingleObject(target->hMutex, INFINITE);
  }

  bool Mutex::Lock::tryLock(Mutex& fresh_target)
  {
    bool locked = WaitForSingleObject(fresh_target.hMutex, 0) != WAIT_TIMEOUT;
	  if(locked)
		  target = &fresh_target;
	  else
		  target = NULL;
	  return locked;
  }

  Mutex::Lock::~Lock()
  {
	  release();
  }

  void Mutex::Lock::release()
  {
	  if(!target)
		  return;
    ReleaseMutex(target->hMutex);
	  target = NULL;
  }
}
