#pragma once

#include "gmock/gmock.h"

#include <BWAPI/Client/Client.h>
#include <BWAPI/UnitType.h>

class MockClient : public BWAPI::Client
{
public:
  MOCK_CONST_METHOD0(isConnected, bool());
  MOCK_METHOD0(connect, bool());
  MOCK_METHOD0(disconnect, void());

  MOCK_METHOD1(update, void(BWAPI::Game&));

  MOCK_METHOD2(setScreenPosition, void(int, int));
  MOCK_METHOD2(pingMinimap, void(int, int));
  MOCK_METHOD10(drawShape, void(BWAPI::ShapeType::Enum, BWAPI::CoordinateType::Enum, int, int, int, int, int, int, BWAPI::Color, bool));
  MOCK_METHOD5(drawText, void(BWAPI::CoordinateType::Enum, const std::string&, int, int, int));
  MOCK_METHOD2(sendText, void(const std::string&, bool));
  MOCK_METHOD1(printText, void(const std::string&));
  MOCK_METHOD0(pauseGame, void());
  MOCK_METHOD0(resumeGame, void());
  MOCK_METHOD0(leaveGame, void());
  MOCK_METHOD0(restartGame, void());
  MOCK_METHOD1(setLocalSpeed, void(int));
  MOCK_METHOD2(issueCommand, void(const BWAPI::Unitset&, BWAPI::UnitCommand));
  MOCK_METHOD2(setAlliance, void(int, int));
  MOCK_METHOD2(setVision, void(int, bool));

  MOCK_METHOD5(createUnit, void(int, BWAPI::UnitType, int, int, int));
  MOCK_METHOD2(killUnits, void(const BWAPI::Unitset&, bool));
};
