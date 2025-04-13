#pragma once
#include"../../../../../pch.h"
#include "../../../Engine/AnimationType.h"
#include"../../../Engine/Animation/Animation.h"
#include "../../../../../Global.h"
#include "../../../Engine/Collision/Collision.h"
#include"../../../Engine/DamageLib/DamageLib.h"


enum class SkillState
{
	Idle,
	Casting1,   // 첫 번째 애니메이션 실행 중
	Damaging,  // 대미지 처리
	Hitting,   // 두 번째 애니메이션 실행 중
	Casting2   // 세 번째 애니메이션 실행 중
};
class SkillObject
{
private:
	std::vector<Vec2> TargetPos;
	double CurrentTime = 0.0;
	Collision CheckCollision;
	DamageLib DmgLib;
	Animation* Skill_1;
	Animation* Hit_Skill;

public:
	virtual ~SkillObject() = default;
	void Update_For_Renderring(Vec2 Pos);
	void Update_For_Calc(ObjectClass* My,double DeltaTime);

public:
	virtual std::string Get_SkillAnimation1_Path()const = 0;
	virtual std::string Get_SkillHitAnimation_Path()const = 0;
	virtual double Get_MaxCoolTime()const = 0;
	virtual double Get_CoolTime()const =0;
	virtual float Get_SkillRadius()const =0;
	virtual float Get_SkillDmg()const =0;

	virtual Vec2 Get_SkillPos()const =0;
	virtual Vec2 Get_SkillSize()const =0;
	virtual Vec2 Get_HitSkillPos()const =0;
	virtual Vec2 Get_HitSkillSize()const =0;

	virtual SkillState Get_SkillState()const =0;

	virtual bool* Get_IsLastFrame1() =0;
	virtual bool* Get_IsLastFrame2() =0;
	virtual bool* Get_IsLastFrame3() =0;

	virtual std::map<int, ID3D11ShaderResourceView* >& Get_SkillAnimation1() =0;
	virtual std::map<int, ID3D11ShaderResourceView* >& Get_SkillHitAnimation() =0;

	virtual Animation& Get_Skill_1() =0;
	virtual Animation& Get_Hit_Skill() =0;
	virtual bool Who_Am_I(std::string Who) = 0;
public:
	virtual int Get_Key()const = 0;

public:

	virtual void Minus_CoolTime(double _Time) = 0;
	virtual void Set_CoolTime(double _Time)=0;
	virtual void Set_SkillState(SkillState _ChangeState)=0;
	virtual void Set_IsLastFrame1(bool _Set) =0;
	virtual void Set_IsLastFrame2(bool _Set) =0;
	virtual void Set_IsLastFrame3(bool _Set) =0;

public:
	virtual void Set_Key(int _key) = 0;
};

