#include "stdafx.h"
#include "CppUnitTest.h"

#include <FPSCounter.h>

#include <chrono>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BWAPI;
using namespace std::chrono_literals;

namespace BWAPITest
{
	TEST_CLASS(FPSCounterTest)
	{
	public:
		TEST_METHOD(FPSCounterCtorDefault)
		{
      FPSCounter counter;
      Assert::AreEqual(0, counter.getFps());
      Assert::AreEqual(0.0, counter.getAverageFps());
		}

    TEST_METHOD(FPSCounterGetFPSbasic)
    {
      FPSCounter counter;

      counter.init();
      std::this_thread::sleep_for(1s);
      counter.update(60);

      // Using a tolerance because time-based tests can be unreliable
      Assert::AreEqual(60, counter.getFps(), 1.0);
    }

    TEST_METHOD(FPSCounterGetFPSFastestSpeedAfterOneFrame)
    {
      FPSCounter counter;

      counter.init();
      std::this_thread::sleep_for(42ms);
      counter.update(1);

      // Using a tolerance because time-based tests can be unreliable
      Assert::AreEqual(24, counter.getFps(), 1.0);
    }

    TEST_METHOD(FPSCounterGetAverageFPSConvergesToTarget)
    {
      FPSCounter counter;

      counter.init();
      std::this_thread::sleep_for(42ms);
      counter.update(1);
      auto initialSample = counter.getAverageFps();

      // Need this because the counter only updates every one second
      for (int i = 2; i <= 25; ++i) {
        std::this_thread::sleep_for(42ms);
        counter.update(i);
      }
      auto newSample = counter.getAverageFps();

      Assert::IsTrue(initialSample < newSample && newSample < 24.0);
    }
	};


}