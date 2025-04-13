
#include"ObjectList.h"
#include "PlayerList/PlayerObject.h"
#include"MapList/MapObject.h"
#include "MonsterList/GreenMushroom/GreenMushroom.h"
#include "MonsterList/OrangeMushroom/OrangeMushroom.h"
#include <cstdlib>  
#include <ctime>   
RoutineObject::RoutineObject(const string& _Name)
{
	
	if (_Name.find("Player") != std::string::npos)
	{
		ObjectClasses = make_unique<PlayerObject>(_Name);
	}
	else if (_Name.find("Maps") != std::string::npos)
	{
		ObjectClasses = make_unique<MapObject>(_Name);
		
	}
	else if (_Name.find("Mushroom") != std::string::npos)
	{
		static int count = 0;
		if (count == 0)
		{
			srand(static_cast<unsigned int>(time(nullptr))); // 시드 초기화 (1번만 하면 됨)
			count++;
		}
		int min = 0;
		int max = 1270;
		int Random_X= rand() % (max - min + 1) + min;
		if (_Name.find("GreenMushroom") != std::string::npos)
		{
			ObjectClasses = make_unique<GreenMushroom>(_Name, Random_X);
		}
		else
		{
			ObjectClasses = make_unique<OrangeMushroom>(_Name, Random_X);
		}
	}

	
}

ObjectClass* RoutineObject::GetObj()
{
	if (!ObjectClasses)return nullptr;

	return ObjectClasses.get();
}

void RoutineObject::Update()
{
	ObjectClasses->Update();
}


bool ObjectClass::Get_immortal()
{
	return false;
}

AnimationType ObjectClass::Get_Animaion_State()
{
	return AnimationType();
}

Vec2 ObjectClass::Get_Size()
{
	return Vec2();
}

double ObjectClass::Get_direction()
{
	return 0.0;
}

void ObjectClass::Set_Hp(float _Hp)
{
}

void ObjectClass::Set_Mp(float _Hp)
{
}

void ObjectClass::Set_Xp(float _Hp)
{
}

void ObjectClass::Set_Pos(Vec2& Vec)
{
}

void ObjectClass::Set_State(AnimationType state)
{
}



void ObjectClass::Set_Speed(float _Speed)
{
}

void ObjectClass::Set_JumpPower(float _JumpPower)
{
}

void ObjectClass::Set_direction(double _direction)
{
}

void ObjectClass::Set_AttackSpeed(float _AttackSpeed)
{
}

void ObjectClass::Change_Jump(bool st)
{
}

void ObjectClass::Change_MovingAndJmp(bool st)
{
}

void ObjectClass::Set_Str(int _Str)
{
}

void ObjectClass::Set_Dex(int _Dex)
{
}

bool ObjectClass::Set_immortal(bool State)
{
	return false;
}

void ObjectClass::Set_Lv(int _Lv)
{
}

void ObjectClass::Set_Is_Lv_Up(bool _Lv)
{
}

void ObjectClass::Set_Can_Delete_Time(double DeltaTime)
{
}


void ObjectClass::Start_Monster_Action(ObjectClass* My, double DeltaTime)
{
}

void ObjectClass::Start_Skill_Calc(ObjectClass* My, double DeltaTime)
{
}



ObjectClass* ObjectClass::GetObj()
{
	return nullptr;
}



int ObjectClass::Get_Lv()
{
	return 0;
}

bool ObjectClass::Get_Is_Lv_Up()
{
	return false;
}

string ObjectClass::Get_Name()
{
	return string();
}

float ObjectClass::Get_Hp()
{
	return 0.0f;
}

float ObjectClass::Get_Mp()
{
	return 0.0f;
}

float ObjectClass::Get_Xp()
{
	return 0.0f;
}

Vec2 ObjectClass::Get_Pos()
{
	return Vec2();
}

bool ObjectClass::Is_Moving()
{
	return false;
}

bool ObjectClass::Is_Jump()
{
	return false;
}

bool ObjectClass::Is_MovingAndJmp()
{
	return false;
}

float ObjectClass::Get_Speed()
{
	return 0.0f;
}

float ObjectClass::Get_JumpPower()
{
	return 0.0f;
}

float ObjectClass::Get_AttackSpeed()
{
	return 0.0f;
}

float ObjectClass::Get_BasicDmg()
{
	return 0.0f;
}

float ObjectClass::Get_Radius()
{
	return 0.0f;
}

int ObjectClass::Get_Str()
{
	return 0;
}

int ObjectClass::Get_Dex()
{
	return 0;
}



double ObjectClass::Get_Can_Delete_Time()
{
	return 0.0;
}

float ObjectClass::Get_See_Radius()
{
	return 0.0f;
}
