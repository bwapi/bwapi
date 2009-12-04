#pragma once
/***********************************************************************
 *  Is basically a Buffer
 *  Unitfies a static sized Header with a dynamic sized Tail array in one
 *  flexible constant time access structure
 *
 *  Memory map:
 *    HeadType head;
 *    int tailCount;
 *    TailType entries[tailCount];
 *
 *  declare a "static const int Id" in HeadType for typed packet/message transfers
 *******/

#include "Buffer.h"
#include "MemoryFrame.h"

namespace Util
{
  template<typename H, typename T> class Hybrid;
  template<typename H, typename T> class HybridFrame;

  template<typename H, typename T> class Hybrid
  {
  private:
    Util::Buffer buffer;

  public:
    typedef H HeadType;
    typedef T TailType;

    struct Empty
    {
    };

    Hybrid()
    {
    }
    void release()
    {
      release();
    }
    H &makeHead()
    {
      release();
      H head;
      buffer.appendStructure<H>(head);
      int null = 0;
      buffer.appendStructure<int>(null);
      return buffer.getMemory().getAs<H>();
    }
    void appendEntry(const T &entry)
    {
      buffer.appendStructure<T>(entry);
      // increment entry count
      buffer.getMemory().offsetAs<int>(sizeof(H))++;
    }
    HybridFrame<H, T> getFrame()
    {
      return HybridFrame<H,T>(buffer.getMemory());
    }
    MemoryFrame getMemory()
    {
      return this->buffer.getMemory();
    }
  };

/***********************************************************************
 *  Is basically a MemoryFrame
 *  Like the MemoryFrame and Buffer concept, this behaves like a view
 *  to a Hybrid
 *******/

  template<typename H, typename T> class HybridFrame
  {
  private:
    Util::MemoryFrame frame;

  public:
    typedef H HeadType;
    typedef T TailType;

    H &head;
    int &count;
    T *tail;

    HybridFrame(const Util::MemoryFrame &source)
      : frame(source)
      , head(source.getAs<H>())
      , count(source.offsetAs<int>(sizeof(H)))
      , tail(source.offset<T>(sizeof(H)+sizeof(int)))
    {
    }
    MemoryFrame getMemory()
    {
      return this->frame;
    }
  };
}
