#pragma once
#include "../../ObjectList/ObjectList.h"
#include "../Collision/Collision.h"
#include "../DamageLib/DamageLib.h"

class Calculation
{
private:
	
	double Velocity_x;
	double Velocity_y;
	
	double Ec;
	double DmgEc;
	double AttackTime;
	Collision Collisions;
	DamageLib DmgCLass;
	
	double AlertTime=0.0;
	

public:
	~Calculation();

	

	void Calculation_Init();
	void Player_For_Calc(const string& _Name);
	void Monster_For_Calc(const string& _Name);
	void Start_Calc(const string& _Name);
	
	void Calc_For_Vector(ObjectClass* Obj, double DeltaTime,double Move_Time);
	Vec2 Calc_Event(ObjectClass* Obj, double DeltaTime, double Move_Time);
	void Calc_For_Dmg(ObjectClass* MyObj, double DeltaTime);
	void Handle_Movement(ObjectClass* Obj, AnimationType Current_State, float Speed, double Move_Time, double DeltaTime, Vec2& Calc, double& Ec, double Gravi);
	void Apply_JumpPhysics(ObjectClass* Obj, double DeltaTime, Vec2& Calc, double& Ec, double Gravi);
	void Handle_Alert(ObjectClass* Obj,double DeltaTime);

};

