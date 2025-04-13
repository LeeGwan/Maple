#pragma once

#include "pch.h"

// 컴포넌트 헤더들 (엔진 구성요소 포함)
#include "Component/Window/window.h"
#include "Component/Schedule/Rendering/RenderingObject.h"
#include "Component/Schedule/ObjectList/ObjectManager.h"
#include "Component/Schedule/ObjectList/DamageObject/DamageObject.h"
#include "Component/Schedule/ObjectList/SoundList/SoundObject.h"
#include "Component/Schedule/ObjectList/LoginMap/Login/HttpSocket.h"

// 메모리 유효성 검사 매크로 정의
#define IsValidAddr(addr) \
    (static_cast<uintptr_t>(addr) > 0x100 && static_cast<uintptr_t>(addr) < 0x00007fffffffffff && !IsBadReadPtr((void*)addr, sizeof(uintptr_t)))

#define IsValidPtr(ptr) \
    ([&]() -> bool { uintptr_t _val = *(uintptr_t*)(ptr); return IsValidAddr(_val); }())

// Global 클래스: 엔진의 모든 전역 리소스를 통합 관리하는 클래스
class Global
{
public:
    // ---------- 전역 공유 객체 ----------
    
    // 윈도우 시스템 관리자
    shared_ptr<window> MakeWindows;

    // 렌더링 시스템 관리자
    shared_ptr<RenderObject> MakeRender;

    // 게임 객체(플레이어, 몬스터 등) 매니저
    shared_ptr<ObjectManager> G_ObjectManager;

    // 계산 가능 여부 플래그 (모든 시스템 초기화 이후 true)
    bool CanUseCalc;

    // ---------- 생성자 ----------
    
    // 기본 생성자: 각종 초기값 설정
    Global();

    // ---------- 정적 유틸 컴포넌트 ----------
    
    // 전역 데미지 출력 처리기
    static DamageObject DmgObject;

    // 전역 사운드 출력 처리기
    static SoundObject G_SoundList;

    // 렌더링 또는 스케줄 타이머
    static Timer RenderTimer;

    // 로그인 처리기 (회원가입 포함)
    static HttpSocket LoginCheck;

    // ---------- 시간 관련 유틸 ----------
    
    // 프레임 기준 델타 타임 업데이트
    void Timer_Update();

    // 현재 프레임의 경과 시간 반환 (초 단위)
    double GetDeltaTime() const;

    // 일정 시간 동안 대기 (초 단위)
    void WaitForInterval(double seconds);
};

// 외부에서 접근 가능한 전역 포인터
extern Global* G_ctx;
