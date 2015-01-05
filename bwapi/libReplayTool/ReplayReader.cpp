#include "ReplayReader.h"
#include <cstdio>

using namespace BWAPI;
using namespace ReplayTool;

ReplayReader::ReplayReader(DWORD dwSize)
  : pActionsBegin( (BYTE*)malloc(dwSize) )
  , pActionsEnd( pActionsBegin == nullptr ? nullptr : pActionsBegin + dwSize  )
  , pCurrent( pActionsBegin )
  , bFrameSize(0)
  , bFrameBytesRead(0)
  , dwCurrentFrameTick(0)
  , dwHighestFrameTick(0)
  , end(false)
  , validFrame(false)
{
}

ReplayReader::~ReplayReader()
{
  if ( this->pActionsBegin != nullptr )
  {
    free(this->pActionsBegin);
    this->pActionsBegin = nullptr;
  }
}

ReplayReader::operator void *() const
{
  return this->pActionsBegin;
}

bool ReplayReader::isGood() const
{
  return this->pActionsBegin != nullptr && this->end == false;
}

size_t ReplayReader::size() const
{
  return this->pActionsEnd - this->pActionsBegin;
}

void ReplayReader::readData(void* data, size_t size)
{
  // The special case when the action buffer is empty
  if (this->size() == 0)
  {
    this->end = true;
  }
  else if (this->pCurrent + size <= pActionsEnd)
  {
    memcpy(data, this->pCurrent, size);
    this->pCurrent += size;
    this->bFrameBytesRead += size;

    if ( this->bFrameBytesRead >= this->bFrameSize )
      this->validFrame = false;

    if (this->pCurrent >= this->pActionsEnd)
      this->end = true;
  }
  else
  {
    this->end = true;
  }
};

void ReplayReader::newFrame()
{
  // Read the frame time
  DWORD dwNewFrameTick = this->readDWORD();
  // Log an error message if the frame is impossible
  if ( dwNewFrameTick < this->dwCurrentFrameTick )
  {
    //this->log("#### ERROR: Frame count has decreased ( %u < %u ) ####\n", dwNewFrameTick, this->dwCurrentFrameTick);
  }
  // store the current frame tick
  this->dwCurrentFrameTick = dwNewFrameTick;
  if ( dwNewFrameTick > this->dwHighestFrameTick )  // log the highest frame tick
    this->dwHighestFrameTick = dwNewFrameTick;

  // Read the frame byte count
  this->bFrameSize = this->readBYTE();
  this->bFrameBytesRead = 0;

  // Set as valid frame
  this->validFrame = true;
}

std::string ReplayReader::readCString()
{
  if ( !*this )
    return "";

  std::string str;

  char *pStart = (char*)this->pCurrent;
  while ( this->readBYTE() != '\0' )
  {}

  str.assign(pStart, (char*)this->pCurrent - pStart);
  return str;
}

bool ReplayReader::isValidFrame() const
{
  return this->validFrame;
}

DWORD ReplayReader::highestFrameTick() const
{
  return this->dwHighestFrameTick;
}

