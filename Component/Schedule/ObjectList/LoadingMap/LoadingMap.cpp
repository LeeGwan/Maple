#include "LoadingMap.h"
#include "../../../../Global.h"


// -------------------------------------
// 생성자
// -------------------------------------
LoadingMap::LoadingMap()
{
    // 초기화는 init()에서 수행
}

// -------------------------------------
// 소멸자: 사용한 리소스 해제
// -------------------------------------
LoadingMap::~LoadingMap()
{
    for (const auto& [key, value] : InitAnimation1)
    {
        if (value)
        {
            value->Release(); // DirectX 텍스처 리소스 해제
        }
    }
    InitAnimation1.clear();
}

// -------------------------------------
// 로딩 애니메이션 초기화
// -------------------------------------
void LoadingMap::init()
{
    InitAnimation_Path = LoadingMapPath; // 리소스 경로 설정

    // 63개의 프레임 로드 (스킬용 애니메이션 초기화 사용)
    InitAnim.InitAnimations_Skill(Temp, InitAnimation_Path, InitAnimation1, 63);
}

// -------------------------------------
// 프레임 갱신 및 이미지 렌더링
// -------------------------------------
void LoadingMap::Update(bool* CheckFinish)
{
    // 현재 프레임 이미지 가져오기
    ID3D11ShaderResourceView* FrameRect = InitAnim.Start_Animation_Update_For_Skiil(InitAnimation1, CheckFinish);

    if (!FrameRect) return;

    // 전체 창 크기 기준으로 로딩 이미지 출력
    ImVec2 WindowSize = G_ctx->MakeWindows->Get_WindowSize();
    G_ctx->MakeRender->Draw_Back_Image(
        FrameRect,
        Vec2(0.0f, 0.0f),
        Vec2(WindowSize.x, WindowSize.y),
        0.0f
    );
}
