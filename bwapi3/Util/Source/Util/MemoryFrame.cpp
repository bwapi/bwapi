#include "MemoryFrame.h"
#include <stdlib.h>
#include <memory.h>

namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  MemoryFrame::MemoryFrame(void *base, unsigned int size)
    : frameBase(base)
    , frameSize(size)
  {
    if(size < 0)
      this->frameSize = 0;
  }

  MemoryFrame::MemoryFrame()
    : frameBase(NULL)
    , frameSize(0)
  {
  }

  MemoryFrame::~MemoryFrame()
  {
  }
  //----------------------- FILL ---------------------------------------
  void MemoryFrame::fill(unsigned char value)
  {
    ::memset(this->frameBase, value, this->frameSize);
  }
  //----------------------- SUB FRAME BY SIZE --------------------------
  MemoryFrame MemoryFrame::getSubFrame(int from, int size)
  {
    from = _limit(from, 0, this->frameSize);
    size = _limit(size, 0, this->frameSize-from);
    return MemoryFrame(
      (void*)((int)this->frameBase + from),
      size);
  }
  //----------------------- SUB FRAME BY LIMITS ------------------------
  MemoryFrame MemoryFrame::getSubFrameByLimits(int from, int to)
  {
    from = _limit(from, 0, this->frameSize);
    to =   _limit(to, from, this->frameSize);
    return MemoryFrame(
      (void*)((int)this->frameBase + from),
      to - from);
  }
  //----------------------- BEGIN --------------------------------------
  void *MemoryFrame::begin() const
  {
    return frameBase;
  }
  //----------------------- END ----------------------------------------
  void *MemoryFrame::end() const
  {
    return (void*)((int)frameBase+frameSize);
  }
  //----------------------- SIZE ---------------------------------------
  unsigned int MemoryFrame::size() const
  {
    return frameSize;
  }
  //----------------------- IS INSIDE ----------------------------------
  bool MemoryFrame::isInside(void *ptr) const
  {
    return ptr >= begin() && ptr < end();
  }
  //----------------------- IS EMPTY -----------------------------------
  bool MemoryFrame::isEmpty() const
  {
    return this->frameSize <= 0;
  }
  //----------------------- IS MULTIPLE OF -----------------------------
  bool MemoryFrame::isMultipleOf(unsigned int bytes) const
  {
    return (this->frameSize % bytes) == 0;
  }
  //----------------------- OPERATOR == --------------------------------
  bool MemoryFrame::operator == (const MemoryFrame &operand)
  {
    return this->frameBase == operand.frameBase &&
           this->frameSize == operand.frameSize;
  }
  //----------------------- COMPARE BYTES ------------------------------
  bool MemoryFrame::compareBytes(const Util::MemoryFrame &that)
  {
    if(this->frameSize != that.frameSize)
      return false;
    return ::memcmp(this->frameBase, that.frameBase, this->frameSize) == 0;
  }
  //----------------------- SKIP ---------------------------------------
  void MemoryFrame::skip(unsigned int bytes)
  {
    if(bytes > this->frameSize)
      bytes = this->frameSize;
    this->frameBase = (void*)((int)this->frameBase + bytes);
    this->frameSize -= bytes;
    return;
  }
  //----------------------- READ ---------------------------------------
  MemoryFrame MemoryFrame::read(int bytes)
  {
    MemoryFrame retval = this->getSubFrame(0, bytes);
    this->skip(bytes);
    return retval;
  }
  //----------------------- WRITE --------------------------------------
  void MemoryFrame::write(const MemoryFrame &source)
  {
    int bytes = source.size();
    ::memcpy(this->begin(), source.begin(), bytes);
    this->skip(bytes);
  }
  //----------------------- LIMIT --------------------------------------
  int MemoryFrame::_limit(int a, int low, int hi)
  {
    if(a < low)
      return low;
    if(a > hi)
      return hi;
    return a;
  }
  //----------------------- --------------------------------------------
}
