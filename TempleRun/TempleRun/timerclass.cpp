///////////////////////////////////////////////////////////////////////////////
// Filename: timerclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "timerclass.h"


TimerClass::TimerClass()
{
	Play = false;

	time = 0;
}


TimerClass::TimerClass(const TimerClass& other)
{
}


TimerClass::~TimerClass()
{
}


bool TimerClass::Initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);
	return true;
}

void TimerClass::Restart()
{
	time = 0;

	return;
}
void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_currentTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_currentTime = currentTime;

	//time = (float)(currentTime - m_startTime) / 10000000;
	if (Play == true)
	{
		time += (float)timeDifference / 10000000;
	}
	return;
}


float TimerClass::GetFrameTime()
{
	return m_frameTime;
}

float TimerClass::GetStartTime()
{
	return time;
}