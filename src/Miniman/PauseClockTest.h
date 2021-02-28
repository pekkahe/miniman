#ifndef PAUSECLOCKTEST_H_
#define PAUSECLOCKTEST_H_

#include "PauseClock.h"

namespace tests
{
	class PauseClockTest
	{
	public:
		PauseClockTest();
		~PauseClockTest();

		void run();

	private:
		void pause();
		void unpause();
		void restart();

	private:
		PauseClock pauseClock_;
	};
}

#endif

