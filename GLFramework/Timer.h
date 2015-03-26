#pragma once

#include <vector>
#include <algorithm>

// Manages time in seconds.
class Timer
{

private:

	static std::vector<Timer*> listTimers;

	float currentTime = 0.0f;
	float tickInterval;
	float duration;
	bool isRunning = true;
	bool ticked = false;

public:

	Timer(float interval, float duration = 0);

	// === Functions ===

	static void updateTimers(float dt);

	void update(float dt);
	Timer* destroy();
	bool hasTicked();
	void start();
	void stop();

	// === Accessors ===

	float getTickInterval() const;
	float getDuration() const;
	bool getIsRunning() const;

	void setTickInterval(float interval);
	
};