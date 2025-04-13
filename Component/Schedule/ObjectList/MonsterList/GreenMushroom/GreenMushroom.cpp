#include "GreenMushroom.h"

// -------------------------------------------
// 생성자: 이름 설정 및 초기 속성값 정의
// -------------------------------------------
GreenMushroom::GreenMushroom(const std::string& _Name, float Random_x)
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    // 고유한 몬스터 이름 생성 (분 + 밀리초)
    Name = "Monster_" + _Name + "_" + std::to_string(st.wMinute) + std::to_string(st.wMilliseconds);

    // 상태 초기화
    Moving = false;
    Jump = false;
    MovingAndJmp = false;
    Pos = { Random_x, 565.0f };  // 초기 위치
    MaxHp = 100.0f;
    Hp = 100.0f;
    Can_Delete = 0.0;
    BasicDmg = 8.0f;
    M_direction = 0.0f;
    AttackSpeed = 1.5f;
    Speed = 50.0f;
    JumpPower = 300.948f;
    See_Radius = 150.0f;
    Radius = 45.0f;

    // 애니메이션 프레임 수 설정 (Stand, Alert, Move, Jump, Attack, Die, Hit)
    std::vector<int> Animaion_Count{ 3, 1, 4, 0, 0, 4, 1 };

    // 렌더러 초기화
    MonsterRenders.Init(_Name, Pos, Animaion_Count);
    MonsterRenders.Set_Current_AnimationType_State(AnimationType::Stand);

    InitializeCriticalSection(&cs);
}

// -------------------------------------------
// 소멸자: 크리티컬 섹션 해제
// -------------------------------------------
GreenMushroom::~GreenMushroom()
{
    DeleteCriticalSection(&cs);
}

// -------------------------------------------
// 프레임 업데이트 (HP바 + 렌더링)
// -------------------------------------------
void GreenMushroom::Update()
{
    MonsterRenders.Draw_Hp_Bar(Pos, MaxHp, Hp);
    MonsterRenders.Update(Pos, M_direction);
}

// -------------------------------------------
// AI 동작 실행
// -------------------------------------------
void GreenMushroom::Start_Monster_Action(ObjectClass* My, double DeltaTime)
{
    MonsterAct.MonsterAI_Start(My, DeltaTime);
}

// -------------------------------------------
// Getter 함수
// -------------------------------------------
std::string GreenMushroom::Get_Name()                    { return Name; }
float GreenMushroom::Get_Hp()                            { return Hp; }
float GreenMushroom::Get_Xp()                            { return 25.05f; }
Vec2 GreenMushroom::Get_Pos()                            { return Pos; }
bool GreenMushroom::Is_Moving()                          { return Moving; }
bool GreenMushroom::Is_Jump()                            { return Jump; }
bool GreenMushroom::Is_MovingAndJmp()                    { return MovingAndJmp; }
float GreenMushroom::Get_Speed()                         { return Speed; }
float GreenMushroom::Get_JumpPower()                     { return JumpPower; }
float GreenMushroom::Get_AttackSpeed()                   { return AttackSpeed; }
float GreenMushroom::Get_BasicDmg()                      { return BasicDmg; }
float GreenMushroom::Get_See_Radius()                    { return See_Radius; }
float GreenMushroom::Get_Radius()                        { return Radius; }
double GreenMushroom::Get_direction()                    { return M_direction; }
double GreenMushroom::Get_Can_Delete_Time()              { return Can_Delete; }
AnimationType GreenMushroom::Get_Animaion_State()        { return MonsterRenders.Get_Current_AnimationType_State(); }
Vec2 GreenMushroom::Get_Size()                           { return MonsterRenders.Get_Size(); }

// -------------------------------------------
// Setter 및 상태 전환
// -------------------------------------------
void GreenMushroom::Set_Hp(float _Hp)
{
    EnterCriticalSection(&cs);
    Hp = _Hp;
    LeaveCriticalSection(&cs);
}

void GreenMushroom::Set_Pos(Vec2& Vec)
{
    EnterCriticalSection(&cs);
    Pos = Vec;
    LeaveCriticalSection(&cs);
}

void GreenMushroom::Set_State(AnimationType _state)
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
        // Alert, Jump 등 필요한 경우 확장 가능
        default:
            break;
    }
    MonsterRenders.Set_Current_AnimationType_State(_state);
    LeaveCriticalSection(&cs);
}

void GreenMushroom::Set_Speed(float _Speed)
{
    EnterCriticalSection(&cs);
    Speed = _Speed;
    LeaveCriticalSection(&cs);
}

void GreenMushroom::Set_JumpPower(float _JumpPower)
{
    EnterCriticalSection(&cs);
    JumpPower = _JumpPower;
    LeaveCriticalSection(&cs);
}

void GreenMushroom::Set_AttackSpeed(float _AttackSpeed)
{
    EnterCriticalSection(&cs);
    AttackSpeed = _AttackSpeed;
    LeaveCriticalSection(&cs);
}

void GreenMushroom::Set_direction(double _direction)
{
    EnterCriticalSection(&cs);
    M_direction = _direction;
    LeaveCriticalSection(&cs);
}

// -------------------------------------------
// 상태 변화 함수 (Jump / Move & Jump)
// -------------------------------------------
void GreenMushroom::Change_Jump(bool st)
{
    EnterCriticalSection(&cs);
    Jump = st;
    LeaveCriticalSection(&cs);
}

void GreenMushroom::Change_MovingAndJmp(bool st)
{
    EnterCriticalSection(&cs);
    MovingAndJmp = st;
    LeaveCriticalSection(&cs);
}

// -------------------------------------------
// 삭제 타이머 증가
// -------------------------------------------
void GreenMushroom::Set_Can_Delete_Time(double DeltaTime)
{
    EnterCriticalSection(&cs);
    Can_Delete += DeltaTime;
    LeaveCriticalSection(&cs);
}
