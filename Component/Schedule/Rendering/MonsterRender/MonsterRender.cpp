#include "MonsterRender.h"
#include"../Color/Color.h"

MonsterRender::~MonsterRender()
{
	for (int i = 0;i < 2;i++)
	{
		Resource[i]->Release();
	}

		
}

MonsterRender::MonsterRender()
{

	
}

void MonsterRender::Init(string Name, Vec2& Pos, vector<int> AnimationCout)
{
	string Path = Monster + Name+"\\";

	Anim.InitAnimations_Move(Size, Path, Animations, AnimationCout);
	Size.x = 25.0f;
	Size.y = 30.0f;
	ResourcePath[0] = BottomUI + std::string("BarBgr.png");
	ResourcePath[1] = BottomUI + std::string("Hp.png");
	DmgTime = 0.0;
	for (int i = 0;i < 2;i++)
	{
		G_ctx->MakeRender->LoadTextureFromFileDX11(ResourcePath[i].c_str(), &Resource[i], TMP);//이순서
	}

	

}

void MonsterRender::Update(Vec2& Pos, double Direction)
{
	
	float DistanceOrSpeed = 0.0f;
	if (Pos.distance(BeforeVec) > 20.0f)
	{
		DistanceOrSpeed = Pos.distance(BeforeVec);
	}
	AnimationType Temp_State = Current_AnimationType_State;
	ID3D11ShaderResourceView* FrameRect = Anim.Start_Animation_Update(Animations, Temp_State, DistanceOrSpeed);
	if (!FrameRect)return;//예외처리

	Current_AnimationType_State = Temp_State;

	G_ctx->MakeRender->Draw_Image(FrameRect, Pos, Size, Direction);

	

	BeforeVec = Vec2(Pos);
}

void MonsterRender::Draw_Hp_Bar(Vec2& Pos, float MaxState, float State)
{
	Vec2 BG_HPgroundBarRoot = Pos + BackGroundPos::MainBar::Hp::Monster_HpBarPosBackGroundOffset;
	Vec2 BG_HpBarPosSize = BackGroundPos::MainBar::Hp::Monster_HpBarBackGroundSize;


	Vec2 BG_barSize = { BG_HpBarPosSize.x , BG_HpBarPosSize.y };


	Vec2 HPgroundBarRoot =  Pos+BackGroundPos::MainBar::Hp::Monster_HpBarPosOffset;
	Vec2 HpBarPosSize = BackGroundPos::MainBar::Hp::Monster_HpBarSize;
	float fractal = State / MaxState;
	Vec2 barSize = { HpBarPosSize.x * fractal, HpBarPosSize.y };

	G_ctx->MakeRender->Draw_Back_Image(Resource[0], BG_HPgroundBarRoot, BG_barSize, 0.0);
	G_ctx->MakeRender->Draw_Back_Image(Resource[1], HPgroundBarRoot, barSize, 0.0);
}



AnimationType MonsterRender::Get_Current_AnimationType_State()
{
	 return Current_AnimationType_State;
}

void MonsterRender::Set_Current_AnimationType_State(AnimationType ChangeState)
{
	Current_AnimationType_State = ChangeState;
}

Vec2 MonsterRender::Get_Size()
{
	return Size;
}
