#include "W.h"
#include "../../../../Engine/Input/Input.h"

// ------------------------------------------------------------
// 생성자: W 스킬의 기본 정보 및 애니메이션 초기화
// ------------------------------------------------------------
W::W()
{
    SkillName = "W_";

    // 스킬 효과음 경로 설정
    WSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
    WSkillHitAnimation_Path = SkillMusicPath + "Hit.mp3";

    // 애니메이션 완료 플래그 초기화
    IsLastFrame1 = true;
    IsLastFrame2 = true;
    IsLastFrame3 = true;

    CoolTime = 0.0;

    // 시전 및 피격 애니메이션 초기화
    W_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", WSkillAnimation1, 20);
    W_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", WSkillHitAnimation, 8);
}

// ------------------------------------------------------------
// 소멸자: DirectX 리소스 해제
// ------------------------------------------------------------
W::~W()
{
    for (const auto& [key, value] : WSkillAnimation1)
        if (value) value->Release();
    WSkillAnimation1.clear();

    for (const auto& [key, value] : WSkillHitAnimation)
        if (value) value->Release();
    WSkillHitAnimation.clear();
}

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------
std::string W::Get_SkillAnimation1_Path() const       { return WSkillAnimation1_Path; }
std::string W::Get_SkillHitAnimation_Path() const     { return WSkillHitAnimation_Path; }

double W::Get_MaxCoolTime() const                     { return MaxCoolTime; }
double W::Get_CoolTime() const                        { return CoolTime; }

float W::Get_SkillRadius() const                      { return SkillRadius; }
float W::Get_SkillDmg() const                         { return SkillDmg; }

Vec2 W::Get_SkillPos() const                          { return SkillPos; }
Vec2 W::Get_SkillSize() const                         { return SkillSize; }

Vec2 W::Get_HitSkillPos() const                       { return HitSkillPos; }
Vec2 W::Get_HitSkillSize() const                      { return HitSkillSize; }

SkillState W::Get_SkillState() const                  { return CurrentState; }

bool* W::Get_IsLastFrame1()                           { return &IsLastFrame1; }
bool* W::Get_IsLastFrame2()                           { return &IsLastFrame2; }
bool* W::Get_IsLastFrame3()                           { return &IsLastFrame3; }

std::map<int, ID3D11ShaderResourceView*>& W::Get_SkillAnimation1()   { return WSkillAnimation1; }
std::map<int, ID3D11ShaderResourceView*>& W::Get_SkillHitAnimation() { return WSkillHitAnimation; }

Animation& W::Get_Skill_1()                           { return W_Skill_1; }
Animation& W::Get_Hit_Skill()                         { return W_Hit_Skill; }

int W::Get_Key() const                                { return key; }

// ------------------------------------------------------------
// 특정 prefix로 스킬 식별
// ------------------------------------------------------------
bool W::Who_Am_I(std::string Who)
{
    return !SkillName.compare(Who);
}

// ------------------------------------------------------------
// 쿨타임 관리
// ------------------------------------------------------------
void W::Minus_CoolTime(double _Time)      { CoolTime -= _Time; }
void W::Set_CoolTime(double _Time)        { CoolTime = _Time; }

// ------------------------------------------------------------
// 스킬 상태 전환
// ------------------------------------------------------------
void W::Set_SkillState(SkillState _ChangeState)   { CurrentState = _ChangeState; }

// ------------------------------------------------------------
// 애니메이션 종료 여부 설정
// ------------------------------------------------------------
void W::Set_IsLastFrame1(bool _Set)      { IsLastFrame1 = _Set; }
void W::Set_IsLastFrame2(bool _Set)      { IsLastFrame2 = _Set; }
void W::Set_IsLastFrame3(bool _Set)      { IsLastFrame3 = _Set; }

// ------------------------------------------------------------
// 키 바인딩
// ------------------------------------------------------------
void W::Set_Key(int _key)                { key = _key; }
