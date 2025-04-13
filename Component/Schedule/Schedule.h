#pragma once

#include "../../pch.h"
#include "Rendering/RenderingObject.h"

// Schedule: 모든 스케줄의 기본 인터페이스 역할을 하는 추상 클래스
class Schedule
{
public:
    virtual void Make() = 0; // 실행 준비 작업 (리소스 초기화 등)
    virtual void Run() = 0;  // 실제 실행될 스레드 함수 또는 루프
};

// RenderSchedule: 렌더링 관련 스케줄 처리 클래스
class RenderSchedule : public Schedule
{
    HINSTANCE _hInstance; // 윈도우 인스턴스 핸들

public:
    RenderSchedule(HINSTANCE In_hInstance); // 생성자
    void Make() override;  // 렌더링 스레드 준비
    virtual void Run() override; // 렌더링 로직 실행
};

// EngineSchedule: 게임 엔진 관련 스케줄 (플레이어, 몬스터, 사운드 등 계산 처리)
class EngineSchedule : public Schedule
{
public:
    EngineSchedule();
    void Make() override; // 엔진 관련 스레드 준비

    void Run_For_Player();  // 플레이어 관련 계산 처리
    void Run_For_Monster(); // 몬스터 관련 계산 처리
    void Run_For_Sound();   // 사운드 처리 루프
    virtual void Run() override; // 위 세 함수를 병렬 스레드로 실행
};

// ManagerSchedule: 객체 상태 관리 (예: 몬스터 생성, 제거 등)
class ManagerSchedule : public Schedule
{
private:
    double Accumulator = 0.0;         // 누적 시간
    const double FixedTime = 0.01;    // 고정 프레임 단위
    double CheckAccumulator = 0.0;    // 관리용 보조 타이머

public:
    ManagerSchedule();
    void Make() override; // 관리 스레드 준비

    void Check_Manager_Obj(double DeltaTime); // 객체 상태 점검 및 생성 관리
    virtual void Run() override; // 상태를 주기적으로 점검하는 루프
};

// ScheduleManager: 여러 스케줄들을 관리하고 초기화 및 실행하는 컨트롤러 클래스
class ScheduleManager
{
private:
    vector<Schedule*> ScheduleList; // 등록된 스케줄 리스트
    HINSTANCE _hInstance;           // 윈도우 인스턴스 핸들

public:
    ~ScheduleManager(); // 소멸자: 메모리 정리
    ScheduleManager(HINSTANCE In_hInstance); // 생성자: 핸들 등록

    void Update(HINSTANCE In_hInstance); // 전체 스케줄 추가 및 실행
    void push(Schedule* InPut_Schedule); // 새로운 스케줄 등록
    void MakeALL(); // 등록된 모든 스케줄 Make 호출 (초기화)
};
