#pragma once
////////////////////////////////////////////////////////////////////////
//  Manages SharedMemories and distributes stored data among them     //
//  - provides export packets to keep the other side on track         //
//  - does not release any memory untill destoyed                     //
//  - provides a process independent index                            //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { template <typename T> class SharedSet; }

#include "Buffer.h"
#include "RemoteProcess.h"
#include "SharedMemory.h"
#include <set>
#include <vector>
#include "ForEach.h"

namespace Util
{
  template <typename T> class SharedSet
  {
  public:
    //----------------------- TYPES ------------------------------------
    struct Index
    {
      int blockIndex;
      int blockEntryIndex;
    };
    typedef SharedMemory::Export Export;
    //----------------------- CONSTRUCTION -----------------------------
    SharedSet()           // for import only
      : nextNewBlockSize(0)
    {
    }
    ~SharedSet()
    {
      foreach(Block block, this->ownedBlocks)
      {
        delete block.memory;
      }
      this->ownedBlocks.clear();
    }
    //----------------------- INIT -------------------------------------
    bool init(int blockSize, bool exportReadOnly)
    {
      this->nextNewBlockSize = blockSize;
      this->exportReadOnly = exportReadOnly;
      return true;
    }
    //----------------------- INSERT -----------------------------------
    Index insert(T &storee)
    {
      if(this->newBlockSize <= 0)
      {
        Index index = {-1, -1};
        return index;
      }
      for(unsigned int b = 0; b < this->ownedBlocks.size(); b++)
      {
        Block &block = this->ownedBlocks[b];

        // check if there's a free spot in this block
        if(block.count == block.size)
        {
          continue;
        }

        // find the free spot
        Util::MemoryFrame blockMemory = block.memory->getMemory();
        Entry *entries = blockMemory.beginAs<Entry>();
        int i = block.head;
        while(entries[i].busy && i < block.size)
          i++;
        if(i == block.size)
        {
          // could not find the free spot.
          // this is actually a logical error and should never happen
          block.count = block.size;
          continue;
        }

        // store the new entry here
        entries[i].busy = true;
        entries[i].strucure = storee;

        // update block stats
        block.head = i;
        block.count++;

        // return the process-independent index to this entry
        Index index;
        index.blockIndex = b;
        index.blockEntryIndex = i;
        return index;
      }

      // no free spots in any block
      // allocate new block
      Block newBlock;
      newBlock.size = this->nextNewBlockSize;
      newBlock.count = 1;
      newBlock.head = 1;
      newBlock.memory = new SharedMemory();
      if(!newBlock.memory->create(this->nextNewBlockSize * sizeof(T)))
      {
        Index index = {-1, -1};
        return index;
      }
      this->ownedBlocks.push_back(newBlock);
      this->nextNewBlockSize *= 1.5;

      // clean the new entries
      MemoryFrame temp=newBlock.memory->getMemory();
      Entry *entries = temp.beginAs<Entry>();
      for(int i = 0; i < newBlock.size; i++)
      {
        entries[i].busy = false;
      }

      // fill in the first entry
      entries[0].strucure = storee;

      // return the process-independent index to this entry
      Index index;
      index.blockIndex = this->ownedBlocks.size()-1;
      index.blockEntryIndex = 0;
      return index;
    }
    //----------------------- REMOVE -----------------------------------
    void remove(Index removee)
    {
      // mark target entry as unused
      Block &targetBlock = this->ownedBlocks[removee.blockIndex];
      MemoryFrame temp = targetBlock.memory->getMemory();
      Entry *targetBlockEntries = temp.beginAs<Entry>();
      targetBlockEntries[removee.blockEntryIndex].busy = false;

      // move head to new position
      if(removee.blockEntryIndex < targetBlock.head)
      {
        targetBlock.head = removee.blockEntryIndex;
      }
    }
    //----------------------- IS VALID INDEX ---------------------------
    bool isValidIndex(const Index &index)
    {
      if(index.blockIndex < 0 || index.blockIndex >= this->ownedBlocks.size())
        return false;
      Block &block = this->ownedBlocks[index.blockIndex];
      if(index.blockEntryIndex < 0 || index.blockEntryIndex >= block.size)
        return false;
      return true;
    }
    //----------------------- OPERATOR [] ------------------------------
    T& get(Index pointee)
    {
      Block &targetBlock = this->ownedBlocks[pointee.blockIndex];
      MemoryFrame temp=targetBlock.memory->getMemory();
      Entry *targetBlockEntries = temp.beginAs<Entry>();
      Entry &targetEntry = targetBlockEntries[pointee.blockEntryIndex];

      return targetEntry.strucure;
    }
    //----------------------- CLEAR ------------------------------------
    void clear()
    {
      foreach(Block &block, this->ownedBlocks)
      {
        // mark everything as free
        MemoryFrame temp=block.memory->getMemory();
        Entry *entries = temp.beginAs<Entry>();
        for(int i = 0; i < block.size; i++)
        {
          entries[i].busy = false;
        }

        // init heads
        block.count = 0;
        block.head = 0;
      }
    }
    //----------------------- RELEASE ----------------------------------
    void release()
    {
      foreach(const Block &block, this->ownedBlocks)
      {
        delete block.memory;
      }
      this->ownedBlocks.clear();
    }
    //----------------------- IS UPDATE EXPORT NEEDED ------------------
    bool isUpdateExportNeeded() const
    {
      return this->exportedBlocks < (int)this->ownedBlocks.size();
    }
    //----------------------- EXPORT NEXT ------------------------------
    bool exportNextUpdate(Export &out, RemoteProcess &targetProcess)
    {
      if(this->exportedBlocks >= (int)this->ownedBlocks.size())
        return false;
      out = this->ownedBlocks[this->exportedBlocks].memory->exportToProcess(targetProcess, this->exportReadOnly);
      if(!out.isValid())
        return false;
      this->exportedBlocks++;
      return true;
    }
    //----------------------- IMPORT NEXT ------------------------------
    bool importNextUpdate(const Export &in)
    {
      // import the new SharedMemory
      Block block;
      block.memory = new SharedMemory();
      if(!block.memory->import(in))
        return false;
      block.size = block.memory->getMemory().size();
      this->ownedBlocks.push_back(block);

      return true;
    }
    //----------------------- ------------------------------------------
  private:
    //----------------------- --------------------------------------------
    struct Block
    {
      Util::SharedMemory* memory;
      int size;
      int count;
      int head;   // lowest bound of free slots
    };
    struct Entry
    {
      bool busy;
      T strucure;
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
    //----------------------- ------------------------------------------
  };
}
