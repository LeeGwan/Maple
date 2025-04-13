#include "DamageObject.h"
#include "../../../../Global.h"
DamageObject::DamageObject()
{
	InitializeCriticalSection(&cs);
	//Dmgs



}
DamageObject::~DamageObject()
{
	DeleteCriticalSection(&cs);
}
void DamageObject::Init()
{
	std::string Pathstr(".png");
	for (int i = 0;i < 10;i++)
	{
		RedDmgNumPath[i] = Dmgs + std::string("RedDamage0_0") + to_string(i) + Pathstr;
		G_ctx->MakeRender->LoadTextureFromFileDX11(RedDmgNumPath[i].c_str(), &RedDmgDigitsTex[i], TMP);
		BlueDmgNumPath[i] = Dmgs + std::string("BlueDamage0_0") + to_string(i) + Pathstr;
		G_ctx->MakeRender->LoadTextureFromFileDX11(BlueDmgNumPath[i].c_str(), &BlueDmgDigitsTex[i], TMP);
	}
}
void DamageObject::Set_NowDmg_Calc(double DeltaTime)
{
	if (NowDmg.empty())return;
	//EnterCriticalSection(&cs);

	for (auto& it : NowDmg)
	{

		double Velocity_x = 0.0;
		float F_Velocity_x = 0.0f;

		Velocity_x = 1.0 * 5.0;

		F_Velocity_x = static_cast<float>(Velocity_x * DeltaTime);
		it.Pos.x += F_Velocity_x;

		it.Pos.y -= 10.5 * DeltaTime;

		it.DmgEc += DeltaTime;
	}
//	LeaveCriticalSection(&cs);

}
void DamageObject::Set_NowDmg(Animaion_Dmg& _Animaion_Dmg)
{
	EnterCriticalSection(&cs);
	float y=_Animaion_Dmg.Pos.y;
	y -= 41.0f;
	_Animaion_Dmg.Pos.y = y;

	NowDmg.push_back(_Animaion_Dmg);
	LeaveCriticalSection(&cs);
}
void DamageObject::Check_Delete_NowDmg(double Deltatime)
{
	if (NowDmg.empty()) return;



	for (auto it = NowDmg.begin(); it != NowDmg.end(); )
	{
	

		if (it->DmgEc > 2.5)
		{
	
			it = NowDmg.erase(it); // list는 erase 후 유효한 다음 iterator 반환
		}
		else
		{
			++it;
		}
	}
}

void DamageObject::Draw_Damage_impact()
{
	if (NowDmg.empty())return;

	Vec2 Size = BackGroundPos::MainBar::Damage::DamageSize;
	for (auto& it : NowDmg)
	{
		
		if(!it.IsPlayer)
		G_ctx->MakeRender->DrawLevelNumber(std::round(it.Dmg), RedDmgDigitsTex, it.Pos, Size);

		else
		G_ctx->MakeRender->DrawLevelNumber(std::round(it.Dmg), BlueDmgDigitsTex, it.Pos, Size);
	
	}
	
}
