#include "Collision.h"
#include "../../../../Global.h"
#include <algorithm>



ObjectClass* Collision::Check_Collision(ObjectClass* MyObj,float Radius)
{

	if (MyObj->Get_Name() == "Player")
	{
		ObjectClass* Find_Enemy = Find_Near_Obj_For_Player(MyObj->Get_Pos(), Radius);

		if (!Find_Enemy)return nullptr;


		return Find_Enemy;
		



	}
	else
	{
		ObjectClass* Find_Enemy = Find_Near_Obj_For_Monster(MyObj->Get_Pos(), Radius);
		if (!Find_Enemy)return nullptr;


		return Find_Enemy;
	}

	
}
std::vector<ObjectClass*> Collision::Check_Collision(Vec2 Pos, float Radius)
{
	vector<ObjectClass*> Target_obj = G_ctx->G_ObjectManager->Get_Find_Monster();
	vector<ObjectClass*> Return_Obj;
	//remove_if 는 함수 조건이 참이면 삭제된다
	Target_obj.erase(remove_if(Target_obj.begin(), Target_obj.end(), [](ObjectClass* Target_obj)
		{
			return Target_obj->Get_Hp() <= 0.0f;
		}
	), Target_obj.end());
	for (auto& Target : Target_obj)
	{
		if (Pos.distance(Target->Get_Pos()) <= Radius)
		{
			Return_Obj.push_back(Target);
		}

	}
	return Return_Obj;
}

ObjectClass* Collision::Find_Near_Obj_For_Monster(Vec2 Pos, float Radius)
{
	ObjectClass* Target_obj = G_ctx->G_ObjectManager->Get_Find_obj("Player");

	
	
	
	if (Pos.distance(Target_obj->Get_Pos()) <= Radius)
	{
		return Target_obj;
	}
	


	return nullptr;
}
ObjectClass* Collision::Find_Near_Obj_For_Player(Vec2 Pos, float Radius)
{
	vector<ObjectClass*> Target_obj = G_ctx->G_ObjectManager->Get_Find_Monster();
	//remove_if 는 함수 조건이 참이면 삭제된다
	Target_obj.erase(remove_if(Target_obj.begin(), Target_obj.end(), [](ObjectClass* Target_obj) 
		{	
		return Target_obj->Get_Hp() <= 0.0f;
		}
	),Target_obj.end());
	//remove_if 는 함수 조건이 참이면 삭제된다
	for (auto& Target : Target_obj)
	{
		if (Pos.distance(Target->Get_Pos()) <= Radius)
		{
			return Target;
		}

	}
	return nullptr;
}


