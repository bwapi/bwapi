#pragma once
////////////////////////////////////////////////////////////////////////
//  A memory buffer which can be shared with other processes          //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util
{
  class SharedMemory;
}

#include "MemoryFrame.h"
#include "RemoteProcess.h"
#include "SharedMemoryExport.h"
#include "SharedMemoryPointer.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace Util
{
  class SharedMemory
  {
  public:
    SharedMemory();
    ~SharedMemory();

    bool create(int size);                      // allocates memory
    bool import(SharedMemoryExport source);     // connects to shared memory
    void discard();                             // releases memory
    SharedMemoryExport exportToProcess(RemoteProcess &target, bool readOnly) const;

    template<typename T>
      T *unpack(SharedMemoryPointer<T> p) const // to process address space
      {
        if(!winBufferAddress)
          return NULL;
        return (T*)(this->winBufferAddress + p.offset);
      }

    template<typename T>
      SharedMemoryPointer<T> pack(T *pt) const  // to shared address space
      {
        SharedMemoryPointer<T> retval;
        retval.offset = (int)this->winBufferAddress - (int)pt;
      }

    MemoryFrame getMemory() const;

    static int getPageSize();                   // allocation size snapping

  private:
    SharedMemory(SharedMemory&);                // no copying

    HANDLE mappingObjectHandle;
    void *bufferBase;
    unsigned int bufferSize;
  };
}