#include "PlayerObject.h"




PlayerObject::PlayerObject(const string& _Name)
{
	PlayerName = L"이관호";
	CurrentLv = 0;
	Lv = G_ctx->LoginCheck.Get_Level();
	
	Name = _Name;
	Can_Delete = 0.0;
	Moving =false;
	Jump = false;
	immortal = false;
	MovingAndJmp = false;
	Is_Level_Up = false;
	Pos = { 800.0f,565.0f };//초기위치
	MaxHp = 10000.0f;
	Hp = 10000.0f;
	Mp = 1000.0f;
	Xp = 0.0f;
	MaxXp = 100.0f * Lv;
	direction = 0.0;
	
	Speed = 80.0f;
	JumpPower = 300.948f;
	Radius = 70.0f;
	Str=4;
	Dex =4;
	BasicDmg = 10.0f;
	
	AttackSpeed = 0.375f*static_cast<float>(Dex);
	vector<int> Animaion_Count{ 3,3,4,1,3,0,0 };
	PlayerRenders.Init(Name,Pos, Animaion_Count);
	PlayerRenders.Set_Current_AnimationType_State(AnimationType::Stand);
	InitializeCriticalSection(&cs);
}
void PlayerObject::Set_Can_Delete_Time(double DeltaTime)
{
	
	EnterCriticalSection(&cs);
	Can_Delete += DeltaTime;
	LeaveCriticalSection(&cs);
}
PlayerObject::~PlayerObject()
{
	DeleteCriticalSection(&cs);
}



void PlayerObject::Start_Skill_Calc(ObjectClass* My, double DeltaTime)
{
	SkillList.Update_For_Calc(My, DeltaTime);
}

bool PlayerObject::Set_immortal(bool State)
{
	return immortal=State;
}

void PlayerObject::Set_Is_Lv_Up(bool _Lv)
{
	Lv = _Lv;
}

string PlayerObject::Get_Name()
{
	
	string Out_Name = Name;
	
	return Out_Name;
}

void PlayerObject::Check_State()
{
	MaxXp = 60.0f * Lv;//130

	if (CurrentLv != Lv)
	{
		if (Xp >= MaxXp)
		{
			Xp -=MaxXp;
			
			CurrentLv = Lv;
			Lv++;
			Str += 10;
			Dex += 10;
			Hp += 100.0f;
			MaxHp += 100.0f;
			Hp = MaxHp;
			Is_Level_Up = true;
			std::string LevelUpSound= BackgroundMusicPath + std::string("LevelUp.mp3");
			G_ctx->G_SoundList.PlayEffectSound(LevelUpSound, 3.0, 0.7f);
			G_ctx->LoginCheck.Set_Level(Lv);
			
		}
		//레벨업 구현
	}

}

void PlayerObject::Update()
{
	Check_State();
	if (Is_Level_Up)
	{
	PlayerRenders.Draw_LevelUp(Pos,&Is_Level_Up); // 계속 그리기
	}

	PlayerRenders.Draw_Lv(Lv);
	PlayerRenders.Draw_Hp_Bar(MaxHp, Hp);
	PlayerRenders.Draw_Mp_Bar(1000.0f, 1000.0f);//미구현
	PlayerRenders.Draw_Xp_Bar(MaxXp, Xp);
	PlayerRenders.Update(Pos, direction);
	PlayerRenders.Draw_Name(Pos, PlayerName);
	SkillList.Update_For_Renderring(Pos);

}

bool PlayerObject::Get_Is_Lv_Up()
{
	return false;
}



int PlayerObject::Get_Lv()
{
	return Lv;
}

float PlayerObject::Get_Hp()
{

	float Out_Hp = Hp;
	
	return Out_Hp;
}

float PlayerObject::Get_Mp()
{
	
	float Out_Mp = Mp;

	return Out_Mp;
}

float PlayerObject::Get_Xp()
{
	return Xp;
}

Vec2 PlayerObject::Get_Pos()
{
	return Pos;
}

bool PlayerObject::Is_Moving()
{
	return Moving;
}

bool PlayerObject::Is_Jump()
{
	return Jump;
}

bool PlayerObject::Is_MovingAndJmp()
{
	return MovingAndJmp;
}

float PlayerObject::Get_BasicDmg()
{
	return BasicDmg;
}



float PlayerObject::Get_See_Radius()
{
	return 0.0f;
}

float PlayerObject::Get_Radius()
{
	return Radius;
}

int PlayerObject::Get_Str()
{
	return Str;
}

int PlayerObject::Get_Dex()
{
	return Dex;
}



float PlayerObject::Get_Speed()
{
	return Speed;
}

float PlayerObject::Get_JumpPower()
{
	return JumpPower;
}

float PlayerObject::Get_AttackSpeed()
{
	return AttackSpeed;
}

double PlayerObject::Get_Can_Delete_Time()
{
	return Can_Delete;
}

double PlayerObject::Get_direction()
{
	return direction;
}

bool PlayerObject::Get_immortal()
{
	return immortal;
}

AnimationType PlayerObject::Get_Animaion_State()
{
	return PlayerRenders.Get_Current_AnimationType_State();
}

Vec2 PlayerObject::Get_Size()
{
	return PlayerRenders.Get_Size();
}



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
	Xp+= _Xp;
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
	AnimationType CurrentState =PlayerRenders.Get_Current_AnimationType_State();
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




