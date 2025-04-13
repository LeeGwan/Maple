#include "Global.h"

// ---------------------- 전역 인스턴스 초기화 ----------------------

// 전역 컨텍스트 객체 초기화
Global* G_ctx = new Global;

// 정적 멤버 객체 정의
Timer Global::RenderTimer;
DamageObject Global::DmgObject;
SoundObject Global::G_SoundList;
HttpSocket Global::LoginCheck;

// ---------------------- 생성자 ----------------------

/*
    Global 생성자
    - 주요 매니저 포인터 초기화
    - 계산 가능 플래그 비활성화 (Calc은 Init 이후에 가능)
*/
Global::Global()
{
    CanUseCalc = false;         // 아직 계산 불가 상태
    G_ObjectManager = nullptr;  // 객체 매니저는 Update 시점에 생성됨
}

// ---------------------- 타이머 관련 함수 ----------------------

/*
    Timer_Update
    - 내부 RenderTimer를 갱신하여 현재 프레임의 델타 타임을 계산
*/
void Global::Timer_Update()
{
    RenderTimer.Update(); // 고정 루프/가변 루프에서 호출됨
}

/*
    GetDeltaTime
    - 마지막 Timer_Update() 이후의 경과 시간을 반환
    - 단위: 초 (double)
*/
double Global::GetDeltaTime() const
{
    return RenderTimer.GetDeltaTime();
}

/*
    WaitForInterval
    - 지정된 시간(초) 동안 스레드를 일시 대기 상태로 전환
    - 대기 정확도는 OS/타이머 구현에 의존
*/
void Global::WaitForInterval(double seconds)
{
    RenderTimer.WaitForInterval(seconds);
}
