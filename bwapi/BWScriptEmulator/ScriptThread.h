#pragma once
#include <windows.h>
#include <BWAPI.h>

#include <list>
#include <stack>
#include <deque>
#include <tuple>
#include "Location.h"

using namespace BWAPI;
using namespace Filter;

void UpdateScripts();
bool LoadAIBinary(const char *pszFileName);
int GetStandardUnitCount(BWAPI::UnitType type, bool bCompleted = true, Player player = NULL);

#define AI_THREAD_BROODWAR        0x01
#define AI_THREAD_KILLABLE        0x02
#define AI_THREAD_WANT_KILLED     0x04

class aithread
{
public:
  // ctors
  aithread();
  aithread(WORD wStartBlock, BWAPI::Position location, void *town = NULL);
  
  // dtor
  ~aithread();
  
  // Read data
  template <class T>
  T read()
  {
    T rval = (T&)pbAIScriptBinary[this->dwScriptOffset];
    this->dwScriptOffset += sizeof(T);
    this->dwBytesRead    += sizeof(T);
    return rval;
  };

  // Helper for tuple reading
  template <class T, size_t S> struct TupleHelper;

  // Base case
  template <class T>
  struct TupleHelper<T,0>
  { 
    static void readTuple(T &tup, aithread &thread) {};
  };

  // Recursive
  template <class T, size_t S>
  struct TupleHelper
  {
    static void readTuple(T &tup, aithread &thread)
    {
      TupleHelper<T,S-1>::readTuple(tup,thread);
      std::get<S-1>(tup) = thread.read<std::tuple_element<S-1,T>::type>();
    };
  };

  // Read data as tuple
  template <class T>
  void readTuple(T &tup)
  {
    TupleHelper<T,std::tuple_size<T>::value>::readTuple(tup, *this);
  };

  // Execute script
  void  execute();

  // Flags
  DWORD getFlags() const;
  void  setFlags(DWORD dwFlags);
  void  clearFlags(DWORD dwFlags);

  // Sleep
  DWORD sleep();
  void setSleep(DWORD dwSleepAmt);

  // Thread
  void  killThread();

  // Debug
  void  showDebug(int x, int y);
  void  saveDebug(const char prefix, int iOpcode, const char *pszFormat = NULL, ...);

  // Retry
  bool  noretry();
  bool  retry();

  // Execution Offset
  void setScriptOffset(DWORD dwNewOffset);
  DWORD getScriptOffset() const;

  // Location
  Location getLocation();

  // Callstack
  void call(DWORD dwOffset);
  bool ret();

  // Town
  void *town();

private:
  DWORD             dwScriptOffset;   // the offset in the AIScript file
  DWORD             dwSleepTime;      // the number of frames remaining before the thread continues execution
  DWORD             dwPlayerID;       // the player index that owns the thread

  Location          location;         // the location boundary that the script is being executed in
  void              *pTown;           // town information
  DWORD             dwFlags;          // the flags for the thread behaviour (contains baseID*8, flags are &7)
  BYTE              bTotBuildCount[UnitTypes::Enum::MAX]; // not included

  int               threadId;
  DWORD             dwBytesRead;
  std::deque<char*> debugQueue;
  std::stack<DWORD> callstack;

  bool retryBlock;
};

void AICreateThread(WORD wStartBlock, Position location, void *pTown = NULL);
void AICreateThread(const char *pszScriptID, Position location);

extern BYTE *pbAIScriptBinary;
extern std::list<aithread> aiThreadList;

