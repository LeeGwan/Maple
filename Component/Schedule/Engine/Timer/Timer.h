#pragma once
#include "../../../../pch.h"

class Timer {
private:
    LARGE_INTEGER frequency;
    LARGE_INTEGER lastTime;
    double deltaTime;

public:
    Timer();
    void Update();

    double GetDeltaTime() const;

    // 특정 시간(초) 동안 대기 (CPU 사용량 줄이기)
    void WaitForInterval(double seconds);
};