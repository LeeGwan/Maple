#include "GreenMushroom.h"

GreenMushroom::GreenMushroom(const string& _Name,float Random_x)
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	string Check_NAME = "Monster_"+_Name + "_" + to_string(st.wMinute) + to_string(st.wMilliseconds);
	Name = Check_NAME;
	Moving = false;
	Jump = false;
	MovingAndJmp = false;
	Pos = { Random_x,565.0f };//초기위치
	MaxHp = 100.0f;
	Hp = 100.0f;
	Can_Delete = 0.0;
	BasicDmg=8.0f;
	M_direction = 0.0f;
	AttackSpeed = 1.5f;
	Speed = 50.0f;
	JumpPower = 300.948f;
	
	See_Radius=150.0f;
	Radius=45.0f;
	vector<int> Animaion_Count{ 3,1,4,0,0,4,1 };
	MonsterRenders.Init(_Name, Pos, Animaion_Count);
	MonsterRenders.Set_Current_AnimationType_State(AnimationType::Stand);

	InitializeCriticalSection(&cs);
}



void GreenMushroom::Set_Can_Delete_Time(double DeltaTime)
{
	EnterCriticalSection(&cs);
	Can_Delete += DeltaTime;
	LeaveCriticalSection(&cs);
}

GreenMushroom::~GreenMushroom()
{
	DeleteCriticalSection(&cs);
}

string GreenMushroom::Get_Name()
{

	string Out_Name = Name;

	return Out_Name;
}

void GreenMushroom::Update()
{

	MonsterRenders.Draw_Hp_Bar(Pos, MaxHp, Hp);

	MonsterRenders.Update(Pos, M_direction);


}

void GreenMushroom::Start_Monster_Action(ObjectClass* My, double DeltaTime)
{
	MonsterAct.MonsterAI_Start(My, DeltaTime);
}


float GreenMushroom::Get_Hp()
{

	float Out_Hp = Hp;

	return Out_Hp;
}



float GreenMushroom::Get_Xp()
{
	return 25.05f;
}



Vec2 GreenMushroom::Get_Pos()
{
	return Pos;
}

bool GreenMushroom::Is_Moving()
{
	return Moving;
}

bool GreenMushroom::Is_Jump()
{
	return Jump;
}

bool GreenMushroom::Is_MovingAndJmp()
{
	return MovingAndJmp;
}



float GreenMushroom::Get_Speed()
{
	return Speed;
}

float GreenMushroom::Get_JumpPower()
{
	return JumpPower;
}

float GreenMushroom::Get_AttackSpeed()
{
	return AttackSpeed;
}

float GreenMushroom::Get_BasicDmg()
{
	return BasicDmg;;
}

float GreenMushroom::Get_See_Radius()
{
	return See_Radius;
}

float GreenMushroom::Get_Radius()
{
	return Radius;
}


double GreenMushroom::Get_direction()
{
	return M_direction;
}

double GreenMushroom::Get_Can_Delete_Time()
{
	return Can_Delete;
}

AnimationType GreenMushroom::Get_Animaion_State()
{
	return MonsterRenders.Get_Current_AnimationType_State();
}

Vec2 GreenMushroom::Get_Size()
{
	return MonsterRenders.Get_Size();
}



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




