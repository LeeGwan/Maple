#include "OrangeMushroom.h"


OrangeMushroom::OrangeMushroom(const string& _Name, float Random_x)
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	string Check_NAME = "Monster_" + _Name + "_" + to_string(st.wMinute) + to_string(st.wMilliseconds);
	Name = Check_NAME;
	Moving = false;
	Jump = false;
	MovingAndJmp = false;
	Pos = { Random_x,565.0f };//초기위치
	MaxHp = 80.0f;
	Hp = 80.0f;
	Can_Delete = 0.0;
	BasicDmg = 6.0f;
	M_direction = 0.0f;
	AttackSpeed = 1.5f;
	Speed = 60.0f;
	JumpPower = 300.948f;
	
	See_Radius = 150.0f;
	Radius = 45.0f;
	vector<int> Animaion_Count{ 2,1,3,0,0,3,0 };
	MonsterRenders.Init(_Name, Pos, Animaion_Count);
	MonsterRenders.Set_Current_AnimationType_State(AnimationType::Stand);

	InitializeCriticalSection(&cs);
}

void OrangeMushroom::Set_Can_Delete_Time(double DeltaTime)
{
	EnterCriticalSection(&cs);
	Can_Delete += DeltaTime;
	LeaveCriticalSection(&cs);
}

OrangeMushroom::~OrangeMushroom()
{
	DeleteCriticalSection(&cs);
}

string OrangeMushroom::Get_Name()
{

	string Out_Name = Name;

	return Out_Name;
}

void OrangeMushroom::Update()
{

	MonsterRenders.Draw_Hp_Bar(Pos, MaxHp, Hp);

	MonsterRenders.Update(Pos, M_direction);


}

void OrangeMushroom::Start_Monster_Action(ObjectClass* My, double DeltaTime)
{
	MonsterAct.MonsterAI_Start(My, DeltaTime);
}

float OrangeMushroom::Get_Hp()
{

	float Out_Hp = Hp;

	return Out_Hp;
}



float OrangeMushroom::Get_Xp()
{
	return 20.1f;
}



Vec2 OrangeMushroom::Get_Pos()
{
	return Pos;
}

bool OrangeMushroom::Is_Moving()
{
	return Moving;
}

bool OrangeMushroom::Is_Jump()
{
	return Jump;
}

bool OrangeMushroom::Is_MovingAndJmp()
{
	return MovingAndJmp;
}



float OrangeMushroom::Get_Speed()
{
	return Speed;
}

float OrangeMushroom::Get_JumpPower()
{
	return JumpPower;
}

float OrangeMushroom::Get_AttackSpeed()
{
	return AttackSpeed;
}

float OrangeMushroom::Get_BasicDmg()
{
	return BasicDmg;;
}

float OrangeMushroom::Get_See_Radius()
{
	return See_Radius;
}

float OrangeMushroom::Get_Radius()
{
	return Radius;
}


double OrangeMushroom::Get_direction()
{
	return M_direction;
}

double OrangeMushroom::Get_Can_Delete_Time()
{
	return Can_Delete;
}

AnimationType OrangeMushroom::Get_Animaion_State()
{
	return MonsterRenders.Get_Current_AnimationType_State();
}

Vec2 OrangeMushroom::Get_Size()
{
	return MonsterRenders.Get_Size();
}



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

void OrangeMushroom::Set_State(AnimationType _state)
{
	AnimationType CurrentState = MonsterRenders.Get_Current_AnimationType_State();
	if (CurrentState == _state)return;
	EnterCriticalSection(&cs);

	switch (_state)
	{
	case AnimationType::Stand:
		Moving = false;
		break;

	case AnimationType::Alert:
		break;

	case AnimationType::Move:
		Moving = true;
		break;

	case AnimationType::Jump:

		break;

	default:
		break;
	}
	MonsterRenders.Set_Current_AnimationType_State(_state);
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





