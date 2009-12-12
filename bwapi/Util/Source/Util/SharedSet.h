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

      static const Index Invalid;   // instantiation further down
      bool isValid()
      {
        return this->blockIndex >= 0;
      }
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
      if(this->nextNewBlockSize <= 0)
      {
        return Index::Invalid;
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
        return Index::Invalid;
      }
      this->ownedBlocks.push_back(newBlock);
      this->nextNewBlockSize = (int)(this->nextNewBlockSize * 1.5);

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
    bool isIndexValid(const Index &index)
    {
      if(index.blockIndex < 0 || index.blockIndex >= (int)this->ownedBlocks.size())
        return false;
      Block &block = this->ownedBlocks[index.blockIndex];
      if(index.blockEntryIndex < 0 || index.blockEntryIndex >= block.size)
        return false;
      return true;
    }
    //----------------------- OPERATOR [] ------------------------------
    T& get(Index pointee)
    {
      Entry &targetEntry = getEntryByIndex(pointee);

      return targetEntry.strucure;
    }
    //----------------------- CLEAR ------------------------------------
    void clear()
    {
      for(int i = 0; i < this->ownedBlocks.size(); i++)
      {
        Block &block = this->ownedBlocks[i];
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
      block.size = block.memory->getMemory().size() / sizeof(T);
      this->ownedBlocks.push_back(block);

      return true;
    }
    //----------------------- BEGIN ------------------------------------
    Index begin()
    {
      if(ownedBlocks.size() <= 0)
        return Index::Invalid;
      Index retval = {0, 0};
      if(isIndexValid(retval))
        return retval;
      return getNext(retval);
    }
    //----------------------- GET NEXT ---------------------------------
    Index getNext(Index index)
    {
      while(index.blockIndex < (int)ownedBlocks.size())
      {
        Block &block = ownedBlocks[index.blockIndex];
        while(index.blockEntryIndex < block.size)
        {
          index.blockEntryIndex++;
          if(getEntryByIndexAndBlock(index.blockEntryIndex, block).busy)
            return index;
        }
        index.blockEntryIndex = 0;
        index.blockIndex++;
      }
      return Index::Invalid;
    }
    //----------------------- ------------------------------------------
  private:
    //----------------------- --------------------------------------------
    struct Block
    {
      Util::SharedMemory* memory;
      int size;   // measured in Entry count
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
    int nextNewBlockSize; // in Entry count
    int exportedBlocks;
    bool exportReadOnly;
    bool exportHasCleared;
    std::vector<Block> ownedBlocks;
    //----------------------- GET ENTRY BY INDEX -------------------------
    Entry &getEntryByIndex(Index pointee)
    {
      Block &targetBlock = this->ownedBlocks[pointee.blockIndex];
      MemoryFrame temp=targetBlock.memory->getMemory();
      Entry *targetBlockEntries = temp.beginAs<Entry>();
      return targetBlockEntries[pointee.blockEntryIndex];
    }
    //----------------------- GET ENTRY BY INDEX AND BLOCK ---------------
    Entry &getEntryByIndexAndBlock(int index, Block &block)
    {
      Entry *targetBlockEntries = block.memory->getMemory().beginAs<Entry>();
      return targetBlockEntries[index];
    }
    //----------------------- ------------------------------------------
  };

  template<typename T> const typename SharedSet<T>::Index SharedSet<T>::Index::Invalid = {-1, -1};
}
