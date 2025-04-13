#include "MonsterAI.h"
#include "../../../../Global.h"
#include <algorithm>
#include "MonsterAI.h"
#include "../../../../Global.h"
#include <algorithm>

// -------------------------------------
// 생성자
// -------------------------------------
MonsterAI::MonsterAI()
{
    // 필요 시 초기값 세팅
}

// -------------------------------------
// 몬스터가 플레이어에게 경계 상태로 접근
// -------------------------------------
void MonsterAI::Monster_Alet(ObjectClass* My, double DeltaTime)
{
    ObjectClass* Target = G_ctx->G_ObjectManager->Get_Find_obj("Player");

    Vec2 TargetPos = Target->Get_Pos();
    Vec2 MyPos = My->Get_Pos();
    float Distance = MyPos.distance(TargetPos);

    Vec2 Get_normalized = (TargetPos - MyPos).normalized();
    float Check_Normal = Get_normalized.x;

    // 방향 설정: 오른쪽 -> 왼쪽 이동 / 왼쪽 -> 오른쪽 이동
    double Direction = (Check_Normal > 0) ? -1.0 : (Check_Normal < 0) ? 1.0 : 0.0;
    My->Set_direction(Direction);

    float Normalized = (100.f * DeltaTime) * Direction;
    Vec2 Path(MyPos.x + Normalized, MyPos.y);

    // 맵 밖으로 벗어나지 않도록 제한
    ImVec2 Map_Size = G_ctx->MakeWindows->Get_MapSize();
    Path.x = clamp(Path.x, 50.0f, Map_Size.x);

    AletTimer += DeltaTime;

    // 일정 시간 후 스탠딩 상태 전환
    if (AletTimer > 0.5f)
    {
        AletTimer -= 0.5f;
        My->Set_State(AnimationType::Stand);
    }
    else
    {
        My->Set_Pos(Path);
    }
}

// -------------------------------------
// 플레이어를 공격할 때 (피해 적용)
// -------------------------------------
void MonsterAI::Player_Alet(ObjectClass* My, double DeltaTime)
{
    Dmg_Result Result = DmgLib.Damage_Update(My);
    if (!Result.T_obj) return;

    // 대상이 경직 상태가 아니고 무적이 아닐 경우 데미지 적용
    if (Result.T_obj->Get_Animaion_State() != AnimationType::Alert && !Result.T_obj->Get_immortal())
    {
        Animaion_Dmg Set_Dmg_Infor = { Result.Dmg, DeltaTime, Result.T_obj->Get_Pos() };
        Set_Dmg_Infor.IsPlayer = true;
        G_ctx->DmgObject.Set_NowDmg(Set_Dmg_Infor);
    }

    // 일정 시간 무적 상태
    Result.T_obj->Set_immortal(true);
}

// -------------------------------------
// 몬스터 AI 루프 (상태 판별 및 이동)
// -------------------------------------
void MonsterAI::MonsterAI_Start(ObjectClass* My, double DeltaTime)
{
    float Radius = My->Get_Radius();
    float Can_See = My->Get_See_Radius();

    ObjectClass* Target = Collisions.Check_Collision(My, Can_See);
    Accumulator += DeltaTime;

    // 5초마다 랜덤 행동 설정
    if (Accumulator >= 5.0f || init == 0)
    {
        Random_Pos = Random_Act(My);
        init = 1;
        Accumulator -= 5.0f;
    }

    // 경계 상태면 해당 행동 실행 후 리턴
    if (My->Get_Animaion_State() == AnimationType::Alert)
    {
        Monster_Alet(My, DeltaTime);
        return;
    }

    // 기본 상태 처리 (대상 없으면 랜덤 행동)
    Vec2 MyPos = My->Get_Pos();
    Vec2 TargetPos = Target ? Target->Get_Pos() : Random_Pos;
    AnimationType AnyType = Target ? AnimationType::Move : Random_State;
    My->Set_State(AnyType);

    if (AnyType == AnimationType::Stand) return;

    float Distance = MyPos.distance(TargetPos);
    Vec2 Get_normalized = (TargetPos - MyPos).normalized();

    // 이동 방향 설정
    double Direction = 0.0;
    float Check_Normal = Target ? Get_normalized.x : LeftOrRight;
    Direction = (Check_Normal > 0) ? 1.0 : (Check_Normal < 0) ? -1.0 : 0.0;
    My->Set_direction(Direction);

    // 위치 이동
    float Normalized = (My->Get_Speed() * DeltaTime) * Direction;
    Vec2 Path(MyPos.x + Normalized, MyPos.y);

    ImVec2 Map_Size = G_ctx->MakeWindows->Get_MapSize();
    Path.x = clamp(Path.x, 50.0f, Map_Size.x);
    My->Set_Pos(Path);

    if (My->Get_Animaion_State() != AnimationType::Move)
    {
        My->Set_State(AnimationType::Move);
    }

    // 공격 반경 내 플레이어 탐색 후 공격 실행
    ObjectClass* Target_Check = Collisions.Check_Collision(My, Radius);
    if (Target_Check)
    {
        Player_Alet(My, DeltaTime);
    }
}

// -------------------------------------
// 몬스터 랜덤 행동 설정 (스탠드 or 이동)
// -------------------------------------
Vec2 MonsterAI::Random_Act(ObjectClass* My)
{
    Vec2 Move_Pos;
    RandonState = rand() % 2; // 0: Stand, 1: Move

    Vec2 My_Pos = My->Get_Pos();

    if (RandonState == 0)
    {
        Random_State = AnimationType::Stand;
        return { -999.f, -999.f };  // 스탠딩일 경우 이동 없음
    }
    else
    {
        Random_State = AnimationType::Move;
        Move_Pos = Random_Act_Calc(My_Pos, Random_State);
    }

    return Move_Pos;
}

// -------------------------------------
// 랜덤 위치 계산 (좌/우 방향 무작위 이동)
// -------------------------------------
Vec2 MonsterAI::Random_Act_Calc(Vec2 Pos, AnimationType State)
{
    Vec2 OutPutPos = Pos;
    int What_LeftOrRight = rand() % 2;
    int min = 10, max = 60;

    // 방향 랜덤 결정
    LeftOrRight = (What_LeftOrRight == 0) ? -1.0f : 1.0f;
    int Random_X = rand() % (max - min + 1) + min;

    OutPutPos.x += Random_X * LeftOrRight;
    return OutPutPos;
}
