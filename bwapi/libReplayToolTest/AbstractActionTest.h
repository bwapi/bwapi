#pragma once

#include "gtest\gtest.h"
#include "GameAction.h"

typedef ReplayTool::GameAction* CreateAction();

template<CreateAction T>
class AbstractActionTest : public testing::Test
{
public:
  void SetUp() { action = T(); }
  void TearDown() { delete action; }

protected:
  virtual void testToString() = 0;
  virtual void testRead() = 0;

  ReplayTool::GameAction *action;
};