#ifndef LTIMER_HPP
#define LTIMER_HPP
#include <time.h>
#include <SDL.h>
#include <stdlib.h>

/**
 * A class for a Timer, taken from LazyFoo tutorials.
 * Has functionality to start, stop, pause, unpause, check
 * if started or paused, and get the ticks since the timer
 * started.
 */
class LTimer {
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();

	void stop();

	void pause();

	void unpause();

	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();

	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

#endif
