#include "R.h"
#include "../../../../Engine/Input/Input.h"
R::R()
{
	SkillName ="R_";
	RSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
	RSkillHitAnimation_Path = SkillMusicPath + std::string("Hit.mp3");

	IsLastFrame1 = true;
	IsLastFrame2 = true;
	IsLastFrame3 = true;
	CoolTime=0.0;

	R_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", RSkillAnimation1, 10);
	R_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", RSkillHitAnimation,8);



}

R::~R()
{
	for (const auto& [key, value] : RSkillAnimation1) {
		value->Release();
	}
	RSkillAnimation1.clear();
	for (const auto& [key, value] : RSkillHitAnimation) {
		value->Release();
	}
	RSkillHitAnimation.clear();
}

std::string R::Get_SkillAnimation1_Path() const
{
	return RSkillAnimation1_Path;
}

std::string R::Get_SkillHitAnimation_Path() const
{
	return RSkillHitAnimation_Path;
}



double R::Get_MaxCoolTime() const
{
	return MaxCoolTime;
}

double R::Get_CoolTime() const
{
	return CoolTime;
}

float R::Get_SkillRadius() const
{
	return SkillRadius;
}

float R::Get_SkillDmg() const
{
	return SkillDmg;
}

Vec2 R::Get_SkillPos() const
{
	return SkillPos;
}

Vec2 R::Get_SkillSize() const
{
	return SkillSize;
}

Vec2 R::Get_HitSkillPos() const
{
	return HitSkillPos;
}

Vec2 R::Get_HitSkillSize() const
{
	return HitSkillSize;
}

SkillState R::Get_SkillState() const
{
	return CurrentState;
}

bool* R::Get_IsLastFrame1()
{
	return &IsLastFrame1;
}

bool* R::Get_IsLastFrame2()
{
	return &IsLastFrame2;
}

bool* R::Get_IsLastFrame3()
{
	return &IsLastFrame3;
}

std::map<int, ID3D11ShaderResourceView*>& R::Get_SkillAnimation1() 
{
	return RSkillAnimation1;
}

std::map<int, ID3D11ShaderResourceView*>& R::Get_SkillHitAnimation() 
{
	return RSkillHitAnimation;
}

Animation& R::Get_Skill_1()
{
	return R_Skill_1;
}

Animation& R::Get_Hit_Skill()
{
	return R_Hit_Skill;
}

int R::Get_Key() const
{
	return key;
}

bool R::Who_Am_I(std::string Who)
{
	return !SkillName.compare(Who);
}

void R::Minus_CoolTime(double _Time)
{
	CoolTime -= _Time;
}

void R::Set_CoolTime(double _Time)
{
	CoolTime = _Time;
}

void R::Set_SkillState(SkillState _ChangeState)
{
	CurrentState = _ChangeState;
}

void R::Set_IsLastFrame1(bool _Set)
{
	IsLastFrame1 = _Set;
}

void R::Set_IsLastFrame2(bool _Set)
{
	IsLastFrame2 = _Set;
}

void R::Set_IsLastFrame3(bool _Set)
{
	IsLastFrame3 = _Set;
}

void R::Set_Key(int _key)
{
	key = _key;
}

