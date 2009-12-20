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
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

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
        friend class SharedMemory;    // structure is part of SharedMemory
    private:
	    int offset;
    };
    //----------------------- FUNCTIONS --------------------------------
    SharedMemory();
    ~SharedMemory();

    void create(int size);                      // allocates memory
    void import(Export source);     // connects to shared memory
    void release();                             // releases memory
    Export exportToProcess(RemoteProcess &target, bool readOnly) const;

    template<typename T>
      T *unpack(Pointer<T> p) const // to process address space
      {
        if(!(int)this->bufferBase)
          return NULL;
        return (T*)(this->bufferBase + p.offset);
      }

    template<typename T>
      Pointer<T> pack(T *pt) const  // to shared address space
      {
        Pointer<T> retval;
        retval.offset = (int)pt - (int)this->bufferBase;
      }

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