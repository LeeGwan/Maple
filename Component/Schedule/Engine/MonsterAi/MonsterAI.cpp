#include "MonsterAI.h"
#include "../../../../Global.h"
#include <algorithm>
MonsterAI::MonsterAI()
{
	
}

void MonsterAI::Monster_Alet(ObjectClass* My, double DeltaTime)
{
	
	ObjectClass* Target = G_ctx->G_ObjectManager->Get_Find_obj("Player");


	Vec2 TargetPos = Target->Get_Pos();
	Vec2 MyPos = My->Get_Pos();
	float Distance = My->Get_Pos().distance(TargetPos);
	Vec2 Get_normalized = (TargetPos - MyPos).normalized();
	float Check_Normal =  Get_normalized.x;
	double Direction=0.0;
	if (Check_Normal > 0)
	{
		Direction = -1.0;

	}
	else if (Check_Normal < 0)
	{
		Direction = 1.0;

	}
	My->Set_direction(Direction);
	float Normalized = (100.f * DeltaTime) * Direction;//´ÜÀ§ º¤ÅÍ *¹æÇâº¤ÅÍ 
	Vec2 Path(MyPos.x + Normalized, MyPos.y);
	ImVec2 Map_Size = G_ctx->MakeWindows->Get_MapSize();
	Path.x = clamp(Path.x, 50.0f, Map_Size.x);
	AletTimer += DeltaTime;


	if (AletTimer > 0.5f)
	{
		AletTimer -= 0.5f;
		My->Set_State(AnimationType::Stand);
	}
	else
	{
		My->Set_Pos(Path);
		
	}
	
}

void MonsterAI::Player_Alet(ObjectClass* My, double DeltaTime)
{


	Dmg_Result Result = DmgLib.Damage_Update(My);
	if (!Result.T_obj)return;

	if (Result.T_obj->Get_Animaion_State() != AnimationType::Alert && !Result.T_obj->Get_immortal())
	{
		Animaion_Dmg Set_Dmg_Infor = { Result.Dmg, DeltaTime,Result.T_obj->Get_Pos() };
		Set_Dmg_Infor.IsPlayer = true;
		G_ctx->DmgObject.Set_NowDmg(Set_Dmg_Infor);
	}
	Result.T_obj->Set_immortal(true);
}

void MonsterAI::MonsterAI_Start(ObjectClass* My, double DeltaTime)
{
	float Radius = My->Get_Radius();
	float Can_See = My->Get_See_Radius();
	ObjectClass* Target = Collisions.Check_Collision(My, Can_See);
	Accumulator += DeltaTime;
	if (Accumulator >= 5.0f || init == 0)
	{
		Random_Pos = Random_Act(My);

		if (init != 0)
		{
			Accumulator -= 5.0f;
		}
		else
		{
			init = 1;
		}
	}
	if (My->Get_Animaion_State() == AnimationType::Alert)
	{
		Monster_Alet(My, DeltaTime);
		return;
	}
	
	
	
	
		double Direction = 0.0;
		Vec2 MyPos = My->Get_Pos();
		Vec2 TargetPos = !Target ? Random_Pos : Target->Get_Pos();
		AnimationType AnyType = !Target ? Random_State : AnimationType::Move;
		My->Set_State(AnyType);
		if (AnyType == AnimationType::Stand)
		{
			return;
		}
		
		//LeftOrRight
		float Distance = My->Get_Pos().distance(TargetPos);
		Vec2 Get_normalized =(TargetPos - MyPos).normalized();
		float Check_Normal = !Target ? LeftOrRight : Get_normalized.x;
		if (Check_Normal > 0)
		{
			Direction = 1.0;
			
		}
		else if (Check_Normal < 0)
		{
			Direction = -1.0;
			
		}
		My->Set_direction(Direction);
		
		float Normalized = (My->Get_Speed() * DeltaTime) * Direction;//´ÜÀ§ º¤ÅÍ *¹æÇâº¤ÅÍ 
		Vec2 Path(MyPos.x + Normalized, MyPos.y);
		ImVec2 Map_Size = G_ctx->MakeWindows->Get_MapSize();
		Path.x = clamp(Path.x, 50.0f, Map_Size.x);
	
			My->Set_Pos(Path);
	
	
		if (My->Get_Animaion_State() != AnimationType::Move)
		{

			My->Set_State(AnimationType::Move);
		}
		ObjectClass* Target_Check = Collisions.Check_Collision(My, Radius);
		if (!Target_Check)return;
		else 
		{
			Player_Alet(My, DeltaTime);
		}
	
	

}

Vec2 MonsterAI::Random_Act(ObjectClass* My)
{
	Vec2 Move_Pose;
	/*
	if (My->Get_Name().find("GreenMushroom") != std::string::npos)
	{
		RandonState = rand() % 2;//0 Stand 1,Move 
	}
	else
	{
		RandonState = rand() % 3;//0 Stand 1,Move 2,jump
	}
	*/
	RandonState = rand() % 2;//0 Stand 1,Move 
	Vec2 My_Pos = My->Get_Pos();
	if (RandonState == 0)
	{
		Random_State = AnimationType::Stand;
		return {-999.f, -999.f};
		
	}
	else if (RandonState == 1)
	{
		
		Random_State = AnimationType::Move;

		Move_Pose = Random_Act_Calc(My_Pos, AnimationType::Move);
	
	}
	else if (RandonState == 2)
	{
		Random_State = AnimationType::Jump;
		Move_Pose = Random_Act_Calc(My_Pos, AnimationType::Jump);
	
	}
	return Move_Pose;
}

Vec2 MonsterAI::Random_Act_Calc(Vec2 Pos, AnimationType State)
{
	Vec2 OutPutPos(Pos.x, Pos.y);
	int What_LeftOrRight = rand() % 2;
	int min = 10;
	int max = 60;
	LeftOrRight = What_LeftOrRight == 0 ? -1.0f : 1.0f;
	int Random_X = rand() % (max - min + 1) + min;
	OutPutPos.x +=(Random_X * LeftOrRight);
	return OutPutPos;
}
