#include "T.h"
#include "../../../../Engine/Input/Input.h"

// ------------------------------------------------------------
// 생성자: T 스킬 초기화
// ------------------------------------------------------------
T::T()
{
    SkillName = "T_";

    // 사운드 경로 설정
    TSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
    TSkillHitAnimation_Path = SkillMusicPath + SkillName + "Hit.mp3";

    // 상태 초기화
    IsLastFrame1 = true;
    IsLastFrame2 = true;
    IsLastFrame3 = true;
    CoolTime = 0.0;

    // 애니메이션 리소스 초기화
    T_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", TSkillAnimation1, 49);
    T_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", TSkillHitAnimation, 8);
}

// ------------------------------------------------------------
// 소멸자: 로드된 텍스처 리소스 해제
// ------------------------------------------------------------
T::~T()
{
    for (const auto& [key, value] : TSkillAnimation1)
        if (value) value->Release();
    TSkillAnimation1.clear();

    for (const auto& [key, value] : TSkillHitAnimation)
        if (value) value->Release();
    TSkillHitAnimation.clear();
}

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------
std::string T::Get_SkillAnimation1_Path() const       { return TSkillAnimation1_Path; }
std::string T::Get_SkillHitAnimation_Path() const     { return TSkillHitAnimation_Path; }

double T::Get_MaxCoolTime() const                     { return MaxCoolTime; }
double T::Get_CoolTime() const                        { return CoolTime; }

float T::Get_SkillRadius() const                      { return SkillRadius; }
float T::Get_SkillDmg() const                         { return SkillDmg; }

Vec2 T::Get_SkillPos() const                          { return SkillPos; }
Vec2 T::Get_SkillSize() const                         { return SkillSize; }

Vec2 T::Get_HitSkillPos() const                       { return HitSkillPos; }
Vec2 T::Get_HitSkillSize() const                      { return HitSkillSize; }

SkillState T::Get_SkillState() const                  { return CurrentState; }

bool* T::Get_IsLastFrame1()                           { return &IsLastFrame1; }
bool* T::Get_IsLastFrame2()                           { return &IsLastFrame2; }
bool* T::Get_IsLastFrame3()                           { return &IsLastFrame3; }

std::map<int, ID3D11ShaderResourceView*>& T::Get_SkillAnimation1()   { return TSkillAnimation1; }
std::map<int, ID3D11ShaderResourceView*>& T::Get_SkillHitAnimation() { return TSkillHitAnimation; }

Animation& T::Get_Skill_1()                           { return T_Skill_1; }
Animation& T::Get_Hit_Skill()                         { return T_Hit_Skill; }

int T::Get_Key() const                                { return key; }

// ------------------------------------------------------------
// prefix 비교로 스킬 식별
// ------------------------------------------------------------
bool T::Who_Am_I(std::string Who)
{
    return !SkillName.compare(Who);
}

// ------------------------------------------------------------
// 쿨타임 처리
// ------------------------------------------------------------
void T::Minus_CoolTime(double _Time)      { CoolTime -= _Time; }
void T::Set_CoolTime(double _Time)        { CoolTime = _Time; }

// ------------------------------------------------------------
// 상태 전환 처리
// ------------------------------------------------------------
void T::Set_SkillState(SkillState _ChangeState)   { CurrentState = _ChangeState; }

// ------------------------------------------------------------
// 애니메이션 종료 상태 설정
// ------------------------------------------------------------
void T::Set_IsLastFrame1(bool _Set)      { IsLastFrame1 = _Set; }
void T::Set_IsLastFrame2(bool _Set)      { IsLastFrame2 = _Set; }
void T::Set_IsLastFrame3(bool _Set)      { IsLastFrame3 = _Set; }

// ------------------------------------------------------------
// 키 바인딩
// ------------------------------------------------------------
void T::Set_Key(int _key)                { key = _key; }
