#include "Q.h"
#include "../../../../Engine/Input/Input.h"
Q::Q()
{
	SkillName ="Q_";

	QSkillAnimation1_Path = SkillMusicPath + SkillName + "1.mp3";
	QSkillHitAnimation_Path = SkillMusicPath + SkillName + "Hit.mp3";
	IsLastFrame1 = true;
	IsLastFrame2 = true;
	IsLastFrame3 = true;
	CoolTime=0.0;

	Q_Skill_1.InitAnimations_Skill(TMP1, SkillPath + SkillName + "1\\", QSkillAnimation1, 43);
	Q_Hit_Skill.InitAnimations_Skill(TMP1, SkillPath + SkillName + "Hit\\", QSkillHitAnimation,7);



}
Q::~Q()
{
	for (const auto& [key, value] : QSkillAnimation1) {
		value->Release();
	}
	QSkillAnimation1.clear();
	for (const auto& [key, value] : QSkillHitAnimation) {
		value->Release();
	}
	QSkillHitAnimation.clear();
}

std::string Q::Get_SkillAnimation1_Path() const
{
	return QSkillAnimation1_Path;
}

std::string Q::Get_SkillHitAnimation_Path() const
{
	return QSkillHitAnimation_Path;
}



double Q::Get_MaxCoolTime() const
{
	return MaxCoolTime;
}

double Q::Get_CoolTime() const
{
	return CoolTime;
}

float Q::Get_SkillRadius() const
{
	return SkillRadius;
}

float Q::Get_SkillDmg() const
{
	return SkillDmg;
}

Vec2 Q::Get_SkillPos() const
{
	return SkillPos;
}

Vec2 Q::Get_SkillSize() const
{
	return SkillSize;
}

Vec2 Q::Get_HitSkillPos() const
{
	return HitSkillPos;
}

Vec2 Q::Get_HitSkillSize() const
{
	return HitSkillSize;
}

SkillState Q::Get_SkillState() const
{
	return CurrentState;
}

bool* Q::Get_IsLastFrame1()
{
	return &IsLastFrame1;
}

bool* Q::Get_IsLastFrame2()
{
	return &IsLastFrame2;
}

bool* Q::Get_IsLastFrame3()
{
	return &IsLastFrame3;
}

std::map<int, ID3D11ShaderResourceView*>& Q::Get_SkillAnimation1()
{
	return QSkillAnimation1;
}

std::map<int, ID3D11ShaderResourceView*>& Q::Get_SkillHitAnimation()
{
	return QSkillHitAnimation;
}

Animation& Q::Get_Skill_1()
{
	return Q_Skill_1;
}

Animation& Q::Get_Hit_Skill()
{
	return Q_Hit_Skill;
}

int Q::Get_Key() const
{
	return key;
}

bool Q::Who_Am_I(std::string Who)
{
	return !SkillName.compare(Who);
}

void Q::Minus_CoolTime(double _Time)
{
	CoolTime -= _Time;
}

void Q::Set_CoolTime(double _Time)
{
	CoolTime = _Time;
}

void Q::Set_SkillState(SkillState _ChangeState)
{
	CurrentState = _ChangeState;
}

void Q::Set_IsLastFrame1(bool _Set)
{
	IsLastFrame1 = _Set;
}

void Q::Set_IsLastFrame2(bool _Set)
{
	IsLastFrame2 = _Set;
}

void Q::Set_IsLastFrame3(bool _Set)
{
	IsLastFrame3 = _Set;
}

void Q::Set_Key(int _key)
{
	key = _key;
}

