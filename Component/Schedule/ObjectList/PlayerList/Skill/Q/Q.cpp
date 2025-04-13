#include "Q.h"
#include "../../../../Engine/Input/Input.h"

// ------------------------------------------------------------
// 생성자: Q 스킬 초기화
// ------------------------------------------------------------
Q::Q()
{
    SkillName = "Q_";

    // 사운드 경로 설정
    QSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
    QSkillHitAnimation_Path = SkillMusicPath + SkillName + "Hit.mp3";

    // 프레임 초기 상태
    IsLastFrame1 = true;
    IsLastFrame2 = true;
    IsLastFrame3 = true;

    CoolTime = 0.0;

    // 애니메이션 리소스 초기화
    Q_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", QSkillAnimation1, 43);
    Q_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", QSkillHitAnimation, 7);
}

// ------------------------------------------------------------
// 소멸자: 로딩된 텍스처 리소스 해제
// ------------------------------------------------------------
Q::~Q()
{
    for (const auto& [key, value] : QSkillAnimation1)
        if (value) value->Release();
    QSkillAnimation1.clear();

    for (const auto& [key, value] : QSkillHitAnimation)
        if (value) value->Release();
    QSkillHitAnimation.clear();
}

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------
std::string Q::Get_SkillAnimation1_Path() const       { return QSkillAnimation1_Path; }
std::string Q::Get_SkillHitAnimation_Path() const     { return QSkillHitAnimation_Path; }

double Q::Get_MaxCoolTime() const                     { return MaxCoolTime; }
double Q::Get_CoolTime() const                        { return CoolTime; }

float Q::Get_SkillRadius() const                      { return SkillRadius; }
float Q::Get_SkillDmg() const                         { return SkillDmg; }

Vec2 Q::Get_SkillPos() const                          { return SkillPos; }
Vec2 Q::Get_SkillSize() const                         { return SkillSize; }
Vec2 Q::Get_HitSkillPos() const                       { return HitSkillPos; }
Vec2 Q::Get_HitSkillSize() const                      { return HitSkillSize; }

SkillState Q::Get_SkillState() const                  { return CurrentState; }

bool* Q::Get_IsLastFrame1()                           { return &IsLastFrame1; }
bool* Q::Get_IsLastFrame2()                           { return &IsLastFrame2; }
bool* Q::Get_IsLastFrame3()                           { return &IsLastFrame3; }

std::map<int, ID3D11ShaderResourceView*>& Q::Get_SkillAnimation1()   { return QSkillAnimation1; }
std::map<int, ID3D11ShaderResourceView*>& Q::Get_SkillHitAnimation() { return QSkillHitAnimation; }

Animation& Q::Get_Skill_1()                           { return Q_Skill_1; }
Animation& Q::Get_Hit_Skill()                         { return Q_Hit_Skill; }

int Q::Get_Key() const                                { return key; }

// ------------------------------------------------------------
// 이 스킬이 특정 prefix인지 확인
// ------------------------------------------------------------
bool Q::Who_Am_I(std::string Who)
{
    return !SkillName.compare(Who);
}

// ------------------------------------------------------------
// 쿨타임 감소 및 설정
// ------------------------------------------------------------
void Q::Minus_CoolTime(double _Time)      { CoolTime -= _Time; }
void Q::Set_CoolTime(double _Time)        { CoolTime = _Time; }

// ------------------------------------------------------------
// 스킬 상태 설정
// ------------------------------------------------------------
void Q::Set_SkillState(SkillState _ChangeState)   { CurrentState = _ChangeState; }

// ------------------------------------------------------------
// 애니메이션 프레임 완료 여부 설정
// ------------------------------------------------------------
void Q::Set_IsLastFrame1(bool _Set)      { IsLastFrame1 = _Set; }
void Q::Set_IsLastFrame2(bool _Set)      { IsLastFrame2 = _Set; }
void Q::Set_IsLastFrame3(bool _Set)      { IsLastFrame3 = _Set; }

// ------------------------------------------------------------
// 키 바인딩 설정
// ------------------------------------------------------------
void Q::Set_Key(int _key)                { key = _key; }
