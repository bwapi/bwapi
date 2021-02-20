#pragma once
/***********************************************************************
 *  SharedMemory is a memory buffer which can be shared with other
 *  processes
 *
 *
 *  SharedMemory::Pointer<T> is a T* pointer to inside a SharedMemory
 *  - SharedMemory instance dependent
 *  - process independent
 *
 *  use SharedMemory::unpack()
 *
 *
 *  SharedMemory::Export is used to export this SharedMemory instance
 *  to another processes, transmit it with any IPC and import at the
 *  other side
 ***********/

namespace Util
{
  class SharedMemory;
}

#include "MemoryFrame.h"
#include "Exceptions.h"
#include "RemoteProcess.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace Util
{
  class SharedMemory
  {
  public:
    //----------------------- TYPES ------------------------------------
    struct Export
    {
      friend class SharedMemory;    // structure is part of SharedMemory

      Export();
      bool isValid() const;

    private:
      HANDLE targetProcessMappingObjectHandle;
      unsigned int bufferSize;
      bool readOnly;
    };
    template <typename T> struct Pointer
    {
      Pointer() = default;

      friend class SharedMemory;    // structure is part of SharedMemory
    private:
      int offset;
    };
    //----------------------- FUNCTIONS --------------------------------
    SharedMemory();
    ~SharedMemory();

    bool create(int size, const char* systemName = nullptr);  // allocates memory, returns false if opened an existing named object
    void import(Export source);     // connects to shared memory
    void release();                             // releases memory
    Export exportToProcess(RemoteProcess &target, bool readOnly) const;

    MemoryFrame getMemory() const;

    static int getPageSize();                   // allocation size snapping

  private:
    //----------------------- ------------------------------------------
    SharedMemory(SharedMemory&);                // no copying
    HANDLE mappingObjectHandle;
    void *bufferBase;
    unsigned int bufferSize;
    //----------------------- ------------------------------------------
  };
}