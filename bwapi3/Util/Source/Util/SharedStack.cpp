#include "SharedStack.h"
#include "ForEach.h"
namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  SharedStack::SharedStack()
    : exportedBlocks(0)
    , nextNewBlockSize(0)
    , exportHasCleared(true)
  {
  }

  SharedStack::~SharedStack()
  {
    this->release();
    this->ownedBlocks.clear();
  }
  //----------------------- ALLOCATE -----------------------------------
  void SharedStack::init(int blockSize, bool exportReadOnly)
  {
    this->nextNewBlockSize = blockSize;
    this->exportReadOnly = exportReadOnly;
    this->_createNewPageBlock();
  }
  //----------------------- INSERT BYTES -------------------------------
  SharedStack::Index SharedStack::insertBytes(int byteCount)
  {
    Block *targetBlock = NULL;
    unsigned int b;
    for(b = 0; b < this->ownedBlocks.size(); b++)
    {
      Block &block = this->ownedBlocks[b];

      // check if there's a free spot in this block
      if(block.head + byteCount + sizeof(EntryHead)*2
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
      if(byteCount + sizeof(EntryHead)*2 > (unsigned)this->nextNewBlockSize)
        throw GeneralException(__FUNCTION__ ": pushed memory is too huge");

      // create new block
      this->_createNewPageBlock();

      // return this newly created, last, block
      b = this->ownedBlocks.size()-1;
      targetBlock = &this->ownedBlocks[b];
    }

    // insertion
    Block &block = *targetBlock;

    // get memory of next empty space
    Util::MemoryFrame memory = block.memory->getMemory();
    memory = memory.getSubFrameByLimits(block.head, memory.size());

    // write the head
    EntryHead entry;
    entry.size = byteCount;
    memory.writeAs(entry);

    // store the new entry here
    memory.skip(byteCount);

    // mark next entry as final (null entry/null head)
    entry.size = 0;
    memory.writeAs(entry);

    // return the process-independent index to this entry
    Index index;
    index.blockIndex = b;
    index.blockHead = block.head;

    // update modified block's stats
    block.head += byteCount + sizeof(EntryHead);

    return index;
  }
  //----------------------- INSERT -------------------------------------
  SharedStack::Index SharedStack::insert(const Util::MemoryFrame &storee)
  {
    // reserve space
    Index i = this->insertBytes(storee.size());
    if(!i.isValid())
      return Index::invalid;
    Util::MemoryFrame memory = this->get(i);

    // store the new entry here
    memory.write(storee);

    return i;
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
    foreach(const Block &block, this->ownedBlocks)
    {
      delete block.memory;
    }
    this->ownedBlocks.clear();
    this->exportedBlocks = 0;
  }
  //----------------------- IS UPDATE EXPORT NEEDED ------------------
  bool SharedStack::isUpdateExportNeeded() const
  {
    return (unsigned)this->exportedBlocks < this->ownedBlocks.size();
  }
  //----------------------- EXPORT NEXT ------------------------------
  void SharedStack::exportNextUpdate(Export &out, RemoteProcess &targetProcess)
  {
    if(this->exportedBlocks >= (int)this->ownedBlocks.size())
      throw GeneralException("nothing more to export");
    out = this->ownedBlocks[this->exportedBlocks].memory->exportToProcess(targetProcess, this->exportReadOnly);
    this->exportedBlocks++;
  }
  //----------------------- IMPORT NEXT ------------------------------
  void SharedStack::importNextUpdate(const Export &in)
  {
    // import the new SharedMemory
    Block block;
    block.memory = new SharedMemory();
    block.memory->import(in);

    block.size = block.memory->getMemory().size();
    this->ownedBlocks.push_back(block);
  }
  //----------------------- BEGIN --------------------------------------
  SharedStack::Index SharedStack::begin() const
  {
    // any blocks we have?
    if(this->ownedBlocks.size() <= 0)
      return Index::invalid;
    // first index
    Index index;
    index.blockIndex = 0;
    index.blockHead = 0;
    // check if the first block is not actually empty
    if(this->ownedBlocks[0].memory->getMemory().getAs<int>() == 0)
      return Index::invalid;
    return index;
  }
  //----------------------- GET ----------------------------------------
  Util::MemoryFrame SharedStack::get(Index index) const
  {
    if(index.blockIndex >= (int)this->ownedBlocks.size())
      return Util::MemoryFrame();
    Util::MemoryFrame memory = this->ownedBlocks[index.blockIndex].memory->getMemory();
    memory.skip(index.blockHead);
    EntryHead entry = memory.readAs<EntryHead>();
    return memory.getSubFrame(0, entry.size);
  }
  //----------------------- GET NEXT -----------------------------------
  SharedStack::Index SharedStack::getNext(Index index) const
  {
    // get this entry
    Util::MemoryFrame memory = this->ownedBlocks[index.blockIndex].memory->getMemory();
    memory.skip(index.blockHead);
    EntryHead entry = memory.readAs<EntryHead>();

    // shift to next address
    index.blockHead += sizeof(EntryHead);
    index.blockHead += entry.size;
    if(index.blockHead >= this->ownedBlocks[index.blockIndex].size) // out of range, weird error, shouldn't have happened
      return Index::invalid;

    // get next entry
    memory = this->ownedBlocks[index.blockIndex].memory->getMemory();
    memory.skip(index.blockHead);
    entry = memory.readAs<EntryHead>();
    while(entry.size == 0) // last entry in this block
    {
      if(index.blockIndex == (int)this->ownedBlocks.size()-1) // last block
        return Index::invalid;

      // get the begin of next block
      index.blockIndex++;
      index.blockHead = 0;
      memory = this->ownedBlocks[index.blockIndex].memory->getMemory();
      entry = memory.readAs<EntryHead>();
    }
    return index;
  }
  //----------------------- --------------------------------------------
  //----------------------- CREATE NEW PAGE BLOCK ----------------------
  void SharedStack::_createNewPageBlock()
  {
    // create new Block with SharedMemory in it
    Block newBlock;
    newBlock.size = this->nextNewBlockSize;
    newBlock.head = 0;
    newBlock.memory = new SharedMemory();
    newBlock.memory->create(this->nextNewBlockSize);
    this->ownedBlocks.push_back(newBlock);

    // next block gets created bigger
    // self-compensating approach during memory drainage
    this->nextNewBlockSize = (int)(this->nextNewBlockSize * 1.75);
  }
  //----------------------- --------------------------------------------
  //----------------------- INDEX IS VALID -----------------------------
  bool SharedStack::Index::isValid()
  {
    return this->blockIndex >= 0;
  }
  const SharedStack::Index SharedStack::Index::invalid = {-1, -1};
  //----------------------- --------------------------------------------
}
