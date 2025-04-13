#include "Schedule.h"
#include "../Window/window.h"
#include "../../Global.h"
#include "Engine/Calculation/Calculation.h"

//
// ScheduleManager: 스케줄 객체들의 생성과 실행을 총괄하는 매니저 클래스 구현
//

ScheduleManager::~ScheduleManager()
{
    // 전역 컨텍스트 메모리 해제
    delete G_ctx;
}

ScheduleManager::ScheduleManager(HINSTANCE In_hInstance)
{
    _hInstance = In_hInstance;

    // 윈도우 객체 초기화 (렌더링 준비)
    G_ctx->MakeWindows = make_shared<window>();
}

void ScheduleManager::Update(HINSTANCE In_hInstance)
{
    // 각종 스케줄 클래스들을 인스턴스로 생성하고 실행 대기열에 추가

    // 1. 객체 상태관리 스케줄 추가
    ManagerSchedule* Thread_Manager_Schedule = new ManagerSchedule();
    push(Thread_Manager_Schedule);

    // 2. 게임 엔진 계산 스케줄 추가 (플레이어, 몬스터, 사운드)
    EngineSchedule* Thread_Engine_Schedule = new EngineSchedule();
    push(Thread_Engine_Schedule);

    // 3. 렌더링 스케줄 추가
    RenderSchedule* Thread_Render_Schedule = new RenderSchedule(In_hInstance);
    push(Thread_Render_Schedule);

    // 모든 스케줄에 대해 초기화(Make) 실행
    MakeALL();
}

void ScheduleManager::push(Schedule* InPut_Schedule)
{
    // 스케줄 리스트에 스케줄 객체를 등록
    ScheduleList.push_back(InPut_Schedule);
}

void ScheduleManager::MakeALL()
{
    // 등록된 모든 스케줄 객체에 대해 Make() 실행
    for (auto It_Schedule : ScheduleList)
    {
        It_Schedule->Make();
    }
}

//
// RenderSchedule: 렌더링 처리 전용 스케줄 클래스
//

RenderSchedule::RenderSchedule(HINSTANCE In_hInstance)
    : _hInstance(In_hInstance)
{
}

void RenderSchedule::Make()
{
    // 렌더링 전용 스레드 생성
    thread RenderThread(bind(&RenderSchedule::Run, this));

    // 렌더링은 메인 루프에 가까우므로 join (blocking 실행)
    RenderThread.join();
}

void RenderSchedule::Run()
{
    // 렌더링 루프 진입
    G_ctx->MakeWindows->Update(_hInstance);
}

//
// EngineSchedule: 게임 내 계산 요소(플레이어/몬스터/사운드)들을 처리하는 스케줄 클래스
//

EngineSchedule::EngineSchedule()
{
}

void EngineSchedule::Make()
{
    // 메인 엔진 스레드 (실행 시 내부에서 또 세분화됨)
    thread Engine_Thread(bind(&EngineSchedule::Run, this));
    Engine_Thread.detach(); // 백그라운드 실행
}

void EngineSchedule::Run()
{
    // 엔진은 3개 서브 루틴으로 구성됨. 각각 별도 스레드로 실행.

    thread Engine_For_Player_Thread(bind(&EngineSchedule::Run_For_Player, this));
    thread Engine_For_Monster_Thread(bind(&EngineSchedule::Run_For_Monster, this));
    thread Engine_For_Sound_Thread(bind(&EngineSchedule::Run_For_Sound, this));

    Engine_For_Player_Thread.detach();
    Engine_For_Monster_Thread.detach();
    Engine_For_Sound_Thread.detach();
}

void EngineSchedule::Run_For_Player()
{
    // 플레이어 관련 계산 루프 시작
    Calculation Player_Calc;
    Player_Calc.Calculation_Init();
    Player_Calc.Start_Calc("Player");
}

void EngineSchedule::Run_For_Monster()
{
    // 몬스터 관련 계산 루프 시작
    Calculation Monster_Calc;
    Monster_Calc.Calculation_Init();
    Monster_Calc.Start_Calc("Monster");
}

void EngineSchedule::Run_For_Sound()
{
    // 사운드 업데이트 및 삭제 루프
    while (true)
    {
        double DeltaTime = G_ctx->GetDeltaTime();

        G_ctx->G_SoundList.Sound_Update(DeltaTime);   // 사운드 상태 업데이트
        G_ctx->G_SoundList.DeleteSound(DeltaTime);    // 완료된 사운드 삭제

        timeBeginPeriod(1); // Sleep 정확도 향상
        Sleep(1);           // 1ms 대기
        timeEndPeriod(1);
    }
}

//
// ManagerSchedule: 오브젝트 생성/삭제 및 상태 점검을 주기적으로 수행하는 스케줄 클래스
//

ManagerSchedule::ManagerSchedule()
{
}

void ManagerSchedule::Make()
{
    // 객체 관리 루프를 실행하는 스레드 시작
    thread Manager_Thread(bind(&ManagerSchedule::Run, this));
    Manager_Thread.detach();
}

void ManagerSchedule::Run()
{
    Timer Manager_Timer;

    // 게임 계산 가능 상태가 될 때까지 대기
    while (!G_ctx->CanUseCalc)
    {
        Sleep(10);
    }

    // 고정 주기로 객체 상태 관리
    while (true)
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
    // 새로운 몬스터 생성 및 죽은 객체 제거
    G_ctx->G_ObjectManager->Create_Object();

    // 살아있는 객체의 상태 점검 및 자동 제거
    G_ctx->G_ObjectManager->Check_Manager(DeltaTime);
}
