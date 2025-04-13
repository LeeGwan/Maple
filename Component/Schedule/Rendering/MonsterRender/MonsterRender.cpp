#include "MonsterRender.h"
#include "../Color/Color.h"

// ------------------------------------------------------------
// 소멸자: 리소스 해제
// ------------------------------------------------------------
MonsterRender::~MonsterRender()
{
    for (int i = 0; i < 2; i++)
    {
        if (Resource[i])
            Resource[i]->Release();
    }
}

// ------------------------------------------------------------
// 생성자 (필요시 기본 초기화 가능)
// ------------------------------------------------------------
MonsterRender::MonsterRender()
{
    // 필드 초기화는 Init()에서 수행
}

// ------------------------------------------------------------
// Init: 애니메이션 및 체력바 텍스처 로딩
// ------------------------------------------------------------
void MonsterRender::Init(std::string Name, Vec2& Pos, std::vector<int> AnimationCout)
{
    std::string Path = Monster + Name + "\\";

    // 애니메이션 초기화
    Anim.InitAnimations_Move(Size, Path, Animations, AnimationCout);

    // 크기 지정
    Size = { 25.0f, 30.0f };

    // 체력바 리소스 경로 지정
    ResourcePath[0] = BottomUI + std::string("BarBgr.png"); // 배경
    ResourcePath[1] = BottomUI + std::string("Hp.png");      // HP 채움

    DmgTime = 0.0;

    // 리소스 로드
    for (int i = 0; i < 2; i++)
    {
        G_ctx->MakeRender->LoadTextureFromFileDX11(ResourcePath[i].c_str(), &Resource[i], TMP);
    }
}

// ------------------------------------------------------------
// Update: 애니메이션 프레임 갱신 및 렌더링
// ------------------------------------------------------------
void MonsterRender::Update(Vec2& Pos, double Direction)
{
    float DistanceOrSpeed = 0.0f;

    // 일정 거리 이상 움직였을 경우만 속도 적용
    if (Pos.distance(BeforeVec) > 20.0f)
    {
        DistanceOrSpeed = Pos.distance(BeforeVec);
    }

    AnimationType Temp_State = Current_AnimationType_State;

    ID3D11ShaderResourceView* FrameRect =
        Anim.Start_Animation_Update(Animations, Temp_State, DistanceOrSpeed);

    if (!FrameRect)
        return;

    Current_AnimationType_State = Temp_State;

    // 캐릭터 렌더링
    G_ctx->MakeRender->Draw_Image(FrameRect, Pos, Size, Direction);

    // 이전 위치 갱신
    BeforeVec = Vec2(Pos);
}

// ------------------------------------------------------------
// Draw_Hp_Bar: 몬스터 HP 바 그리기
// ------------------------------------------------------------
void MonsterRender::Draw_Hp_Bar(Vec2& Pos, float MaxState, float State)
{
    // 배경 바 위치 및 사이즈
    Vec2 BG_Pos = Pos + BackGroundPos::MainBar::Hp::Monster_HpBarPosBackGroundOffset;
    Vec2 BG_Size = BackGroundPos::MainBar::Hp::Monster_HpBarBackGroundSize;

    // HP 바 위치 및 사이즈 (프로그래스 반영)
    Vec2 HP_Pos = Pos + BackGroundPos::MainBar::Hp::Monster_HpBarPosOffset;
    Vec2 HP_Size = BackGroundPos::MainBar::Hp::Monster_HpBarSize;
    float ratio = State / MaxState;
    Vec2 barSize = { HP_Size.x * ratio, HP_Size.y };

    // 렌더링
    G_ctx->MakeRender->Draw_Back_Image(Resource[0], BG_Pos, BG_Size, 0.0);
    G_ctx->MakeRender->Draw_Back_Image(Resource[1], HP_Pos, barSize, 0.0);
}

// ------------------------------------------------------------
// 상태 접근자
// ------------------------------------------------------------
AnimationType MonsterRender::Get_Current_AnimationType_State()
{
    return Current_AnimationType_State;
}

void MonsterRender::Set_Current_AnimationType_State(AnimationType ChangeState)
{
    Current_AnimationType_State = ChangeState;
}

// ------------------------------------------------------------
// 몬스터 크기 반환
// ------------------------------------------------------------
Vec2 MonsterRender::Get_Size()
{
    return Size;
}
