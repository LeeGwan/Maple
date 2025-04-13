#pragma once
#include "../ObjectList.h"
#include"../ObjectManager.h"
#include "../../Rendering/BackGroundRender/BackGroundRender.h"
class MapObject :public ObjectClass
{
private:
	string Name;
	BackGroundRender MapRender;
	Vec2 Pos;

public:
	virtual ~MapObject() override = default;
	MapObject(const string& _Name);
	string Get_Name()override;
	void Draw_Dmg_Animation();
	void Update()override;

};

