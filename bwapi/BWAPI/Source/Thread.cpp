//
// Usage: SetThreadName (-1, "MainThread");
//
#include <windows.h>
#include <map>

#include "Thread.h"

///////////////////////////////////////////////////////////////////////
//////// The "official"ly modified version of SetThreadName
////////
const unsigned MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   unsigned dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   unsigned dwThreadID; // Thread ID (-1=caller thread).
   unsigned dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName( const char *threadName, unsigned threadId)
{
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName;
   info.dwThreadID = threadId;
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Map of thread names
std::map<unsigned,LPCSTR> threadNames;

// Function to register the name of a thread
void RegisterThreadName(const char *threadName, unsigned threadId)
{
  threadNames[threadId == ~0 ? GetCurrentThreadId() : threadId] = threadName;
}

// Function to check if a debugger is present, and then set the names of all registered threads if it is so
void CheckRegisteredThreads()
{
  // Don't do anything if debugger is not present
  if ( !IsDebuggerPresent() )
    return;

  // iterate all registered thread names and set them for the debugger
  for ( auto &e : threadNames)
    SetThreadName(e.second, e.first);
}
