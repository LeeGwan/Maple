#include "Calculation.h"
#include "../../../../Global.h"
#include "../Input/Input.h"
#define NOMINMAX
#include <iostream>
#include <algorithm>

// ----------------------
// 생성자 / 초기화 함수
// ----------------------

Calculation::~Calculation() {}

void Calculation::Calculation_Init() 
{
    Ec = 0.0;
    AttackTime = 0.0;
    Velocity_y = 0.0;
}

// ----------------------
// 플레이어 계산 루프
// ----------------------

void Calculation::Player_For_Calc(const string& _Name)
{
    Input KeyBoard;
    const double FixedTime = 0.016; // 60FPS 기준 프레임 단위
    double Accumulator = 0.0;
    double CheckAccumulator = 0.0; // 키보드 누적 시간

    // 계산 가능 상태가 될 때까지 대기
    while (!G_ctx->CanUseCalc) 
    {
        Sleep(10);
    }

    // 플레이어 행동 처리 루프
    while (true) 
    {
        ObjectClass* Obj = G_ctx->G_ObjectManager->Get_Find_obj(_Name);
        G_ctx->Timer_Update();
        Accumulator += G_ctx->GetDeltaTime();

        while (Accumulator >= FixedTime) 
        {
            Obj->Start_Skill_Calc(Obj, FixedTime); // 스킬 계산
            G_ctx->DmgObject.Set_NowDmg_Calc(FixedTime);  // 데미지 오브젝트 설정
            G_ctx->DmgObject.Check_Delete_NowDmg(FixedTime); // 데미지 삭제 체크
            Handle_Alert(Obj, FixedTime); // 무적 처리

            Calc_For_Dmg(Obj, FixedTime); // 공격 계산
            CheckAccumulator = KeyBoard.Input_KeyBoard(Obj, FixedTime); // 키 입력 처리

            Calc_For_Vector(Obj, FixedTime, CheckAccumulator); // 위치 및 물리 계산

            Accumulator -= FixedTime;
        }

        timeBeginPeriod(1);
        Sleep(1);
        timeEndPeriod(1);
    }
}

// ----------------------
// 몬스터 계산 루프
// ----------------------

void Calculation::Monster_For_Calc(const string& _Name)
{
    Timer Monster_Timer;
    const double FixedTime = 0.016;
    double Accumulator = 0.0;

    while (!G_ctx->CanUseCalc) 
    {
        Sleep(10);
    }

    vector<ObjectClass*> M_obj;

    while (true)
    {
        M_obj = G_ctx->G_ObjectManager->Get_Find_Monster();
        Monster_Timer.Update();
        Accumulator += Monster_Timer.GetDeltaTime();

        if (M_obj.empty()) continue;

        while (Accumulator >= FixedTime)
        {
            for (auto& It : M_obj)
            {
                if (!It || !IsValidPtr(It)) continue;
                if (It->Get_Animaion_State() == AnimationType::Die) continue;

                It->Start_Monster_Action(It, FixedTime); // AI 동작 계산
            }
            Accumulator -= FixedTime;
        }

        timeBeginPeriod(1);
        Sleep(1);
        timeEndPeriod(1);
    }
}

// ----------------------
// 계산 시작점
// ----------------------

void Calculation::Start_Calc(const string& _Name) 
{
    if (_Name == "Player")
        Player_For_Calc(_Name);
    else
        Monster_For_Calc(_Name);
}

// ----------------------
// 위치 이동 및 벡터 계산
// ----------------------

void Calculation::Calc_For_Vector(ObjectClass* Obj, double DeltaTime, double Move_Time) 
{
    ImVec2 Map_Size = G_ctx->MakeWindows->Get_MapSize();
    Vec2 Obj_Pos = Obj->Get_Pos();
    Vec2 Obj_Size = Obj->Get_Size();

    Vec2 Calc_Pos = Calc_Event(Obj, DeltaTime, Move_Time);

    // 예외 상태
    if (Calc_Pos.x == -999.0f) return;

    // X축 범위 제한 (클램프 처리)
    Calc_Pos.x = clamp(Calc_Pos.x, -62.0f, Map_Size.x);

    Obj->Set_Pos(Calc_Pos); // 위치 갱신
}

// 이동, 점프 등 애니메이션에 따른 위치 계산
Vec2 Calculation::Calc_Event(ObjectClass* Obj, double DeltaTime, double Move_Time) 
{
    float AttackSpeed = Obj->Get_AttackSpeed();
    double D_AttackSpeed = static_cast<double>(AttackSpeed);
    float Speed = Obj->Get_Speed();
    AnimationType Current_State = Obj->Get_Animaion_State();
    Vec2 Calc(Obj->Get_Pos().x, Obj->Get_Pos().y);
    const double Gravi = 1000.0;

    if (Current_State == AnimationType::Move)
    {
        Handle_Movement(Obj, Current_State, Speed, Move_Time, DeltaTime, Calc, Ec, Gravi);
        AttackTime = 0.0;
    }
    else if (Current_State == AnimationType::Jump)
    {
        Apply_JumpPhysics(Obj, DeltaTime, Calc, Ec, Gravi);
        AttackTime = 0.0;
    }
    else 
    {
        return { -999.0f, -999.0f }; // 예외 리턴
    }

    return Calc;
}

// ----------------------
// 공격 및 데미지 계산
// ----------------------

void Calculation::Calc_For_Dmg(ObjectClass* MyObj, double DeltaTime)
{
    float AttackSpeed = MyObj->Get_AttackSpeed();
    double D_AttackSpeed = static_cast<double>(AttackSpeed);
    AnimationType Current_State = MyObj->Get_Animaion_State();

    if (Current_State == AnimationType::Attack)
    {
        AttackTime += DeltaTime;

        if (AttackTime > 0.41 * D_AttackSpeed)
        {
            Dmg_Result Result = DmgCLass.Damage_Update(MyObj);
            if (!Result.T_obj) return;

            Animaion_Dmg Set_Dmg_Infor = {
                Result.Dmg, DeltaTime, Result.T_obj->Get_Pos()
            };

            G_ctx->DmgObject.Set_NowDmg(Set_Dmg_Infor);

            std::string AttackSound = AttackMusicPath + "Attack.mp3";
            G_ctx->G_SoundList.PlayEffectSound(AttackSound, 1.0, 2.7f);

            if (Result.T_obj->Get_Hp() <= 0.0f)
            {
                MyObj->Set_Xp(Result.T_obj->Get_Xp());
            }
            else
            {
                Result.T_obj->Set_State(AnimationType::Alert);
                MyObj->Set_State(AnimationType::Stand);
            }

            AttackTime = 0.0;
        }
    }
}

// ----------------------
// 점프 중 위치 계산
// ----------------------

void Calculation::Apply_JumpPhysics(ObjectClass* Obj, double DeltaTime, Vec2& Calc, double& Ec, double Gravi) 
{
    if (!Obj->Is_Jump())
    {
        Obj->Change_Jump(true);
        Velocity_y = -static_cast<double>(Obj->Get_JumpPower());

        std::string JumpMusicPath = BackgroundMusicPath + "Jump.mp3";
        G_ctx->G_SoundList.PlayEffectSound(JumpMusicPath, 1.0, 0.7f);
    }

    Calc.y = 565.0f + Velocity_y * Ec + 0.5 * Gravi * Ec * Ec;
    Ec += DeltaTime;

    // 바닥에 도달한 경우 점프 종료
    if (Calc.y > 565.0f) 
    {
        Ec = 0.0;
        Obj->Change_MovingAndJmp(false);
        Obj->Change_Jump(false);
        Calc.y = 565.0f;
    }
}

// ----------------------
// 이동 상태 처리
// ----------------------

void Calculation::Handle_Movement(ObjectClass* Obj, AnimationType Current_State, float Speed,
    double Move_Time, double DeltaTime, Vec2& Calc, double& Ec, double Gravi)
{
    double Velocity_x = 0.0;
    float F_Velocity_x = 0.0f;

    bool isJumping = Obj->Is_MovingAndJmp();
    double direction = Obj->Get_direction();

    if (isJumping)
    {
        Velocity_x = direction * Speed * Move_Time; // 점프 중엔 이동량 증가
    }
    else
    {
        Velocity_x = direction * Speed;
    }

    F_Velocity_x = static_cast<float>(Velocity_x * DeltaTime);
    Calc.x += F_Velocity_x;

    if (isJumping)
    {
        Apply_JumpPhysics(Obj, DeltaTime, Calc, Ec, Gravi);
    }
    else if (Calc.y > 565.0f)
    {
        Calc.y = 0.0f; // 지형 밖 예외처리
    }
}

// ----------------------
// 무적 상태 처리
// ----------------------

void Calculation::Handle_Alert(ObjectClass* Obj, double DeltaTime)
{
    if (!Obj->Get_immortal()) return;

    AlertTime += DeltaTime;

    if (AlertTime > 1.5)
    {
        Obj->Set_immortal(false);
        AlertTime = 0.0;
    }
}
