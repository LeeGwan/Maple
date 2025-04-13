#pragma once
#include "../../ObjectList.h"
#include "../../../Engine/MonsterAi/MonsterAI.h"

#include"../../ObjectManager.h"
#include "../../../Rendering/MonsterRender/MonsterRender.h"

class OrangeMushroom :public ObjectClass
{
private:
	string Name;
	Vec2 Pos;
	float See_Radius;
	float Radius;

	double Can_Delete;
	float MaxHp;
	float Hp;
	float BasicDmg;

	float Speed;
	float JumpPower;
	float AttackSpeed;

	double M_direction = 0.0;
	bool Moving;
	bool Jump;
	bool MovingAndJmp;
	MonsterRender MonsterRenders;
	MonsterAI MonsterAct;

	CRITICAL_SECTION cs;
public:

	OrangeMushroom(const string& _Name, float Random_x);

	string Get_Name()override;
	void Update()override;
	void Start_Monster_Action(ObjectClass* My, double DeltaTime)override;

	float Get_Hp()override;

	float Get_Xp()override;
	Vec2 Get_Pos()override;
	bool Is_Moving()override;
	bool Is_Jump()override;
	bool Is_MovingAndJmp()override;
	float Get_Speed()override;
	float Get_JumpPower()override;
	float Get_AttackSpeed()override;
	float Get_BasicDmg()override;
	float Get_See_Radius()override;
	float Get_Radius()override;

	double Get_direction()override;
	double Get_Can_Delete_Time()override;
	AnimationType Get_Animaion_State()override;
	Vec2 Get_Size()override;

	void Set_Hp(float _Hp)override;
	
	
	void Set_Pos(Vec2& Vec)override;
	void Set_State(AnimationType state)override;
	void Set_Speed(float _Speed) override;
	void Set_JumpPower(float _JumpPower) override;
	void Set_AttackSpeed(float _AttackSpeed)override;
	void Set_direction(double _direction)override;
	void Change_Jump(bool st)override;
	void Change_MovingAndJmp(bool st)override;



	void Set_Can_Delete_Time(double DeltaTime)override;
	~OrangeMushroom() override;
};

