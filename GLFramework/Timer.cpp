#include "Timer.h"

using namespace std;

// === Static Declarations ===

vector<Timer*> Timer::listTimers;

Timer::Timer(float interval, float duration)
{
	this->tickInterval = interval;
	this->duration = duration;
	listTimers.push_back(this);
}

void Timer::updateTimers(float dt)
{
	for (Timer* timer : listTimers)
	{
		timer->update(dt);
	}
}

void Timer::update(float dt)
{
	if (!isRunning) return;

	// Manage time in seconds.
	currentTime += dt;

	// If tick inverval is elasped, set tick.
	if (currentTime > tickInterval)
	{
		// If timer is set to run over a period of time.
		if (duration != 0.0f)
		{
			duration -= currentTime;
		}

		currentTime = 0.0f;
		ticked = true;
	}

	// Stop timer if it has ran its duration
	if (duration < 0.0f)
	{
		isRunning = false;
	}
}

Timer* Timer::destroy()
{
	listTimers.erase(remove(listTimers.begin(), listTimers.end(), this), listTimers.end());
	delete this;
	return NULL;
}

bool Timer::hasTicked()
{
	if (ticked)
	{
		ticked = false;
		return true;
	}

	return false;
}

void Timer::start()
{
	isRunning = true;
}

void Timer::stop()
{
	isRunning = false;
}

float Timer::getTickInterval() const
{
	return tickInterval;
}

float Timer::getDuration() const
{
	return duration;
}

bool Timer::getIsRunning() const
{
	return isRunning;
}

void Timer::setTickInterval(float interval)
{
	tickInterval = interval;
}