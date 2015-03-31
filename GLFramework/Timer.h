#pragma once

#include <vector>
#include <algorithm>

// Manages time in seconds.
class Timer
{

private:

	static std::vector<Timer*> listTimers;

public:

	static void updateTimers(float dt);

private:

	float currentTime = 0.0f;
	float tickInterval;
	float tickDuration;
	float durationLeft;
	bool isRunning = false;
	bool ticked = false;

public:

	Timer(float interval, float duration = 0);
	~Timer();

	// === Functions ===

	void update(float dt);
	bool hasTicked();
	void start();
	void stop();
	void reset();

	// === Accessors ===

	float getTickInterval() const;
	float getDuration() const;
	bool getIsRunning() const;

	void setTickInterval(float interval);
	
};