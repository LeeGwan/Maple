#include "Timer.h"
#include <timeapi.h> // Sleep 정밀도 향상
#pragma comment(lib, "winmm.lib")


Timer::Timer()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&lastTime);
	deltaTime = 0.0;
}






void Timer::Update()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	deltaTime = (double)(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
	lastTime = currentTime;
}

double Timer::GetDeltaTime() const
{
	return deltaTime;
}

void Timer::WaitForInterval(double seconds)
{
	std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}
