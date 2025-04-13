#include "PlayerObject.h"

// ------------------------------------------
// 생성자: 플레이어 기본 정보 및 상태 초기화
// ------------------------------------------
PlayerObject::PlayerObject(const std::string& _Name)
{
    PlayerName = L"이관호";
    CurrentLv = 0;

    // 로그인 서버에서 현재 레벨 정보 가져오기
    Lv = G_ctx->LoginCheck.Get_Level();

    // 기본 설정
    Name = _Name;
    Can_Delete = 0.0;
    Moving = false;
    Jump = false;
    immortal = false;
    MovingAndJmp = false;
    Is_Level_Up = false;

    Pos = { 800.0f, 565.0f }; // 초기 위치
    MaxHp = Hp = 10000.0f;
    Mp = 1000.0f;
    Xp = 0.0f;
    MaxXp = 100.0f * Lv;

    direction = 0.0;
    Speed = 80.0f;
    JumpPower = 300.948f;
    Radius = 70.0f;

    Str = 4;
    Dex = 4;
    BasicDmg = 10.0f;
    AttackSpeed = 0.375f * static_cast<float>(Dex);

    // 애니메이션 프레임 수 (Stand, Alert, Move, Jump, Attack, Die, Hit)
    std::vector<int> Animaion_Count{ 3, 3, 4, 1, 3, 0, 0 };

    // 렌더 초기화
    PlayerRenders.Init(Name, Pos, Animaion_Count);
    PlayerRenders.Set_Current_AnimationType_State(AnimationType::Stand);

    InitializeCriticalSection(&cs);
}

// ------------------------------------------
// 소멸자: 동기화 객체 해제
// ------------------------------------------
PlayerObject::~PlayerObject()
{
    DeleteCriticalSection(&cs);
}

// ------------------------------------------
// 스킬 계산 실행
// ------------------------------------------
void PlayerObject::Start_Skill_Calc(ObjectClass* My, double DeltaTime)
{
    SkillList.Update_For_Calc(My, DeltaTime);
}

// ------------------------------------------
// 무적 상태 설정
// ------------------------------------------
bool PlayerObject::Set_immortal(bool State)
{
    return immortal = State;
}

// ------------------------------------------
// 레벨 설정 (외부에서 수동 설정 시 사용)
// ------------------------------------------
void PlayerObject::Set_Is_Lv_Up(bool _Lv)
{
    Lv = _Lv;
}

// ------------------------------------------
// 삭제 대기 시간 증가
// ------------------------------------------
void PlayerObject::Set_Can_Delete_Time(double DeltaTime)
{
    EnterCriticalSection(&cs);
    Can_Delete += DeltaTime;
    LeaveCriticalSection(&cs);
}

// ------------------------------------------
// 상태에 따른 레벨업 체크 및 스탯 반영
// ------------------------------------------
void PlayerObject::Check_State()
{
    MaxXp = 60.0f * Lv;

    if (CurrentLv != Lv)
    {
        if (Xp >= MaxXp)
        {
            Xp -= MaxXp;
            CurrentLv = Lv;
            Lv++;
            Str += 10;
            Dex += 10;
            MaxHp += 100.0f;
            Hp = MaxHp;
            Is_Level_Up = true;

            std::string LevelUpSound = BackgroundMusicPath + "LevelUp.mp3";
            G_ctx->G_SoundList.PlayEffectSound(LevelUpSound, 3.0, 0.7f);
            G_ctx->LoginCheck.Set_Level(Lv);
        }
    }
}

// ------------------------------------------
// 매 프레임 상태 업데이트 및 렌더링
// ------------------------------------------
void PlayerObject::Update()
{
    Check_State();

    if (Is_Level_Up)
    {
        PlayerRenders.Draw_LevelUp(Pos, &Is_Level_Up);
    }

    PlayerRenders.Draw_Lv(Lv);
    PlayerRenders.Draw_Hp_Bar(MaxHp, Hp);
    PlayerRenders.Draw_Mp_Bar(1000.0f, 1000.0f);
    PlayerRenders.Draw_Xp_Bar(MaxXp, Xp);
    PlayerRenders.Update(Pos, direction);
    PlayerRenders.Draw_Name(Pos, PlayerName);

    SkillList.Update_For_Renderring(Pos);
}

// ------------------------------------------
// 상태 Get 함수들
// ------------------------------------------
std::string PlayerObject::Get_Name()                { return Name; }
bool PlayerObject::Get_Is_Lv_Up()                   { return false; }
int PlayerObject::Get_Lv()                          { return Lv; }
float PlayerObject::Get_Hp()                        { return Hp; }
float PlayerObject::Get_Mp()                        { return Mp; }
float PlayerObject::Get_Xp()                        { return Xp; }
Vec2 PlayerObject::Get_Pos()                        { return Pos; }
bool PlayerObject::Is_Moving()                      { return Moving; }
bool PlayerObject::Is_Jump()                        { return Jump; }
bool PlayerObject::Is_MovingAndJmp()                { return MovingAndJmp; }
float PlayerObject::Get_BasicDmg()                  { return BasicDmg; }
float PlayerObject::Get_See_Radius()                { return 0.0f; }
float PlayerObject::Get_Radius()                    { return Radius; }
int PlayerObject::Get_Str()                         { return Str; }
int PlayerObject::Get_Dex()                         { return Dex; }
float PlayerObject::Get_Speed()                     { return Speed; }
float PlayerObject::Get_JumpPower()                 { return JumpPower; }
float PlayerObject::Get_AttackSpeed()               { return AttackSpeed; }
double PlayerObject::Get_Can_Delete_Time()          { return Can_Delete; }
double PlayerObject::Get_direction()                { return direction; }
bool PlayerObject::Get_immortal()                   { return immortal; }
AnimationType PlayerObject::Get_Animaion_State()    { return PlayerRenders.Get_Current_AnimationType_State(); }
Vec2 PlayerObject::Get_Size()                       { return PlayerRenders.Get_Size(); }

// ------------------------------------------
// 상태 Set 함수들 (쓰레드 동기화 포함)
// ------------------------------------------
void PlayerObject::Set_Hp(float _Hp)
{
    EnterCriticalSection(&cs);
    Hp = _Hp;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_Mp(float _Mp)
{
    EnterCriticalSection(&cs);
    Mp = _Mp;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_Xp(float _Xp)
{
    EnterCriticalSection(&cs);
    Xp += _Xp;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_Pos(Vec2& Vec)
{
    EnterCriticalSection(&cs);
    Pos = Vec;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_State(AnimationType _state)
{
    AnimationType CurrentState = PlayerRenders.Get_Current_AnimationType_State();
    if (CurrentState == _state) return;

    EnterCriticalSection(&cs);
    switch (_state)
    {
        case AnimationType::Stand: Moving = false; break;
        case AnimationType::Move:  Moving = true;  break;
        case AnimationType::Alert:
        case AnimationType::Jump:
        default: break;
    }
    PlayerRenders.Set_Current_AnimationType_State(_state);
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_Speed(float _Speed)
{
    EnterCriticalSection(&cs);
    Speed = _Speed;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_JumpPower(float _JumpPower)
{
    EnterCriticalSection(&cs);
    JumpPower = _JumpPower;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_AttackSpeed(float _AttackSpeed)
{
    EnterCriticalSection(&cs);
    AttackSpeed = _AttackSpeed;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_direction(double _direction)
{
    EnterCriticalSection(&cs);
    direction = _direction;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_Str(int _Str)
{
    EnterCriticalSection(&cs);
    Str = _Str;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_Dex(int _Dex)
{
    EnterCriticalSection(&cs);
    Dex = _Dex;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Change_Jump(bool st)
{
    EnterCriticalSection(&cs);
    Jump = st;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Change_MovingAndJmp(bool st)
{
    EnterCriticalSection(&cs);
    MovingAndJmp = st;
    LeaveCriticalSection(&cs);
}

void PlayerObject::Set_Lv(int _Lv)
{
    Lv = _Lv;
}
