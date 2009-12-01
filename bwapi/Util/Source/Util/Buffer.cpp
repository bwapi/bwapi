#include "Buffer.h"

namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  Buffer::Buffer()
    : bufferBase(0)
    , bufferSize(0)
  {
  }

  Buffer::Buffer(const MemoryFrame &source)
    : bufferBase(0)
    , bufferSize(0)
  {
    this->fromMemory(source);
  }

  Buffer::Buffer(char *c_string)
    : bufferBase(0)
    , bufferSize(0)
  {
    // copy, including terminating null character
    this->fromMemory(MemoryFrame(c_string, strlen(c_string)+1));
  }

  Buffer::Buffer(const Buffer &idol)
    : bufferBase(0)
    , bufferSize(0)
  {
    this->fromMemory(idol.getMemory());
  }

  Buffer::~Buffer()
  {
    release();
  }
  //----------------------- OPERATOR = ---------------------------------
  Buffer& Buffer::operator = (const Buffer& operand)
  {
    Buffer buffer = Buffer(operand);
    this->swipe(buffer);
    return *this;
  }
  //----------------------- SET SIZE -----------------------------------
  void Buffer::setSize(unsigned int size)
  {
    if(!size)
    {
      if(this->bufferBase)
      {
        free(this->bufferBase);
        this->bufferBase = NULL;
      }
    }
    else
    {
      if(!this->bufferBase)
      {
        this->bufferBase = malloc(size);
      }
      else
      {
        this->bufferBase = realloc(this->bufferBase, size);
      }
    }
    this->bufferSize = size;
  }
  //----------------------- RELEASE ------------------------------------
  void Buffer::release()
  {
    if(this->bufferBase)
    {
      free(this->bufferBase);
    }
    this->bufferBase = 0;
    this->bufferSize = 0;
  }
  //----------------------- EXCHANGE -----------------------------------
  void Buffer::exchange(Buffer &target)
  {
    void *tempBase = target.bufferBase;
    unsigned int tempSize = target.bufferSize;
    target.bufferBase = this->bufferBase;
    target.bufferSize = this->bufferSize;
    this->bufferBase = tempBase;
    this->bufferSize = tempSize;
  }
  //----------------------- SWIPE --------------------------------------
  void Buffer::swipe(Buffer &target)
  {
    release();
    this->bufferBase = target.bufferBase;
    this->bufferSize = target.bufferSize;
    target.bufferBase = 0;
    target.bufferSize = 0;
  }
  //----------------------- FROM MEMORY --------------------------------
  void Buffer::fromMemory(const MemoryFrame &source)
  {
    release();
    if(source.isEmpty())
    {
      return;
    }
    setSize(source.size());
    memcpy(this->bufferBase, source.begin(), this->bufferSize);
  }
  //----------------------- APPEND MEMORY ------------------------------
  void Buffer::append(const MemoryFrame &source)
  {
    int middle = this->bufferSize;
    this->setSize(this->bufferSize + source.size());
    memcpy(this->getMemory().offset<void>(middle), source.begin(), source.size());
  }
  //----------------------- APPEND SPACE -------------------------------
  void *Buffer::appendBytes(unsigned int byteCount)
  {
    void *retval = (void*)((int)this->bufferBase + this->bufferSize);
    setSize(this->bufferSize + byteCount);
    return retval;
  }
  //----------------------- GET SIZE -----------------------------------
  unsigned int Buffer::size() const
  {
    if(this->bufferBase)
      return this->bufferSize;
    return 0;
  }
  //----------------------- IS EMPTY -----------------------------------
  bool Buffer::isEmpty() const
  {
    return !this->bufferBase || this->bufferSize == 0;
  }
  //----------------------- GET BOUND ----------------------------------
  const MemoryFrame Buffer::getMemory() const
  {
    return MemoryFrame(this->bufferBase, this->bufferSize);
  }
  MemoryFrame Buffer::getMemory()
  {
    return MemoryFrame(this->bufferBase, this->bufferSize);
  }
  //----------------------- --------------------------------------------
}
