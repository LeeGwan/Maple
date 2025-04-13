#include "LoadingMap.h"
#include "../../../../Global.h"
LoadingMap::LoadingMap()
{
	
}

LoadingMap::~LoadingMap()
{
	for (const auto& [key, value] : InitAnimation1) {
		value->Release();
	}
	InitAnimation1.clear();

}

void LoadingMap::init()
{
	InitAnimation_Path;

	InitAnimation_Path = LoadingMapPath;







	InitAnim.InitAnimations_Skill(Temp, InitAnimation_Path, InitAnimation1, 63);
}

void LoadingMap::Update(bool* CheckFinish)
{
	ID3D11ShaderResourceView* FrameRect = InitAnim.Start_Animation_Update_For_Skiil(InitAnimation1, CheckFinish);
	if (!FrameRect)return;
	ImVec2 POS1 = G_ctx->MakeWindows->Get_WindowSize();
	G_ctx->MakeRender->Draw_Back_Image(FrameRect, Vec2(0.0f, 0.0f), Vec2(POS1.x, POS1.y), 0.0);
	
}
