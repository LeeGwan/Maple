#include "BackGroundRender.h"

BackGroundRender::~BackGroundRender()
{
	BackGroundResource->Release();
}



BackGroundRender::BackGroundRender()
{

	Path = "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\Map\\Map_1.jpg";
	MainBarName = "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\BottomUI\\mainBar.png";
	ExpBarName = "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\BottomUI\\ExpBarBack.png";
	BackGroundResource = nullptr;
	Size.x = ImGui::GetIO().DisplaySize.x;
	Size.y = ImGui::GetIO().DisplaySize.y;


	
	G_ctx->MakeRender->LoadTextureFromFileDX11(Path.c_str(), &BackGroundResource, Size);
	G_ctx->MakeRender->LoadTextureFromFileDX11(MainBarName.c_str(), &MainBarResource, MainBar_2D);
	G_ctx->MakeRender->LoadTextureFromFileDX11(ExpBarName.c_str(), &ExpBarResource, ExpBarBar_2D);

}

void BackGroundRender::Init(string Name, Vec2& Pos, vector<int> AnimationCout)
{
}


void BackGroundRender::Update(Vec2& Pos, double Direction)
{
	
	G_ctx->MakeRender->Draw_Back_Image(BackGroundResource, Vec2(0.0f, 0.0f), Size, 0.0);
	
	G_ctx->MakeRender->Draw_Back_Image(ExpBarResource, BackGroundPos::Exp::ExpBarPosOffset, BackGroundPos::Exp::ExpBarSize, 0.0);
	G_ctx->MakeRender->Draw_Back_Image(MainBarResource, BackGroundPos::MainBar::MainBarPosOffset, BackGroundPos::MainBar::MainBarSize, 0.0);

}

Vec2 BackGroundRender::Get_Size()
{
	return Size;
}



