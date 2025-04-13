#pragma once
#include "../ObjectList.h"
#include"../ObjectManager.h"
#include "../../Rendering/PlayerRender/PlayerRender.h"
#include"Skill/SkillManager.h"
class PlayerObject :public ObjectClass
{
private:
	wstring PlayerName;
	string Name;
	int CurrentLv;
	int Lv;
	Vec2 Pos;
	double Can_Delete;
	double direction;
	float MaxHp;
	float Hp;
	float Mp;
	float MaxXp;
	float Xp;
	float Speed;
	float JumpPower;


	//µ¥¹ÌÁö
	float BasicDmg;
	int Str;
	int Dex;
	float AttackSpeed;
	float Radius;

	bool Moving;
	bool Jump;
	bool MovingAndJmp;
	bool immortal;
	bool Is_Level_Up;

	PlayerRender PlayerRenders;
	SkillManager SkillList;
	CRITICAL_SECTION cs;


public:
	
	PlayerObject(const string& _Name);
	~PlayerObject() override;
public:
	string Get_Name()override;
	void Check_State();
	void Update()override;
	bool Get_Is_Lv_Up()override;
	int Get_Lv()override;
	float Get_Hp()override;
	float Get_Mp()override;
	float Get_Xp()override;
	Vec2 Get_Pos()override;
	bool Is_Moving()override;
	bool Is_Jump()override;
	bool Is_MovingAndJmp()override;
	float Get_BasicDmg()override;
	float Get_See_Radius()override;
	float Get_Radius()override;
	int Get_Str()override;
	int Get_Dex()override;
	float Get_Speed()override;
	float Get_JumpPower()override;
	float Get_AttackSpeed()override;
	double Get_Can_Delete_Time()override;
	double Get_direction()override;
	bool Get_immortal()override;
	AnimationType Get_Animaion_State()override;
	Vec2 Get_Size()override;
public:
	void Set_Hp(float _Hp)override;
	void Set_Mp(float _Mp)override;
	void Set_Xp(float _Xp)override;
	void Set_Pos(Vec2& Vec)override;
	void Set_State(AnimationType state)override;
	void Set_Speed(float _Speed) override;
	void Set_JumpPower(float _JumpPower) override;
	void Set_AttackSpeed(float _AttackSpeed)override;
	
	void Set_direction(double _direction)override;
	void Set_Str(int _Str)override;
	void Set_Dex(int _Dex)override;
	void Change_Jump(bool st)override;
	void Change_MovingAndJmp(bool st)override;
	void Set_Lv(int _Lv)override;
	void Set_Can_Delete_Time(double DeltaTime)override;
	
	 bool Set_immortal(bool State)override;
	 void Set_Is_Lv_Up(bool _Lv)override;
public:
	
	void Start_Skill_Calc(ObjectClass* My, double DeltaTime)override;
};
