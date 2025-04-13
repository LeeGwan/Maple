#include "OrangeMushroom.h"

// --------------------------------------------
// 생성자: 고유 이름 생성 및 속성 초기화
// --------------------------------------------
OrangeMushroom::OrangeMushroom(const std::string& _Name, float Random_x)
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    // 고유 이름 생성: Monster_이름_분+밀리초
    Name = "Monster_" + _Name + "_" + std::to_string(st.wMinute) + std::to_string(st.wMilliseconds);

    // 기본 상태 초기화
    Moving = false;
    Jump = false;
    MovingAndJmp = false;
    Pos = { Random_x, 565.0f };
    MaxHp = Hp = 80.0f;
    Can_Delete = 0.0;
    BasicDmg = 6.0f;
    M_direction = 0.0f;
    AttackSpeed = 1.5f;
    Speed = 60.0f;
    JumpPower = 300.948f;
    See_Radius = 150.0f;
    Radius = 45.0f;

    // 애니메이션 프레임 수 설정: Stand, Alert, Move, Jump, Attack, Die, Hit
    std::vector<int> Animaion_Count{ 2, 1, 3, 0, 0, 3, 0 };
    MonsterRenders.Init(_Name, Pos, Animaion_Count);
    MonsterRenders.Set_Current_AnimationType_State(AnimationType::Stand);

    InitializeCriticalSection(&cs);
}

// --------------------------------------------
// 소멸자: 크리티컬 섹션 해제
// --------------------------------------------
OrangeMushroom::~OrangeMushroom()
{
    DeleteCriticalSection(&cs);
}

// --------------------------------------------
// 업데이트: HP 바 및 애니메이션 처리
// --------------------------------------------
void OrangeMushroom::Update()
{
    MonsterRenders.Draw_Hp_Bar(Pos, MaxHp, Hp);
    MonsterRenders.Update(Pos, M_direction);
}

// --------------------------------------------
// AI 동작 실행
// --------------------------------------------
void OrangeMushroom::Start_Monster_Action(ObjectClass* My, double DeltaTime)
{
    MonsterAct.MonsterAI_Start(My, DeltaTime);
}

// --------------------------------------------
// 삭제 타이머 누적
// --------------------------------------------
void OrangeMushroom::Set_Can_Delete_Time(double DeltaTime)
{
    EnterCriticalSection(&cs);
    Can_Delete += DeltaTime;
    LeaveCriticalSection(&cs);
}

// --------------------------------------------
// Getter 함수들
// --------------------------------------------
std::string OrangeMushroom::Get_Name()               { return Name; }
float OrangeMushroom::Get_Hp()                       { return Hp; }
float OrangeMushroom::Get_Xp()                       { return 20.1f; }
Vec2 OrangeMushroom::Get_Pos()                       { return Pos; }
bool OrangeMushroom::Is_Moving()                     { return Moving; }
bool OrangeMushroom::Is_Jump()                       { return Jump; }
bool OrangeMushroom::Is_MovingAndJmp()               { return MovingAndJmp; }
float OrangeMushroom::Get_Speed()                    { return Speed; }
float OrangeMushroom::Get_JumpPower()                { return JumpPower; }
float OrangeMushroom::Get_AttackSpeed()              { return AttackSpeed; }
float OrangeMushroom::Get_BasicDmg()                 { return BasicDmg; }
float OrangeMushroom::Get_See_Radius()               { return See_Radius; }
float OrangeMushroom::Get_Radius()                   { return Radius; }
double OrangeMushroom::Get_direction()               { return M_direction; }
double OrangeMushroom::Get_Can_Delete_Time()         { return Can_Delete; }
AnimationType OrangeMushroom::Get_Animaion_State()   { return MonsterRenders.Get_Current_AnimationType_State(); }
Vec2 OrangeMushroom::Get_Size()                      { return MonsterRenders.Get_Size(); }

// --------------------------------------------
// Setter 및 상태 변경 함수들 (쓰레드 보호)
// --------------------------------------------
void OrangeMushroom::Set_Hp(float _Hp)
{
    EnterCriticalSection(&cs);
    Hp = _Hp;
    LeaveCriticalSection(&cs);
}

void OrangeMushroom::Set_Pos(Vec2& Vec)
{
    EnterCriticalSection(&cs);
    Pos = Vec;
    LeaveCriticalSection(&cs);
}

void OrangeMushroom::Set_Speed(float _Speed)
{
    EnterCriticalSection(&cs);
    Speed = _Speed;
    LeaveCriticalSection(&cs);
}

void OrangeMushroom::Set_JumpPower(float _JumpPower)
{
    EnterCriticalSection(&cs);
    JumpPower = _JumpPower;
    LeaveCriticalSection(&cs);
}

void OrangeMushroom::Set_AttackSpeed(float _AttackSpeed)
{
    EnterCriticalSection(&cs);
    AttackSpeed = _AttackSpeed;
    LeaveCriticalSection(&cs);
}

void OrangeMushroom::Set_direction(double _direction)
{
    EnterCriticalSection(&cs);
    M_direction = _direction;
    LeaveCriticalSection(&cs);
}

void OrangeMushroom::Change_Jump(bool st)
{
    EnterCriticalSection(&cs);
    Jump = st;
    LeaveCriticalSection(&cs);
}

void OrangeMushroom::Change_MovingAndJmp(bool st)
{
    EnterCriticalSection(&cs);
    MovingAndJmp = st;
    LeaveCriticalSection(&cs);
}

// --------------------------------------------
// 상태 전환 시 애니메이션 상태도 변경
// --------------------------------------------
void OrangeMushroom::Set_State(AnimationType _state)
{
    AnimationType CurrentState = MonsterRenders.Get_Current_AnimationType_State();
    if (CurrentState == _state) return;

    EnterCriticalSection(&cs);
    switch (_state)
    {
        case AnimationType::Stand:
            Moving = false;
            break;
        case AnimationType::Move:
            Moving = true;
            break;
        case AnimationType::Alert:
        case AnimationType::Jump:
        default:
            break;
    }

    MonsterRenders.Set_Current_AnimationType_State(_state);
    LeaveCriticalSection(&cs);
}
