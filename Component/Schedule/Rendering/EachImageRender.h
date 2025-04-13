#pragma once

#include "../../../pch.h"
#include "../../../Global.h"
#include "../Engine/AnimationType.h"
#include "../Engine/Animation/Animation.h"
#include "../Rendering/RenderingObject.h"
#include "Color/Color.h"

// ------------------------------------------------------------
// EachImageRender
// - 렌더링 객체의 공통 인터페이스 클래스
// - 모든 렌더러(PlayerRender, MonsterRender 등)는 이 클래스를 상속
// ------------------------------------------------------------
class EachImageRender
{
public:
    virtual ~EachImageRender() = default;

    // 렌더링 업데이트 (프레임마다 호출)
    // - Pos: 위치
    // - Direction: 방향 (ex: 좌우반전 등)
    virtual void Update(Vec2& Pos, double Direction) = 0;

    // 초기화 함수
    // - Name: 객체 이름
    // - Pos: 초기 위치
    // - AnimationCout: 애니메이션 프레임 수 목록
    virtual void Init(std::string Name, Vec2& Pos, std::vector<int> AnimationCout) = 0;

    // 현재 객체의 사이즈 반환
    virtual Vec2 Get_Size() = 0;
};
