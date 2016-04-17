#include "stdafx.h"
#include "CppUnitTest.h"

#include <APMCounter.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BWAPI;

namespace BWAPITest
{
  TEST_CLASS(APMCounterTest)
  {
  public:
    TEST_METHOD(APMCounterCtorDefault)
    {
      APMCounter counter;
      Assert::AreEqual(0, counter.apm(true));
      Assert::AreEqual(0, counter.apm(false));
    }
    TEST_METHOD(APMCounterReflects204APM)
    {
      APMCounter counter;
      counter.init();

      // Send a command every 7th frame for 50 frames.
      for (int frame = 1; frame < 50; ++frame)
      {
        if (frame % 7 == 0) counter.addSelect();
        counter.update(frame);
      }

      // If you send a command every 7th frame for a full minute on fastest speed:
      // (60000ms / 42ms/frame = 1428.57.. frames) / 7frames/action = 204.08.. actions in a minute
      Assert::AreEqual(204, counter.apm());
    }
  };
}
