#include "Timer.h"
#include <timeapi.h>     // Windows Sleep 정확도 향상용
#pragma comment(lib, "winmm.lib") // timeBeginPeriod / timeEndPeriod 사용 시 필요

// ------------------------------------
// 생성자 - 타이머 초기화
// ------------------------------------
Timer::Timer()
{
    // 초당 타이머 주파수 (정밀도)
    QueryPerformanceFrequency(&frequency);

    // 현재 시간 저장 (기준점)
    QueryPerformanceCounter(&lastTime);

    deltaTime = 0.0;
}

// ------------------------------------
// 프레임마다 시간 차 계산
// ------------------------------------
void Timer::Update()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);  // 현재 시간 측정

    // 프레임 간 시간 차이 (초 단위)
    deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

    // 기준 시간 갱신
    lastTime = currentTime;
}

// ------------------------------------
// 마지막 프레임과의 시간 차 반환
// ------------------------------------
double Timer::GetDeltaTime() const
{
    return deltaTime;
}

// ------------------------------------
// 지정된 시간만큼 대기 (초 단위)
// ------------------------------------
void Timer::WaitForInterval(double seconds)
{
    std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}
