#pragma once
////////////////////////////////////////////////////////////////////////
//  SharedMemoryPointer<T> is a T* pointer to inside a SharedMemory   //
//  - SharedMemory instance dependent                                 //
//  - process independent                                             //
//                                                                    //
//  use SharedMemory::unpack()                                        //
////////////////////////////////////////////////////////////////////////

namespace Util
{
  template <typename T> struct SharedMemoryPointer;
}

//#include ".h"

namespace Util
{
  template <typename T> struct SharedMemoryPointer
  {
      friend class SharedMemory;    // structure is part of SharedMemory
  private:
	  int offset;
  };
}
