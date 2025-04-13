#pragma once
#include "../SkillObject.h"
#include "../../../../Engine/AnimationType.h"
#include "../../../../Engine/Animation/Animation.h"

class Q : public SkillObject
{
private:
	std::string SkillName;
	int key;
	const double MaxCoolTime=10.0;
	double CoolTime;
	const float SkillRadius=500.0f;
	const float SkillDmg = 20.0f;
	const Vec2 SkillPos = { 0,140 };
	const Vec2 SkillSize = { 500.0f, 200.0f };
	const Vec2 HitSkillPos = { 0,60 };
	const Vec2 HitSkillSize = { 100.0f, 100.0f };

	SkillState CurrentState = SkillState::Idle;

	
	bool IsLastFrame1;
	bool IsLastFrame2;
	bool IsLastFrame3;
private:

	std::map<int, ID3D11ShaderResourceView* > QSkillAnimation1;

	std::map<int, ID3D11ShaderResourceView* > QSkillHitAnimation;
	Animation Q_Skill_1;
	Animation Q_Hit_Skill;
	Vec2 TMP1;

private:

	std::string QSkillAnimation1_Path;
	std::string QSkillHitAnimation_Path;

public:
	Q();
	~Q();
	std::string Get_SkillAnimation1_Path()const override;
	std::string Get_SkillHitAnimation_Path()const override;
	double Get_MaxCoolTime()const override;
	double Get_CoolTime()const override;
	float Get_SkillRadius()const override;
	float Get_SkillDmg()const override;

	Vec2 Get_SkillPos()const override;
	Vec2 Get_SkillSize()const override;
	Vec2 Get_HitSkillPos()const override;
	Vec2 Get_HitSkillSize()const override;

	SkillState Get_SkillState()const override;

	bool* Get_IsLastFrame1() override;
	bool* Get_IsLastFrame2()override;
	bool* Get_IsLastFrame3()override;

	std::map<int, ID3D11ShaderResourceView* >& Get_SkillAnimation1()  override;
	std::map<int, ID3D11ShaderResourceView* >& Get_SkillHitAnimation() override;

	Animation& Get_Skill_1() override;
	Animation& Get_Hit_Skill() override;

	int Get_Key()const override;
	bool Who_Am_I(std::string Who)override;
public:

	void Minus_CoolTime(double _Time)override;
	void Set_CoolTime(double _Time)override;
	void Set_SkillState(SkillState _ChangeState)override;
	void Set_IsLastFrame1(bool _Set) override;
	void Set_IsLastFrame2(bool _Set) override;
	void Set_IsLastFrame3(bool _Set) override;
public:
	void Set_Key(int _key)override;




};

