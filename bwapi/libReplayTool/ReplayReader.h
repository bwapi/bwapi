#pragma once

#include "AbstractReplayReader.h"

namespace ReplayTool
{
  class ReplayReaderWrap;

  class ReplayReader : public AbstractReplayReader
  {
  public:
    ReplayReader(DWORD dwSize);
    ~ReplayReader();

    // Used to manipulate raw data
    operator void*() const;

    // Check if data was allocated correctly
    bool isGood() const;

    // Read a chunk of data
    void readData(void* data, size_t size);

    // Get the memory block size
    size_t size() const;

    // Begin reading a new frame
    void newFrame();

    // Check if the current frame is still valid
    bool isValidFrame() const;

    // Retrieve highest frame count
    DWORD highestFrameTick() const;

    std::string readCString();

  private:
    // Memory location pointers
    BYTE *pActionsBegin;
    BYTE *pActionsEnd;
    BYTE *pCurrent;


    // Frame bytes
    BYTE bFrameSize;
    BYTE bFrameBytesRead;

    // Frame time
    DWORD dwCurrentFrameTick;
    DWORD dwHighestFrameTick;

    // Flags
    bool end;
    bool validFrame;
  };
}