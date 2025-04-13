#include "DamageLib.h"
#include "../AnimationType.h"
#include "DamageLib.h"
#include "../AnimationType.h"

// --------------------------
// 생성자 - 기본값 초기화
// --------------------------
DamageLib::DamageLib()
{
    AttackTime = 0.0;
}

// --------------------------
// 단일 대상 공격 처리
// --------------------------
Dmg_Result& DamageLib::Damage_Update(ObjectClass* MyObj)
{
    static Dmg_Result ReturnValue = { 0.0f, nullptr };

    // 무작위 추가 데미지
    int Plus_Dmg = rand() % 15;

    // 플레이어인지 여부 확인
    bool IsPlayer = MyObj->Get_Name().find("Player") != std::string::npos;

    // 기본 데미지 계산
    float Basic_Dmg = MyObj->Get_BasicDmg();
    float Total_Dmg = IsPlayer
        ? ((Basic_Dmg / 2.0f) * MyObj->Get_Str()) + Plus_Dmg
        : Basic_Dmg + Plus_Dmg;

    // 충돌 대상 탐색
    ObjectClass* T_Obj = Collisions.Check_Collision(MyObj, MyObj->Get_Radius());

    if (!T_Obj) return ReturnValue;

    ReturnValue = { Total_Dmg, T_Obj };

    // 대상이 경직 상태(Alert)가 아니고, 무적이 아닐 때만 피해 적용
    if (T_Obj->Get_Animaion_State() != AnimationType::Alert && !T_Obj->Get_immortal())
    {
        Calc_Dmg(Total_Dmg, T_Obj);
    }

    return ReturnValue;
}

// --------------------------
// 다중 대상 스킬 피해 처리
// --------------------------
std::vector<Dmg_Result> DamageLib::Damage_Update(std::vector<ObjectClass*>& TargetObj, float SkillDmg)
{
    std::vector<Dmg_Result> ReturnValue;

    for (auto& it : TargetObj)
    {
        float MonsterHP = it->Get_Hp();
        if (MonsterHP == 0.0f) continue;

        // 무작위 보정 포함한 데미지 계산
        float TotalDmg = static_cast<float>(rand() % 55) + SkillDmg;

        Calc_Dmg(TotalDmg, it); // HP 감소
        ReturnValue.push_back({ TotalDmg, it });
    }

    return ReturnValue;
}

// --------------------------
// 실제 데미지 적용 (HP 차감)
// --------------------------
void DamageLib::Calc_Dmg(float Calc_Dmg, ObjectClass* T_Obj)
{
    float T_Hp = T_Obj->Get_Hp();

    if (T_Hp <= Calc_Dmg)
    {
        T_Obj->Set_Hp(0.0f); // 사망 처리
    }
    else
    {
        T_Obj->Set_Hp(T_Hp - Calc_Dmg); // 체력 감소
    }
}
