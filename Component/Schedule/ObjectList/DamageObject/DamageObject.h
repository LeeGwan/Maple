#pragma once
#include"../ObjectList.h"
struct Animaion_Dmg
{
	float Dmg;
	double Deltatime;
	Vec2 Pos;
	double DmgEc = 0.0;
	bool IsPlayer=false;
	Animaion_Dmg operator=(const Animaion_Dmg& other) {
		return { this->Dmg = other.Dmg, this->Deltatime = other.Deltatime };
	}
};
class DamageObject
{
private:
	
	std::list<Animaion_Dmg> NowDmg;
	CRITICAL_SECTION cs;
private:
	string RedDmgNumPath[10];
	ID3D11ShaderResourceView* RedDmgDigitsTex[10];
	string BlueDmgNumPath[10];
	ID3D11ShaderResourceView* BlueDmgDigitsTex[10];
	Vec2 TMP;
public:
	DamageObject();
	~DamageObject();
	void Init();
	void Set_NowDmg_Calc(double DeltaTime);
	void Set_NowDmg(Animaion_Dmg& _Animaion_Dmg);
	void Check_Delete_NowDmg(double Deltatime);
	void Draw_Damage_impact();
};

