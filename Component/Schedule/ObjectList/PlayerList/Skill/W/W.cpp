#include "W.h"
#include "../../../../Engine/Input/Input.h"
W::W()
{
	SkillName ="W_";
	WSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
	WSkillHitAnimation_Path = SkillMusicPath +std::string("Hit.mp3");
	IsLastFrame1 = true;
	IsLastFrame2 = true;
	IsLastFrame3 = true;
	CoolTime=0.0;

	W_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", WSkillAnimation1, 20);
	W_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", WSkillHitAnimation,8);



}

W::~W()
{
	for (const auto& [key, value] : WSkillAnimation1) {
		value->Release();
	}
	WSkillAnimation1.clear();
	for (const auto& [key, value] : WSkillHitAnimation) {
		value->Release();
	}
	WSkillHitAnimation.clear();
}


std::string W::Get_SkillAnimation1_Path() const
{
	return WSkillAnimation1_Path;
}

std::string W::Get_SkillHitAnimation_Path() const
{
	return WSkillHitAnimation_Path;
}

double W::Get_MaxCoolTime() const
{
	return MaxCoolTime;
}

double W::Get_CoolTime() const
{
	return CoolTime;
}

float W::Get_SkillRadius() const
{
	return SkillRadius;
}

float W::Get_SkillDmg() const
{
	return SkillDmg;
}

Vec2 W::Get_SkillPos() const
{
	return SkillPos;
}

Vec2 W::Get_SkillSize() const
{
	return SkillSize;
}

Vec2 W::Get_HitSkillPos() const
{
	return HitSkillPos;
}

Vec2 W::Get_HitSkillSize() const
{
	return HitSkillSize;
}

SkillState W::Get_SkillState() const
{
	return CurrentState;
}

bool* W::Get_IsLastFrame1()
{
	return &IsLastFrame1;
}

bool* W::Get_IsLastFrame2() 
{
	return &IsLastFrame2;
}

bool* W::Get_IsLastFrame3()
{
	return &IsLastFrame3;
}

std::map<int, ID3D11ShaderResourceView*>& W::Get_SkillAnimation1() 
{
	return WSkillAnimation1;
}

std::map<int, ID3D11ShaderResourceView*>& W::Get_SkillHitAnimation() 
{
	return WSkillHitAnimation;
}

Animation& W::Get_Skill_1()
{
	return W_Skill_1;
}

Animation& W::Get_Hit_Skill()
{
	return W_Hit_Skill;
}

int W::Get_Key() const
{
	return key;
}

bool W::Who_Am_I(std::string Who)
{
	return !SkillName.compare(Who);
}

void W::Minus_CoolTime(double _Time)
{
	CoolTime -= _Time;
}

void W::Set_CoolTime(double _Time)
{
	CoolTime = _Time;
}

void W::Set_SkillState(SkillState _ChangeState)
{
	CurrentState =_ChangeState;
}

void W::Set_IsLastFrame1(bool _Set)
{
	IsLastFrame1 = _Set;
}

void W::Set_IsLastFrame2(bool _Set)
{
	IsLastFrame2= _Set;
}

void W::Set_IsLastFrame3(bool _Set)
{
	IsLastFrame3= _Set;
}

void W::Set_Key(int _key)
{
	key = _key;
}

