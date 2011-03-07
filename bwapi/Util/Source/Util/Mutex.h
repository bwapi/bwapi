#pragma once

//
//  A windows mutex or named mutex object
//  Lockable with RAII
//

#include <windows.h>

namespace Util
{
  class Mutex
  {

	  // constructors
  public:
	  Mutex();
	  Mutex(const char* systemName); // named mutex
	  ~Mutex();

	  // state
  private:
	  HANDLE hMutex;

	  // methods
  public:
	  friend class Lock;
	  class Lock
	  {
		  // constructors
	  public:
		  Lock(Mutex&);
		  Lock();
		  ~Lock();


		  // state
	  private:
		  Mutex *target;

		  // methods
	  public:
		  void lock(Mutex&);
		  bool tryLock(Mutex&);
		  void release();
	  };
  };
}
