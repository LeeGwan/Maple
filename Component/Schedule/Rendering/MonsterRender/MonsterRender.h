#pragma once
#include "../RenderingObject.h"
#include "../EachImageRender.h"
#include <map>

// ------------------------------------------------------------
// MonsterRender
// - 몬스터 애니메이션 및 체력바 렌더링 클래스
// ------------------------------------------------------------
class MonsterRender : public EachImageRender
{
private:
    // 애니메이션 프레임 데이터 (상태별로 프레임맵 구성)
    std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>> Animations;

    // 이전 위치 (속도 계산용)
    Vec2 BeforeVec;

    // 렌더링용 기본 크기
    Vec2 Size;

    // 현재 애니메이션 상태
    AnimationType Current_AnimationType_State;

    // 애니메이션 처리 객체
    Animation Anim;

    // 체력바 리소스 경로 및 로딩된 텍스처
    std::string ResourcePath[2];                         // [0] 배경, [1] HP
    ID3D11ShaderResourceView* Resource[2] = { nullptr }; // 텍스처 핸들

    // 임시 사이즈(텍스처 로드용 임시 Vec2)
    Vec2 TMP;

    // 데미지 관련 타이머 (추후 이펙트 처리 시 활용 가능)
    double DmgTime = 0.0;
    const double FixedTime = 0.016; // 60FPS 기준

public:
    // 생성자/소멸자
    MonsterRender();
    ~MonsterRender() override;

    // 초기화: 애니메이션 프레임 및 체력바 텍스처 로딩
    void Init(std::string Name, Vec2& Pos, std::vector<int> AnimationCout) override;

    // 업데이트: 애니메이션 상태에 따라 몬스터 렌더링
    void Update(Vec2& Pos, double Direction) override;

    // 체력바 그리기
    void Draw_Hp_Bar(Vec2& Pos, float MaxState, float State);

    // 현재 애니메이션 상태 조회/설정
    AnimationType Get_Current_AnimationType_State();
    void Set_Current_AnimationType_State(AnimationType ChangeState);

    // 렌더링 크기 반환
    Vec2 Get_Size() override;
};
