#pragma once

#include "gmock\gmock.h"
#include "AbstractReplayReader.h"

class MockReplayReader : public ReplayTool::AbstractReplayReader
{
public:
  void readData(void* data, size_t size) {}

  MOCK_METHOD0(readCString, std::string());

  // Begin reading a new frame
  MOCK_METHOD0(newFrame, void());

  // Check if the current frame is still valid
  MOCK_CONST_METHOD0(isValidFrame, bool());

  // Retrieve highest frame count
  MOCK_CONST_METHOD0(highestFrameTick, DWORD());

  // Check if data was allocated correctly
  MOCK_CONST_METHOD0(isGood, bool());

  MOCK_METHOD0(readDWORD, DWORD());

  MOCK_METHOD0(readWORD, WORD());

  MOCK_METHOD0(readBYTE, BYTE());

  MOCK_METHOD0(readUnitType, BWAPI::UnitType());

  MOCK_METHOD0(readOrder, BWAPI::Order());

  MOCK_METHOD0(readTechType, BWAPI::TechType());

  MOCK_METHOD0(readUpgradeType, BWAPI::UpgradeType());
};