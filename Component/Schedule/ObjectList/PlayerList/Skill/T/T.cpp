#include "T.h"
#include "../../../../Engine/Input/Input.h"
T::T()
{
	SkillName ="T_";

	TSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
	TSkillHitAnimation_Path = SkillMusicPath + SkillName + std::string("Hit.mp3");
	IsLastFrame1 = true;
	IsLastFrame2 = true;
	IsLastFrame3 = true;
	CoolTime=0.0;

	T_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", TSkillAnimation1, 49);
	T_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", TSkillHitAnimation,8);



}

T::~T()
{
	for (const auto& [key, value] : TSkillAnimation1) {
		value->Release();
	}
	TSkillAnimation1.clear();
	for (const auto& [key, value] : TSkillHitAnimation) {
		value->Release();
	}
	TSkillHitAnimation.clear();
}

std::string T::Get_SkillAnimation1_Path() const
{
	return TSkillAnimation1_Path;
}

std::string T::Get_SkillHitAnimation_Path() const
{
	return TSkillHitAnimation_Path;
}



double T::Get_MaxCoolTime() const
{
	return MaxCoolTime;
}

double T::Get_CoolTime() const
{
	return CoolTime;
}

float T::Get_SkillRadius() const
{
	return SkillRadius;
}

float T::Get_SkillDmg() const
{
	return SkillDmg;
}

Vec2 T::Get_SkillPos() const
{
	return SkillPos;
}

Vec2 T::Get_SkillSize() const
{
	return SkillSize;
}

Vec2 T::Get_HitSkillPos() const
{
	return HitSkillPos;
}

Vec2 T::Get_HitSkillSize() const
{
	return HitSkillSize;
}

SkillState T::Get_SkillState() const
{
	return CurrentState;
}

bool* T::Get_IsLastFrame1()
{
	return &IsLastFrame1;
}

bool* T::Get_IsLastFrame2()
{
	return &IsLastFrame2;
}

bool* T::Get_IsLastFrame3()
{
	return &IsLastFrame3;
}

std::map<int, ID3D11ShaderResourceView*>& T::Get_SkillAnimation1() 
{
	return TSkillAnimation1;
}

std::map<int, ID3D11ShaderResourceView*>& T::Get_SkillHitAnimation() 
{
	return TSkillHitAnimation;
}

Animation& T::Get_Skill_1()
{
	return T_Skill_1;
}

Animation& T::Get_Hit_Skill()
{
	return T_Hit_Skill;
}

int T::Get_Key() const
{
	return key;
}

bool T::Who_Am_I(std::string Who)
{
	return !SkillName.compare(Who);
}

void T::Minus_CoolTime(double _Time)
{
	CoolTime -= _Time;
}

void T::Set_CoolTime(double _Time)
{
	CoolTime = _Time;
}

void T::Set_SkillState(SkillState _ChangeState)
{
	CurrentState = _ChangeState;
}

void T::Set_IsLastFrame1(bool _Set)
{
	IsLastFrame1 = _Set;
}

void T::Set_IsLastFrame2(bool _Set)
{
	IsLastFrame2 = _Set;
}

void T::Set_IsLastFrame3(bool _Set)
{
	IsLastFrame3 = _Set;
}

void T::Set_Key(int _key)
{
	key = _key;
}

