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
	};


}