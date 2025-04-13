#pragma once
#include "../../../../../pch.h"
#include"Q/Q.h"
#include "W/W.h"
#include "E/E.h"
#include "R/R.h"
#include "T/T.h"
#include "SkillObject.h"
class SkillManager
{
private:
	vector<SkillObject*> SkillManagers;
	CRITICAL_SECTION cs;
private:
	Q Skill_Q;
	W Skill_W;
	E Skill_E;
	R Skill_R;
	T Skill_T;
public:
	SkillManager();
	~SkillManager();

	void Update_For_Renderring(Vec2 Pos);
	void Update_For_Calc(ObjectClass* My, double DeltaTime);
};

