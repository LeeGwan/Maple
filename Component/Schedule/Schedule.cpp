#include "Schedule.h"
#include "../Window/window.h"
#include"../../Global.h"
#include "Engine/Calculation/Calculation.h"
// ScheduleManager.cpp

ScheduleManager::~ScheduleManager()
{
    delete G_ctx;  // 전역 컨텍스트 메모리 해제
}

ScheduleManager::ScheduleManager(HINSTANCE In_hInstance)
{
    _hInstance = In_hInstance;
    G_ctx->MakeWindows = make_shared<window>();  // 윈도우 객체 생성
}

void ScheduleManager::Update(HINSTANCE In_hInstance)
{
    // 렌더링 기반 스케줄 쓰레드 생성

    // 매니저 스케줄 쓰레드
    ManagerSchedule Thread_Manager_Schedule;
    push(&Thread_Manager_Schedule);

    // 엔진 스케줄 쓰레드
    EngineSchedule Thread_Engine_Schedule;
    push(&Thread_Engine_Schedule);

    // 렌더 스케줄 쓰레드
    RenderSchedule Thread_Render_Schedule(In_hInstance);
    push(&Thread_Render_Schedule);

    // 모든 스케줄의 Make 함수 실행
    MakeALL();
}

void ScheduleManager::push(Schedule* InPut_Schedule)
{
    ScheduleList.push_back(InPut_Schedule);  // 스케줄 리스트에 추가
}

void ScheduleManager::MakeALL()
{
    for (auto It_Schedule : ScheduleList)
    {
        It_Schedule->Make();  // 각 스케줄의 Make 실행
    }
}

// RenderSchedule.cpp

RenderSchedule::RenderSchedule(HINSTANCE In_hInstance)
{
    _hInstance = In_hInstance;
}

void RenderSchedule::Make()
{
    // 렌더링 전용 쓰레드 생성 및 실행
    thread RenderThread(bind(&RenderSchedule::Run, this));
    RenderThread.join();  // 메인 스레드에서 기다림
}

void RenderSchedule::Run()
{
    // 렌더링 업데이트 실행
    G_ctx->MakeWindows->Update(_hInstance);
}


// EngineSchedule.cpp

EngineSchedule::EngineSchedule()
{
}

void EngineSchedule::Make()
{
    // 엔진 메인 루프 실행 쓰레드 시작
    thread Engine_Thread(bind(&EngineSchedule::Run, this));
    Engine_Thread.detach();  // 백그라운드 실행
}

void EngineSchedule::Run()
{
    // 플레이어, 몬스터, 사운드 별 쓰레드 분리 실행
    thread Engine_For_Player_Thread(bind(&EngineSchedule::Run_For_Player, this));
    thread Engine_For_Monster_Thread(bind(&EngineSchedule::Run_For_Monster, this));
    thread Engine_For_Sound_Thread(bind(&EngineSchedule::Run_For_Sound, this));

    Engine_For_Player_Thread.detach();
    Engine_For_Monster_Thread.detach();
    Engine_For_Sound_Thread.detach();
}

void EngineSchedule::Run_For_Player()
{
    // 플레이어 계산 루틴
    Calculation Player_Calc;
    Player_Calc.Calculation_Init();
    Player_Calc.Start_Calc("Player");
}

void EngineSchedule::Run_For_Monster()
{
    // 몬스터 계산 루틴
    Calculation Monster_Calc;
    Monster_Calc.Calculation_Init();
    Monster_Calc.Start_Calc("Monster");
}

void EngineSchedule::Run_For_Sound()
{
    // 사운드 계산 루틴 - 지속적으로 사운드 업데이트
    while (1)
    {
        double DeltaTime = G_ctx->GetDeltaTime();
        G_ctx->G_SoundList.Sound_Update(DeltaTime);
        G_ctx->G_SoundList.DeleteSound(DeltaTime);

        timeBeginPeriod(1);  // 정확도 향상
        Sleep(1);
        timeEndPeriod(1);
    }
}


// ManagerSchedule.cpp

ManagerSchedule::ManagerSchedule()
{
}

void ManagerSchedule::Make()
{
    // 매니저 스케줄 쓰레드 실행
    thread Manager_Thread(bind(&ManagerSchedule::Run, this));
    Manager_Thread.detach();
}

void ManagerSchedule::Run()
{
    Timer Manager_Timer;

    // 계산 가능 상태 대기
    while (!G_ctx->CanUseCalc)
    {
        Sleep(10);
    }

    // 고정된 시간 간격으로 객체 매니지먼트
    while (1)
    {
        Manager_Timer.Update();
        Accumulator += Manager_Timer.GetDeltaTime();

        while (Accumulator >= FixedTime)
        {
            Check_Manager_Obj(FixedTime);
            Accumulator -= FixedTime;
        }

        timeBeginPeriod(1);
        Sleep(1);
        timeEndPeriod(1);
    }
}

void ManagerSchedule::Check_Manager_Obj(double DeltaTime)
{
    G_ctx->G_ObjectManager->Create_Object();             // 객체 생성
    G_ctx->G_ObjectManager->Check_Manager(DeltaTime);    // 객체 상태 검사 및 관리
}

