#pragma once
////////////////////////////////////////////////////////////////////////
//  Used to connect SharedMemory in different processes               //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { struct SharedMemoryExport; }

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace Util
{
  struct SharedMemoryExport
  {
    friend class SharedMemory;

    SharedMemoryExport();
    bool isValid() const;

  private:
    HANDLE targetProcessMappingObjectHandle;
    unsigned int bufferSize;
    bool readOnly;
  };
}
