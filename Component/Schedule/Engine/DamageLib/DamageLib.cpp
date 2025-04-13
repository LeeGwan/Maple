#include "DamageLib.h"
#include "../AnimationType.h"

DamageLib::DamageLib()
{
	AttackTime = 0.0;
}
Dmg_Result& DamageLib::Damage_Update(ObjectClass* MyObj)//소리나 이벤트처리할떄
{
	Dmg_Result ReturnValue = {0.0f,nullptr};

	int Plus_Dmg = rand() % 15;
	bool Check_Obj = MyObj->Get_Name().find("Player") != std::string::npos;
	float Basic_Dmg = MyObj->Get_BasicDmg();
	float Total_Dmg = Check_Obj ?((MyObj->Get_BasicDmg() / 2) * MyObj->Get_Str())+ Plus_Dmg : MyObj->Get_BasicDmg()+ Plus_Dmg;
	ObjectClass* T_Obj=Collisions.Check_Collision(MyObj, MyObj->Get_Radius());
	if (!T_Obj)
	{
		return ReturnValue;
	}
	ReturnValue = { Total_Dmg ,T_Obj };
	auto a = T_Obj->Get_Animaion_State();
	if (T_Obj->Get_Animaion_State() != AnimationType::Alert&& !T_Obj->Get_immortal())
	{
		
		Calc_Dmg(Total_Dmg, T_Obj);
	
		
	}
	


	return ReturnValue;
}

std::vector<Dmg_Result> DamageLib::Damage_Update(std::vector<ObjectClass*>& TargetObj, float SkillDmg)
{
	std::vector<Dmg_Result> ReturnValue;
	float SetMonsterHP = 0.0;
	float ToTalDmg = 0.0;

	for (auto& it : TargetObj)
	{
		float MonsterHP = it->Get_Hp();
		if (MonsterHP == 0.0)continue;

		ToTalDmg= rand() % 55 + SkillDmg;
		Calc_Dmg(ToTalDmg, it);
		ReturnValue.push_back({ ToTalDmg ,it });
	}
	return ReturnValue;
}

void DamageLib::Calc_Dmg(float Calc_Dmg, ObjectClass* T_Obj)
{

	float T_Hp=T_Obj->Get_Hp();
	float Calc_T_Hp = 0.0f;
	if (T_Hp <= Calc_Dmg)
	{
		T_Obj->Set_Hp(0.0f);
		
		
	}
	else
	{
		Calc_T_Hp = T_Hp - Calc_Dmg;
		T_Obj->Set_Hp(Calc_T_Hp);
	
	}


}