#include "E.h"
#include "../../../../Engine/Input/Input.h"
E::E()
{
	SkillName ="E_";
	ESkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
	ESkillHitAnimation_Path = SkillMusicPath + std::string("Hit.mp3");

	IsLastFrame1 = true;
	IsLastFrame2 = true;

	CoolTime=0.0;

	E_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", ESkillAnimation1, 14);
	E_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", ESkillHitAnimation,8);



}
E::~E()
{
	for (const auto& [key, value] : ESkillAnimation1) {
		value->Release();
	}
	ESkillAnimation1.clear();
	for (const auto& [key, value] : ESkillHitAnimation) {
		value->Release();
	}
	ESkillHitAnimation.clear();
}

std::string E::Get_SkillAnimation1_Path() const
{
	return ESkillAnimation1_Path;
}

std::string E::Get_SkillHitAnimation_Path() const
{
	return ESkillHitAnimation_Path;
}


double E::Get_MaxCoolTime() const
{
	return MaxCoolTime;
}

double E::Get_CoolTime() const
{
	return CoolTime;
}

float E::Get_SkillRadius() const
{
	return SkillRadius;
}

float E::Get_SkillDmg() const
{
	return SkillDmg;
}

Vec2 E::Get_SkillPos() const
{
	return SkillPos;
}

Vec2 E::Get_SkillSize() const
{
	return SkillSize;
}

Vec2 E::Get_HitSkillPos() const
{
	return HitSkillPos;
}

Vec2 E::Get_HitSkillSize() const
{
	return HitSkillSize;
}

SkillState E::Get_SkillState() const
{
	return CurrentState;
}

bool* E::Get_IsLastFrame1()
{
	return &IsLastFrame1;
}

bool* E::Get_IsLastFrame2()
{
	return &IsLastFrame2;
}

bool* E::Get_IsLastFrame3()
{
	return &IsLastFrame3;
}

std::map<int, ID3D11ShaderResourceView*>& E::Get_SkillAnimation1() 
{
	return ESkillAnimation1;
}

std::map<int, ID3D11ShaderResourceView*>& E::Get_SkillHitAnimation() 
{
	return ESkillHitAnimation;
}

Animation& E::Get_Skill_1()
{
	return E_Skill_1;
}

Animation& E::Get_Hit_Skill()
{
	return E_Hit_Skill;
}

int E::Get_Key() const
{
	return key;
}

bool E::Who_Am_I(std::string Who)
{
	return !SkillName.compare(Who);
}

void E::Minus_CoolTime(double _Time)
{
	CoolTime -= _Time;
}

void E::Set_CoolTime(double _Time)
{
	CoolTime = _Time;
}

void E::Set_SkillState(SkillState _ChangeState)
{
	CurrentState = _ChangeState;
}

void E::Set_IsLastFrame1(bool _Set)
{
	IsLastFrame1 = _Set;
}

void E::Set_IsLastFrame2(bool _Set)
{
	IsLastFrame2 = _Set;
}

void E::Set_IsLastFrame3(bool _Set)
{
	IsLastFrame3 = _Set;
}

void E::Set_Key(int _key)
{
	key = _key;
}

