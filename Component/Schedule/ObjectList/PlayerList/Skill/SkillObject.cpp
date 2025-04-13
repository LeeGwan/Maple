#include "SkillObject.h"
#include "../../../Engine/Input/Input.h"


void SkillObject::Update_For_Renderring(Vec2 Pos)
{
	SkillState CurrentState = Get_SkillState();
	std::map<int, ID3D11ShaderResourceView*> SkillAnimation1 = Get_SkillAnimation1();
	std::map<int, ID3D11ShaderResourceView*> SkillHitAnimation = Get_SkillHitAnimation();
	Skill_1=&Get_Skill_1();
	Hit_Skill =&Get_Hit_Skill();
	bool* IsLastFrame1 = Get_IsLastFrame1();
	bool* IsLastFrame2 = Get_IsLastFrame2();
	if (CurrentState == SkillState::Casting1)
	{
		Vec2 ChangePos;
		ID3D11ShaderResourceView* FrameRect;
		if (Who_Am_I("Q_"))
		{
			
			FrameRect = Skill_1->Start_Animation_Update_For_Skiil(SkillAnimation1, IsLastFrame1, true);
		}
		else
		{
			FrameRect = Skill_1->Start_Animation_Update_For_Skiil(SkillAnimation1, IsLastFrame1);
		}
		if (Who_Am_I("T_"))
		{
			ChangePos = Get_SkillPos();
		}
		else
		{
		
			ChangePos = Pos - Get_SkillPos();
		}
		

		G_ctx->MakeRender->Draw_Image(FrameRect, ChangePos, Get_SkillSize(), 0.0);
		if (!*IsLastFrame1)
		{
			Set_SkillState(SkillState::Damaging);
		
		}

	}
	if (CurrentState == SkillState::Hitting)
	{
		ID3D11ShaderResourceView* FrameRect = Hit_Skill->Start_Animation_Update_For_Skiil(SkillHitAnimation, IsLastFrame2);
		for (const auto& it : TargetPos)
		{
			
			Vec2 ChangePos = it - Get_HitSkillPos();
			G_ctx->MakeRender->Draw_Image(FrameRect, ChangePos, Get_HitSkillSize(), 0.0);
		}
		if (!*IsLastFrame1)
		{
			TargetPos.clear();
			TargetPos.shrink_to_fit();//남은 메모리까지 정리
			CurrentState = SkillState::Idle;
		}

	}
}

void SkillObject::Update_For_Calc(ObjectClass* My, double DeltaTime)
{
	Input KeyBoard;
	double CoolTime = Get_CoolTime();
	double MaxCoolTime = Get_MaxCoolTime();
	SkillState CurrentState = Get_SkillState();
	Vec2 MyPos = My->Get_Pos();
	if (KeyBoard.Input_SKillKeyBoard(Get_Key()) && Get_CoolTime() == 0.0 && CurrentState == SkillState::Idle)
	{
		Set_SkillState(SkillState::Casting1);
		Set_CoolTime(MaxCoolTime);
		if (Who_Am_I("T_"))
		{
			G_ctx->G_SoundList.PlayEffectSound(Get_SkillAnimation1_Path(), 7.0, 0.7f);
		}
		else if (Who_Am_I("Q_"))
		{
			
		}
		else
		{
			G_ctx->G_SoundList.PlayEffectSound(Get_SkillAnimation1_Path(), 4.0, 0.7f);
		}

	}
	else if (CoolTime > 0.0)
	{
		
		Minus_CoolTime(DeltaTime);

		if (CurrentState == SkillState::Damaging)
		{
			float SkillRadius = Get_SkillRadius();
			float SkillDmg = Get_SkillDmg();
			std::vector<ObjectClass*> TargetObj = CheckCollision.Check_Collision(MyPos, SkillRadius);
			
			if (TargetObj.empty())return;

			std::vector<Dmg_Result> Result = DmgLib.Damage_Update(TargetObj, SkillDmg);
			for (const auto it : Result)
			{
				TargetPos.push_back(it.T_obj->Get_Pos());
				Animaion_Dmg Set_Dmg_Infor = { it.Dmg,DeltaTime,it.T_obj->Get_Pos() };
				G_ctx->DmgObject.Set_NowDmg(Set_Dmg_Infor);
				float hp = it.T_obj->Get_Hp();
				if (hp <= 0.0f)
					My->Set_Xp(it.T_obj->Get_Xp());
			}
			Set_SkillState(SkillState::Hitting);
			G_ctx->G_SoundList.PlayEffectSound(Get_SkillHitAnimation_Path(), 1.0, 0.7f);
		}

	}
	else if (CoolTime < 0.0)
	{

		Set_CoolTime(0.0);
		Set_SkillState(SkillState::Idle);
		Set_IsLastFrame1(true);
		Set_IsLastFrame2(true);

	}
}

