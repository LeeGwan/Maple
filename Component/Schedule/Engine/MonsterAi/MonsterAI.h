#pragma once
#include "../../ObjectList/ObjectList.h"
#include "../Collision/Collision.h"
#include "../DamageLib/DamageLib.h"
#include <random>
#include <thread>
class MonsterAI
{
private:
	
	double FixedTime =0.0; // «¡∑π¿”
	double Accumulator = 0.0;
	
	Collision Collisions;
	DamageLib DmgLib;
	int RandonState;
	float LeftOrRight;
	int Random_x;

	Vec2 Random_Pos;
	AnimationType Random_State;
	int init = 0;

	double AletTimer = 0.0;
	Vec2 Before_Path;

public:
	MonsterAI();
	void Monster_Alet(ObjectClass* My, double DeltaTime);
	void Player_Alet(ObjectClass* My, double DeltaTime);
	void MonsterAI_Start(ObjectClass* My, double DeltaTime);
	Vec2 Random_Act(ObjectClass* My);
	Vec2 Random_Act_Calc(Vec2 Pos, AnimationType State);
};

