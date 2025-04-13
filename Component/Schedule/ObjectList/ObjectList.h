#pragma once
#include "../../../pch.h"
#include "../Engine/AnimationType.h"



class ObjectClass
{

public:
	
	virtual void Update() = 0;

public:
	virtual ObjectClass* GetObj();
	virtual int Get_Lv();
	virtual bool Get_Is_Lv_Up();
	virtual ~ObjectClass() = default;
	virtual string Get_Name();
	virtual float Get_Hp();
	virtual float Get_Mp();
	virtual float Get_Xp();
	virtual Vec2 Get_Pos();
	virtual bool Is_Moving();
	virtual bool Is_Jump();
	virtual bool Is_MovingAndJmp() ;
	virtual float Get_Speed() ;
	virtual float Get_JumpPower() ;
	virtual float Get_AttackSpeed() ;
	virtual float Get_BasicDmg() ;
	virtual float Get_Radius() ;
	virtual int Get_Str() ;
	virtual int Get_Dex() ;
	virtual double Get_Can_Delete_Time() ;
	virtual float Get_See_Radius() ;
	virtual bool Get_immortal();
	virtual AnimationType Get_Animaion_State() ;
	virtual Vec2 Get_Size() ;
	virtual double Get_direction() ;

public:
	virtual void Set_Hp(float _Hp) ;
	virtual void Set_Mp(float _Hp) ;
	virtual void Set_Xp(float _Hp) ;
	virtual void Set_Pos(Vec2& Vec) ;
	virtual void Set_State(AnimationType state) ;

	virtual void Set_Speed(float _Speed) ;
	virtual void Set_JumpPower(float _JumpPower) ;
	virtual void Set_direction(double _direction) ;
	virtual void Set_AttackSpeed(float _AttackSpeed) ;
	virtual void Change_Jump(bool st) ;
	virtual void Change_MovingAndJmp(bool st) ;
	virtual void Set_Str(int _Str) ;
	virtual void Set_Dex(int _Dex) ;
	virtual bool Set_immortal(bool State);
	virtual void Set_Lv(int _Lv);
	virtual void Set_Is_Lv_Up(bool _Lv);
	virtual void Set_Can_Delete_Time(double DeltaTime) ;

public:
	virtual void Start_Monster_Action(ObjectClass* My, double DeltaTime);
	virtual void Start_Skill_Calc(ObjectClass* My, double DeltaTime);


};

class RoutineObject :public ObjectClass
{
private:
	
	unique_ptr<ObjectClass> ObjectClasses;
public:
	virtual ~RoutineObject() override = default;
	RoutineObject(const string& _Name);
	ObjectClass* GetObj()override;
	void Update()override ;

	
};