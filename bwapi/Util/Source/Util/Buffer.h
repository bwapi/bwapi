#pragma once
////////////////////////////////////////////////////////////////////////
//  Untyped memory allocator and container                            //
//                                                                    //
//  Use this class instead of allocating memory on your own, brings   //
//  you one huge step towards RAII.                                   //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { class Buffer; }

#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "MemoryFrame.h"

namespace Util
{
  class Buffer
  {
  public:
    Buffer();
    Buffer(const MemoryFrame &source);
    Buffer(char *c_string);
    Buffer(const Buffer& idol);
    ~Buffer();

    void setSize(unsigned int size);      // preserves data, if not prior released
    void release();                       // frees memory
    void exchange(Buffer &target);        // exchange memory without copying
    void swipe(Buffer &target);           // frees own memory, leaves target empty handed
    void fromMemory(const MemoryFrame &);
    void append(const MemoryFrame &);
    void *appendBytes(unsigned int byteCount);
    template <typename T>
      void appendStructure(T &source)     // just a wrapper
      {
        append(Util::MemoryFrame::from(source));
      }
    template <typename T>
      T& appendStructure()                // another wrapper. very handy
      {
        T &retval = *(T*)appendBytes(sizeof(T));
        return retval;
      }
    Buffer &operator = (const Buffer &);  // byte per byte copy

    unsigned int size() const;
    bool isEmpty() const;                 // same as getSize() == 0
    const MemoryFrame getMemory() const;
    MemoryFrame getMemory();

  private:
    void *bufferBase;
    unsigned int bufferSize;
  };
}
