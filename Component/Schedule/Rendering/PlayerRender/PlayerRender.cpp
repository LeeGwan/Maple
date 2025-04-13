#include "PlayerRender.h"
#include"../Color/Color.h"


PlayerRender::PlayerRender()
{

}

void PlayerRender::Init(string Name, Vec2& Pos, vector<int> AnimationCout)
{

	Anim.InitAnimations_Move(Size, Player, Animations, AnimationCout);
	LevelUpAnim.InitAnimations_Skill(TMP1, LevelUp, LevelUpAnimation, 20);
	Size.x = 100.0f;
	Size.y = 85.0f;
	SlashPath = BottomUI + std::string("Slash.png");
	Path[0] = BottomUI + std::string("Hp.png");
	Path[1] = BottomUI + std::string("Mp.png");
	Path[2] = BottomUI + std::string("Xp.png");
	Path[3] = BottomUI + std::string("ExpString.png");
	NamePath= BottomUI + std::string("Name.png");
	std::string Pathstr(".png");
	for (int i = 0;i < 10;i++)
	{
		
		

		LevelNumPath[i] = BottomUI +std::string("Lv_") + to_string(i) + Pathstr;
		HpNumPath[i] = BottomUI + std::string("HPNum") + to_string(i) + Pathstr;
		ExpNumPath[i] = BottomUI + std::string("ExpNum") + to_string(i) + Pathstr;
	}
	G_ctx->MakeRender->LoadTextureFromFileDX11(SlashPath.c_str(), &SlashResource, TMP);//이순서
	G_ctx->MakeRender->LoadTextureFromFileDX11(NamePath.c_str(), &NameResource, TMP);//이순서
	for (int i = 0;i < 4;i++)
	{
		G_ctx->MakeRender->LoadTextureFromFileDX11(Path[i].c_str(), &BarResource[i], TMP);//이순서
	}
	for (int i = 0;i < 10;i++)
	{
		G_ctx->MakeRender->LoadTextureFromFileDX11(LevelNumPath[i].c_str(), &levelDigitsTex[i], TMP);//이순서
		G_ctx->MakeRender->LoadTextureFromFileDX11(HpNumPath[i].c_str(), &HpDigitsTex[i], TMP);
		G_ctx->MakeRender->LoadTextureFromFileDX11(ExpNumPath[i].c_str(), &ExpDigitsTex[i], TMP);
	}

}
void PlayerRender::Draw_Name(Vec2 Pos,const wstring& Name)
{
	//플레이어 움직일떄
	std::string C_Name = G_ctx->MakeRender->W_Calc_text_Tran(Name);
	Vec2 NamePosRoot = Pos;
	Vec2 NamePos = NamePosRoot + BackGroundPos::MainBar::Name::PlayerName::NamePosOffset;
	Vec2 NamePos_Text(NamePos.x + 20.0f, NamePos.y+15.0f);
	Vec2 NameBarSize = BackGroundPos::MainBar::Name::PlayerName::NameBarSize;
	Vec2 NameSize = G_ctx->MakeRender->W_Calc_text_size(Name,0);

	//정적
	Vec2 NamePosOffset = BackGroundPos::MainBar::Name::NamePosOffset;
	
	G_ctx->MakeRender->Draw_Back_Image(NameResource, NamePos, NameBarSize, 0.0);
	G_ctx->MakeRender->Draw_Text(NamePos_Text, C_Name, Colors::Simple::lime);
	G_ctx->MakeRender->Draw_Text(NamePosOffset, C_Name, Colors::Simple::Legacy::White_font);
}
void PlayerRender::Draw_Lv(int Lv)
{
	Vec2 LvPos = BackGroundPos::MainBar::Lv::LvPosOffset;
	
	G_ctx->MakeRender->DrawLevelNumber(Lv, levelDigitsTex, LvPos,Vec2(4.0f,4.0f));

}
void PlayerRender::Draw_Xp_Bar(float MaxState, float State)
{
	Vec2 XPgroundBarRoot = BackGroundPos::Exp::ExpBarPosOffset;
	Vec2 XPgroundBarPos = Vec2(XPgroundBarRoot.x + 40.0f, XPgroundBarRoot.y +4.0f);
	Vec2 XpBarPosSize = BackGroundPos::Exp::ExpBarSize;
	Vec2 StringRoot= Vec2(XPgroundBarRoot.x , XPgroundBarRoot.y+4.0f);
	Vec2 StringPos = BackGroundPos::Exp::ExpStringSize;
	XpBarPosSize.x -=50.0f;
	float fractal =  State/ MaxState;
	Vec2 barSize = { XpBarPosSize.x*fractal, XpBarPosSize.y-8.0f };



	G_ctx->MakeRender->Draw_Back_Image(BarResource[2], XPgroundBarPos, barSize, 0.0);
	G_ctx->MakeRender->Draw_Back_Image(BarResource[3], StringRoot, StringPos, 0.0);
	Vec2 NextPos=G_ctx->MakeRender->DrawLevelNumber(State, ExpDigitsTex, Vec2(XpBarPosSize.x / 2.0f, XPgroundBarPos.y +4.0f),Vec2(4.5f,4.5f));
	NextPos.y -= 4.0f;
	G_ctx->MakeRender->Draw_Back_Image(SlashResource, NextPos, Vec2(8.5f, 8.5f),0.0);
	NextPos.x += 20.0f;
	NextPos.y += 4.0f;
	G_ctx->MakeRender->DrawLevelNumber(MaxState, ExpDigitsTex, NextPos, Vec2(4.5f, 4.5f));

}


void PlayerRender::Draw_Hp_Bar(float MaxState, float State)
{

	Vec2 HPgroundBarRoot = BackGroundPos::MainBar::Hp::HpBarPosOffset;
	Vec2 HpBarPosSize = BackGroundPos::MainBar::Hp::HpBarSize;
	float fractal = State / MaxState;
	Vec2 barSize = { HpBarPosSize.x * fractal, HpBarPosSize.y };



	G_ctx->MakeRender->Draw_Back_Image(BarResource[0], HPgroundBarRoot, barSize, 0.0);
	G_ctx->MakeRender->DrawLevelNumber(State, HpDigitsTex, Vec2((HPgroundBarRoot.x+ HpBarPosSize.x)/1.12f, HPgroundBarRoot.y+ 5.0f), Vec2(4.5f, 4.5f));
	
}

void PlayerRender::Draw_Mp_Bar(float MaxState, float State)
{
	Vec2 MPgroundBarRoot = BackGroundPos::MainBar::Mp::MpBarPosOffset;
	Vec2 MpBarPosSize = BackGroundPos::MainBar::Mp::MpBarSize;
	float fractal = State / MaxState;
	Vec2 barSize = { MpBarPosSize.x * fractal, MpBarPosSize.y };



	G_ctx->MakeRender->Draw_Back_Image(BarResource[1], MPgroundBarRoot, barSize, 0.0);
}
void PlayerRender::Draw_LevelUp(Vec2 Pos, bool* Is_Level_Up)
{
	 ID3D11ShaderResourceView * FrameRect=LevelUpAnim.Start_Animation_Update_For_Skiil(LevelUpAnimation, Is_Level_Up);
	 Vec2 LevelUp_Pos(Pos);
	 LevelUp_Pos.x += 10.0f;
	 LevelUp_Pos.y -= 300.0f;
	G_ctx->MakeRender->Draw_Image(FrameRect, LevelUp_Pos, Vec2(300.0f,300.0f),0.0 );
	
}
void PlayerRender::Update(Vec2& Pos, double Direction)
{
	ObjectClass*  PlayerObj=G_ctx->G_ObjectManager->Get_Find_obj("Player");
	float DistanceOrSpeed=0.0f;
	if (Current_AnimationType_State == AnimationType::Attack )
	{
		DistanceOrSpeed = PlayerObj->Get_AttackSpeed();
	}
	else if (Pos.distance(BeforeVec) > 20.0f)
	{
		DistanceOrSpeed = Pos.distance(BeforeVec);
	}
	
		ID3D11ShaderResourceView* FrameRect = Anim.Start_Animation_Update(Animations, Current_AnimationType_State, DistanceOrSpeed);
		if (!FrameRect)return;//예외처리


		G_ctx->MakeRender->Draw_Image(FrameRect, Pos, Size, Direction);
		BeforeVec = Vec2(Pos);
	
}

AnimationType PlayerRender::Get_Current_AnimationType_State()
{
	return Current_AnimationType_State;
}

void PlayerRender::Set_Current_AnimationType_State(AnimationType ChangeState)
{

	Current_AnimationType_State = ChangeState;

}

Vec2 PlayerRender::Get_Size()
{
	return Size;
}











