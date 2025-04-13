#include "R.h"
#include "../../../../Engine/Input/Input.h"

// ------------------------------------------------------------
// 생성자: R 스킬 초기화
// ------------------------------------------------------------
R::R()
{
    SkillName = "R_";

    // 사운드 경로 설정
    RSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
    RSkillHitAnimation_Path = SkillMusicPath + "Hit.mp3";

    // 애니메이션 상태 초기화
    IsLastFrame1 = true;
    IsLastFrame2 = true;
    IsLastFrame3 = true;

    CoolTime = 0.0;

    // 시전 및 히트 애니메이션 프레임 로딩
    R_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", RSkillAnimation1, 10);
    R_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", RSkillHitAnimation, 8);
}

// ------------------------------------------------------------
// 소멸자: 리소스 해제
// ------------------------------------------------------------
R::~R()
{
    for (const auto& [key, value] : RSkillAnimation1)
        if (value) value->Release();
    RSkillAnimation1.clear();

    for (const auto& [key, value] : RSkillHitAnimation)
        if (value) value->Release();
    RSkillHitAnimation.clear();
}

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------
std::string R::Get_SkillAnimation1_Path() const       { return RSkillAnimation1_Path; }
std::string R::Get_SkillHitAnimation_Path() const     { return RSkillHitAnimation_Path; }

double R::Get_MaxCoolTime() const                     { return MaxCoolTime; }
double R::Get_CoolTime() const                        { return CoolTime; }

float R::Get_SkillRadius() const                      { return SkillRadius; }
float R::Get_SkillDmg() const                         { return SkillDmg; }

Vec2 R::Get_SkillPos() const                          { return SkillPos; }
Vec2 R::Get_SkillSize() const                         { return SkillSize; }

Vec2 R::Get_HitSkillPos() const                       { return HitSkillPos; }
Vec2 R::Get_HitSkillSize() const                      { return HitSkillSize; }

SkillState R::Get_SkillState() const                  { return CurrentState; }

bool* R::Get_IsLastFrame1()                           { return &IsLastFrame1; }
bool* R::Get_IsLastFrame2()                           { return &IsLastFrame2; }
bool* R::Get_IsLastFrame3()                           { return &IsLastFrame3; }

std::map<int, ID3D11ShaderResourceView*>& R::Get_SkillAnimation1()   { return RSkillAnimation1; }
std::map<int, ID3D11ShaderResourceView*>& R::Get_SkillHitAnimation() { return RSkillHitAnimation; }

Animation& R::Get_Skill_1()                           { return R_Skill_1; }
Animation& R::Get_Hit_Skill()                         { return R_Hit_Skill; }

int R::Get_Key() const                                { return key; }

// ------------------------------------------------------------
// 이 스킬이 특정 prefix인지 확인
// ------------------------------------------------------------
bool R::Who_Am_I(std::string Who)
{
    return !SkillName.compare(Who);
}

// ------------------------------------------------------------
// 쿨타임 처리
// ------------------------------------------------------------
void R::Minus_CoolTime(double _Time)      { CoolTime -= _Time; }
void R::Set_CoolTime(double _Time)        { CoolTime = _Time; }

// ------------------------------------------------------------
// 스킬 상태 설정
// ------------------------------------------------------------
void R::Set_SkillState(SkillState _ChangeState)   { CurrentState = _ChangeState; }

// ------------------------------------------------------------
// 애니메이션 프레임 완료 여부 설정
// ------------------------------------------------------------
void R::Set_IsLastFrame1(bool _Set)      { IsLastFrame1 = _Set; }
void R::Set_IsLastFrame2(bool _Set)      { IsLastFrame2 = _Set; }
void R::Set_IsLastFrame3(bool _Set)      { IsLastFrame3 = _Set; }

// ------------------------------------------------------------
// 키 바인딩 설정
// ------------------------------------------------------------
void R::Set_Key(int _key)                { key = _key; }
