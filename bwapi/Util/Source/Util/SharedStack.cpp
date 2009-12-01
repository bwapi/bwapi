#include "SharedStack.h"

namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  SharedStack::SharedStack(int firstBlockSize, bool exportReadOnly)
    : exportedBlocks(0)
    , nextNewBlockSize(firstBlockSize)
    , exportReadOnly(exportReadOnly)
    , exportHasCleared(true)
  {
  }

  SharedStack::~SharedStack()
  {
    this->release();
    this->ownedBlocks.clear();
  }
  //----------------------- insert -------------------------------------
  SharedStack::Index SharedStack::insert(const Util::MemoryFrame &storee)
  {
    Block *targetBlock = NULL;
    unsigned int b;
    for(b = 0; b < this->ownedBlocks.size(); b++)
    {
      Block &block = this->ownedBlocks[b];

      // check if there's a free spot in this block
      if(block.head + storee.size() + sizeof(EntryHead)*2
        <= (unsigned)block.size)
      {
        targetBlock = &block;
        break;
      }
    }

    // if we didn't sind a spot
    if(!targetBlock)
    {
      // maybe storee is just too fat!
      if(storee.size() + sizeof(EntryHead)*2 > (unsigned)this->nextNewBlockSize)
      {
        return Index::invalid;
      }

      // create new block
      Block newBlock;
      newBlock.size = this->nextNewBlockSize;
      newBlock.head = 0;
      newBlock.memory = new SharedMemory();
      if(!newBlock.memory->create(this->nextNewBlockSize))
      {
        return Index::invalid;
      }
      this->ownedBlocks.push_back(newBlock);

      targetBlock = &this->ownedBlocks[b = this->ownedBlocks.size()-1];

      // next block gets created bigger
      // self-compensating approach during memory drainage
      this->nextNewBlockSize = (int)(this->nextNewBlockSize * 1.75);
    }

    // insertion
    Block &block = *targetBlock;

    // get memory of next empty space
    Util::MemoryFrame memory = block.memory->getMemory();
    memory = memory.getSubFrameByLimits(block.head, memory.size());

    // write the head
    EntryHead entry;
    entry.size = storee.size();
    memory.writeAs(entry);

    // store the new entry here
    memory.write(storee);

    // mark next entry as final (null entry/null head)
    entry.size = 0;
    memory.writeAs(entry);

    // return the process-independent index to this entry
    Index index;
    index.blockIndex = b;
    index.blockHead = block.head;

    // update modified block's stats
    block.head += storee.size() + sizeof(EntryHead);

    return index;
  }
  //----------------------- CLEAR ------------------------------------
  void SharedStack::clear()
  {
    this->exportHasCleared = true;
    for(unsigned int b = 0; b < this->ownedBlocks.size(); b++)
    {
      Block &block = this->ownedBlocks[b];

      // mark every first header as last
      EntryHead &firstEntry = block.memory->getMemory().getAs<EntryHead>();
      firstEntry.size = 0;

      // reset stats
      block.head = 0;
    }
  }
  //----------------------- RELEASE ----------------------------------
  void SharedStack::release()
  {
    for each(const Block &block in this->ownedBlocks)
    {
      delete block.memory;
    }
    this->ownedBlocks.clear();
    this-> exportedBlocks = 0;
  }
  //----------------------- IS UPDATE EXPORT NEEDED ------------------
  bool SharedStack::isUpdateExportNeeded() const
  {
    return exportHasCleared ||
      (unsigned)this->exportedBlocks < this->ownedBlocks.size();
  }
  //----------------------- ASSEMBLE UPDATE EXPORT PACKET ------------
  bool SharedStack::assembleUpdateExportPacket(Buffer &dest, RemoteProcess &target)
  {
      dest.release();

      dest.appendStructure(this->exportHasCleared);
      this->exportHasCleared = false;
      for(unsigned int i = exportedBlocks; i < this->ownedBlocks.size(); i++)
      {
        ExportBlockEntry &entry = dest.appendStructure<ExportBlockEntry>();
        entry.size =    this->ownedBlocks[i].size;
        entry.memory =  this->ownedBlocks[i].memory->exportToProcess(target, this->exportReadOnly);
        if(!entry.memory.isValid())
          return false;
      }
      this->exportedBlocks = this->ownedBlocks.size();
      return true;
    }
    //----------------------- IMPORT UPDATE PACKET ---------------------
    bool SharedStack::importUpdatePacket(const MemoryFrame &packetMemory)
    {
      MemoryFrame data = packetMemory;

      bool cleared;
      if(!data.readTo(cleared))
      {
        return false;
      }
      // TODO: something...

      if(!data.isMultipleOf<ExportBlockEntry>())
      {
        return false;
      }
      
      while(!data.isEmpty())
      {
        ExportBlockEntry &entry = data.readAs<ExportBlockEntry>();

        // import the new SharedMemory
        Block block;
        block.memory = new SharedMemory();
        if(!block.memory->import(entry.memory))
          return false;
        block.size = entry.size;
        this->ownedBlocks.push_back(block);
      }

      return true;
  }
  //----------------------- INDEX IS VALID -----------------------------
  bool SharedStack::Index::isValid()
  {
    return this->blockIndex >= 0;
  }
  SharedStack::Index SharedStack::Index::invalid = {-1, -1};
  //----------------------- --------------------------------------------
}
