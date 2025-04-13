#include "MapObject.h"

#pragma once

MapObject::MapObject(const string& _Name)
{
	Name = _Name;
	G_ctx->DmgObject.Init();
	G_ctx->G_SoundList.PlayBackgroundMusic("BackGroundMusic.mp3");
	
}

string MapObject::Get_Name()
{
	return Name;
}

void MapObject::Draw_Dmg_Animation()
{
	G_ctx->DmgObject.Draw_Damage_impact();
}

void MapObject::Update()
{
	MapRender.Update(Pos,0.0);
	Draw_Dmg_Animation();
}


