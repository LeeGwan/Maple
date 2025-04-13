#include "BackGroundRender.h"

// ------------------------------------------------------------
// 소멸자: 배경 리소스 해제
// ------------------------------------------------------------
BackGroundRender::~BackGroundRender()
{
    if (BackGroundResource)
        BackGroundResource->Release();
    // MainBarResource, ExpBarResource는 누가 소유? 따로 해제할 필요 있나 확인 필요
}

// ------------------------------------------------------------
// 생성자: 배경, 메인 UI 바, 경험치 바 텍스처 로드
// ------------------------------------------------------------
BackGroundRender::BackGroundRender()
{
    // 리소스 경로 설정
    Path         = "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\Map\\Map_1.jpg";
    MainBarName  = "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\BottomUI\\mainBar.png";
    ExpBarName   = "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\BottomUI\\ExpBarBack.png";

    BackGroundResource = nullptr;

    // 현재 화면 해상도 기준으로 배경 사이즈 설정
    Size.x = ImGui::GetIO().DisplaySize.x;
    Size.y = ImGui::GetIO().DisplaySize.y;

    // 텍스처 로딩
    G_ctx->MakeRender->LoadTextureFromFileDX11(Path.c_str(), &BackGroundResource, Size);
    G_ctx->MakeRender->LoadTextureFromFileDX11(MainBarName.c_str(), &MainBarResource, MainBar_2D);
    G_ctx->MakeRender->LoadTextureFromFileDX11(ExpBarName.c_str(), &ExpBarResource, ExpBarBar_2D);
}

// ------------------------------------------------------------
// Init: 현재는 미사용 (인터페이스 유지 목적?)
// ------------------------------------------------------------
void BackGroundRender::Init(std::string Name, Vec2& Pos, std::vector<int> AnimationCout)
{
    // 현재 배경 렌더에는 사용되지 않음
}

// ------------------------------------------------------------
// Update: 배경 및 하단 UI (메인바, 경험치바) 그리기
// ------------------------------------------------------------
void BackGroundRender::Update(Vec2& Pos, double Direction)
{
    G_ctx->MakeRender->Draw_Back_Image(BackGroundResource, Vec2(0.0f, 0.0f), Size, 0.0);
    G_ctx->MakeRender->Draw_Back_Image(ExpBarResource, BackGroundPos::Exp::ExpBarPosOffset, BackGroundPos::Exp::ExpBarSize, 0.0);
    G_ctx->MakeRender->Draw_Back_Image(MainBarResource, BackGroundPos::MainBar::MainBarPosOffset, BackGroundPos::MainBar::MainBarSize, 0.0);
}

// ------------------------------------------------------------
// 배경 사이즈 반환
// ------------------------------------------------------------
Vec2 BackGroundRender::Get_Size()
{
    return Size;
}
