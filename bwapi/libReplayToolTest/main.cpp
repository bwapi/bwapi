#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ReplayTool.h"
#include "BWAPI.h"
# include <tchar.h>

int _tmain(int argc, TCHAR** argv) 
{
  ReplayTool::init();
  // Since Google Mock depends on Google Test, InitGoogleMock() is
  // also responsible for initializing Google Test.  Therefore there's
  // no need for calling testing::InitGoogleTest() separately.
  testing::InitGoogleMock(&argc, argv);
  
  return RUN_ALL_TESTS();
}