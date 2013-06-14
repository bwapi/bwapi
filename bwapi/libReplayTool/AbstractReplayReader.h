#pragma once

#include <Windows.h>
#include <cstdio>
#include <string>
#include "BWAPI.h"

namespace ReplayTool
{
  class AbstractReplayReader
  {
  public:
    virtual void readData(void* data, size_t size) = 0;

    virtual std::string readCString() = 0;

    // Begin reading a new frame
    virtual void newFrame() = 0;

    // Check if the current frame is still valid
    virtual bool isValidFrame() const = 0;

    // Retrieve highest frame count
    virtual DWORD highestFrameTick() const = 0;

    // Check if data was allocated correctly
    virtual bool isGood() const = 0;

    template<class T>
    T read()
    {
      T val;
      readData(&val, sizeof(val));
      return val;
    }

    virtual DWORD readDWORD() { return read<DWORD>(); }

    virtual WORD readWORD() { return read<WORD>(); }

    virtual BYTE  readBYTE() { return read<BYTE>(); }

    virtual BWAPI::UnitType readUnitType() { return BWAPI::UnitType(this->read<WORD>()); }

    virtual BWAPI::Order readOrder() { return BWAPI::Order(this->read<BYTE>()); }

    virtual BWAPI::TechType readTechType() { return BWAPI::TechType(this->read<BYTE>()); }

    virtual BWAPI::UpgradeType readUpgradeType() { return BWAPI::UpgradeType(this->read<BYTE>()); }

    virtual ~AbstractReplayReader() {}
  };
}