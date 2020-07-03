#include "LTimer.hpp"

/**
 * A class for a Timer, taken from LazyFoo tutorials.
 * Has functionality to start, stop, pause, unpause, check
 * if started or paused, and get the ticks since the timer
 * started.
 */

/**
 * Constructor for an LTimer. Initializes variables.
 */
LTimer::LTimer() {
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

/**
 * Starts the timer.
 */
void LTimer::start() {
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

/**
 * Stops the timer.
 */
void LTimer::stop() {
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

/**
 * Pauses the timer.
 */
void LTimer::pause() {
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused) {
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

/**
 * Unpauses the timer, if paused.
 */
void LTimer::unpause() {
	//If the timer is running and paused
	if (mStarted && mPaused) {
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

/**
 * Gets the ticks from the timer since started.
 */
Uint32 LTimer::getTicks() {
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted) {
		//If the timer is paused
		if (mPaused) {
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		} else {
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

/**
 * Checks if the timer has been started.
 */
bool LTimer::isStarted() {
	//Timer is running and paused or unpaused
	return mStarted;
}

/**
 * Checks if the timer is currently paused.
 */
bool LTimer::isPaused() {
	//Timer is running and paused
	return mPaused && mStarted;
}

