#include "iki_time.h"
#include "iki_window_win.h"

static float			g_time;
static LARGE_INTEGER	g_timeElapsed;
static LARGE_INTEGER	g_timeFrequency;
static LARGE_INTEGER	g_startTime;
static LARGE_INTEGER	g_endTime;

// QueryPerformanceCounter can be used to obtain the current time value, measured in counts.
// QueryPerformanceFrequency can be used to obtain the frequency (counts per second).

void IkiTimeInit(void)
{
	QueryPerformanceFrequency(&g_timeFrequency);
	QueryPerformanceCounter(&g_startTime);
}

void IkiTimeCount(void)
{
	QueryPerformanceCounter(&g_endTime);
	g_timeElapsed.QuadPart = g_endTime.QuadPart - g_startTime.QuadPart;

	g_timeElapsed.QuadPart *= 1000000;
	g_timeElapsed.QuadPart /= g_timeFrequency.QuadPart;

	g_time = (float)g_timeElapsed.QuadPart;

	g_startTime = g_endTime;
}

int64_t IkiGetPerformanceFrequency(void)
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}

int64_t IkiGetPerformanceCounter(void)
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

float IkiGetTimeInMicroseconds(void)
{
	return g_time;
}

float IkiGetTimeInMiliseconds(void)
{
	return g_time / 1000.0f;
}

float IkiGetTimeInSeconds(void)
{
	return IkiGetTimeInMiliseconds() / 1000.0f;
}
