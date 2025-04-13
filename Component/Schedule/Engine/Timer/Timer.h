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

    // Ư�� �ð�(��) ���� ��� (CPU ��뷮 ���̱�)
    void WaitForInterval(double seconds);
};