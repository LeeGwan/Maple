#include "SkillObject.h"
#include "../../../Engine/Input/Input.h"

// ---------------------------------------------
// 스킬 애니메이션 렌더링 처리
// ---------------------------------------------
void SkillObject::Update_For_Renderring(Vec2 Pos)
{
    SkillState CurrentState = Get_SkillState();
    auto SkillAnimation1    = Get_SkillAnimation1();
    auto SkillHitAnimation  = Get_SkillHitAnimation();
    Skill_1 = &Get_Skill_1();
    Hit_Skill = &Get_Hit_Skill();
    bool* IsLastFrame1 = Get_IsLastFrame1();
    bool* IsLastFrame2 = Get_IsLastFrame2();

    // ----------------------------
    // 스킬 시전 애니메이션 출력
    // ----------------------------
    if (CurrentState == SkillState::Casting1)
    {
        Vec2 ChangePos;
        ID3D11ShaderResourceView* FrameRect;

        // "Q_" 계열 스킬은 특수 처리
        if (Who_Am_I("Q_"))
            FrameRect = Skill_1->Start_Animation_Update_For_Skiil(SkillAnimation1, IsLastFrame1, true);
        else
            FrameRect = Skill_1->Start_Animation_Update_For_Skiil(SkillAnimation1, IsLastFrame1);

        // "T_"는 화면 고정, 그 외엔 플레이어 기준 위치 보정
        ChangePos = Who_Am_I("T_") ? Get_SkillPos() : (Pos - Get_SkillPos());

        // 출력
        G_ctx->MakeRender->Draw_Image(FrameRect, ChangePos, Get_SkillSize(), 0.0);

        // 시전 후 데미지 상태로 전환
        if (!*IsLastFrame1)
        {
            Set_SkillState(SkillState::Damaging);
        }
    }

    // ----------------------------
    // 히트 애니메이션 출력
    // ----------------------------
    if (CurrentState == SkillState::Hitting)
    {
        ID3D11ShaderResourceView* FrameRect = Hit_Skill->Start_Animation_Update_For_Skiil(SkillHitAnimation, IsLastFrame2);

        for (const auto& it : TargetPos)
        {
            Vec2 ChangePos = it - Get_HitSkillPos();
            G_ctx->MakeRender->Draw_Image(FrameRect, ChangePos, Get_HitSkillSize(), 0.0);
        }

        // 히트 애니메이션 종료 → 초기화
        if (!*IsLastFrame1)
        {
            TargetPos.clear();
            TargetPos.shrink_to_fit();
            Set_SkillState(SkillState::Idle);
        }
    }
}

// ---------------------------------------------
// 스킬 로직 및 데미지 계산 처리
// ---------------------------------------------
void SkillObject::Update_For_Calc(ObjectClass* My, double DeltaTime)
{
    Input KeyBoard;

    double CoolTime     = Get_CoolTime();
    double MaxCoolTime  = Get_MaxCoolTime();
    SkillState CurrentState = Get_SkillState();
    Vec2 MyPos = My->Get_Pos();

    // ----------------------------
    // 키 입력으로 스킬 시전
    // ----------------------------
    if (KeyBoard.Input_SKillKeyBoard(Get_Key()) && CoolTime == 0.0 && CurrentState == SkillState::Idle)
    {
        Set_SkillState(SkillState::Casting1);
        Set_CoolTime(MaxCoolTime);

        // 스킬 사운드 효과
        if (Who_Am_I("T_"))
        {
            G_ctx->G_SoundList.PlayEffectSound(Get_SkillAnimation1_Path(), 7.0, 0.7f);
        }
        else if (Who_Am_I("Q_"))
        {
            // Q 스킬은 별도 사운드 없음 (예외 처리 가능)
        }
        else
        {
            G_ctx->G_SoundList.PlayEffectSound(Get_SkillAnimation1_Path(), 4.0, 0.7f);
        }
    }

    // ----------------------------
    // 쿨타임 진행 중
    // ----------------------------
    else if (CoolTime > 0.0)
    {
        Minus_CoolTime(DeltaTime);

        // 공격 판정 타이밍
        if (CurrentState == SkillState::Damaging)
        {
            float SkillRadius = Get_SkillRadius();
            float SkillDmg    = Get_SkillDmg();

            // 범위 내 적 탐색
            std::vector<ObjectClass*> TargetObj = CheckCollision.Check_Collision(MyPos, SkillRadius);
            if (TargetObj.empty()) return;

            // 데미지 처리
            std::vector<Dmg_Result> Result = DmgLib.Damage_Update(TargetObj, SkillDmg);
            for (const auto& it : Result)
            {
                TargetPos.push_back(it.T_obj->Get_Pos());

                Animaion_Dmg Set_Dmg_Infor = { it.Dmg, DeltaTime, it.T_obj->Get_Pos() };
                G_ctx->DmgObject.Set_NowDmg(Set_Dmg_Infor);

                if (it.T_obj->Get_Hp() <= 0.0f)
                {
                    My->Set_Xp(it.T_obj->Get_Xp());
                }
            }

            // 히트 애니메이션으로 전환
            Set_SkillState(SkillState::Hitting);
            G_ctx->G_SoundList.PlayEffectSound(Get_SkillHitAnimation_Path(), 1.0, 0.7f);
        }
