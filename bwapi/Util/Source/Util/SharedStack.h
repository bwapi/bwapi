#pragma once
////////////////////////////////////////////////////////////////////////
//  Manages SharedMemories and distributes stored data among them     //
//  - provides export packets to keep the other side on track         //
//  - does not release any memory untill destoyed                     //
//  - provides a process independent index                            //
//  - can be iterated through, nonstandart iterator                   //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { class SharedStack; }

#include "Buffer.h"
#include "MemoryFrame.h"
#include "RemoteProcess.h"
#include "SharedMemory.h"
#include <set>
#include <vector>

namespace Util
{
  class SharedStack
  {
  public:
    //----------------------- TYPES ------------------------------------
    struct Index
    {
      int blockIndex;
      int blockHead;

      bool isValid();
      static Index invalid;
    };
    typedef SharedMemory::Export Export;
    //----------------------- CONSTRUCTION -----------------------------
    SharedStack();
    ~SharedStack();
    //----------------------- METHODS ----------------------------------
    bool init(int blockSize, bool exportReadOnly);
    Index insert(const Util::MemoryFrame &storee);
    void clear();
    void release();   // forces to release all memory blocks. Not exportable
    bool isUpdateExportNeeded() const;
    bool exportNextUpdate(Export &out, RemoteProcess &targetProcess);
    bool importNextUpdate(const Export &in);
    //----------------------- ------------------------------------------
  private:
    struct Block
    {
      Util::SharedMemory* memory;
      int size;
      int head;   // writing head
    };
    struct EntryHead
    {
      int size;
      // size bytes after
    };
    struct ExportBlockEntry
    {
      unsigned int size;
      SharedMemory::Export memory;
    };
    int nextNewBlockSize;
    int exportedBlocks;
    bool exportReadOnly;
    bool exportHasCleared;
    std::vector<Block> ownedBlocks;

    // helpers
    bool _createNewPageBlock();
  };
}
