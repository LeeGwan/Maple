#pragma once
#include"../../ObjectList/ObjectList.h"
#include "../Collision/Collision.h"
struct Dmg_Result
{
	float Dmg;
	ObjectClass* T_obj;
	Dmg_Result operator=(const Dmg_Result& other){
		return { this->Dmg = other.Dmg, this->T_obj= other.T_obj };
	}
};
class DamageLib
{
private:
	double AttackTime;
	Collision Collisions;

public:
	DamageLib();
	Dmg_Result& Damage_Update(ObjectClass* MyObj);
	std::vector<Dmg_Result> Damage_Update(std::vector<ObjectClass*>& TargetObj, float SkillDmg);
	void Calc_Dmg(float Calc_Dmg, ObjectClass* T_Obj);

};

