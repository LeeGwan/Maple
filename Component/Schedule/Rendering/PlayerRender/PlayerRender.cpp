#include "PlayerRender.h"
#include "../Color/Color.h"

// ------------------------------------------------------------
// 생성자
// ------------------------------------------------------------
PlayerRender::PlayerRender() {}

// ------------------------------------------------------------
// Init: 플레이어 애니메이션과 UI 텍스처 초기화
// ------------------------------------------------------------
void PlayerRender::Init(std::string Name, Vec2& Pos, std::vector<int> AnimationCout)
{
    // 애니메이션 세팅
    Anim.InitAnimations_Move(Size, Player, Animations, AnimationCout);
    LevelUpAnim.InitAnimations_Skill(TMP1, LevelUp, LevelUpAnimation, 20);

    // 기본 사이즈 지정
    Size = { 100.0f, 85.0f };

    // 리소스 경로 세팅
    SlashPath = BottomUI + "Slash.png";
    Path[0] = BottomUI + "Hp.png";
    Path[1] = BottomUI + "Mp.png";
    Path[2] = BottomUI + "Xp.png";
    Path[3] = BottomUI + "ExpString.png";
    NamePath = BottomUI + "Name.png";

    std::string Pathstr = ".png";

    // 숫자 이미지 경로 세팅
    for (int i = 0; i < 10; i++) {
        LevelNumPath[i] = BottomUI + "Lv_" + std::to_string(i) + Pathstr;
        HpNumPath[i] = BottomUI + "HPNum" + std::to_string(i) + Pathstr;
        ExpNumPath[i] = BottomUI + "ExpNum" + std::to_string(i) + Pathstr;
    }

    // 리소스 로딩
    G_ctx->MakeRender->LoadTextureFromFileDX11(SlashPath.c_str(), &SlashResource, TMP);
    G_ctx->MakeRender->LoadTextureFromFileDX11(NamePath.c_str(), &NameResource, TMP);

    for (int i = 0; i < 4; i++) {
        G_ctx->MakeRender->LoadTextureFromFileDX11(Path[i].c_str(), &BarResource[i], TMP);
    }

    for (int i = 0; i < 10; i++) {
        G_ctx->MakeRender->LoadTextureFromFileDX11(LevelNumPath[i].c_str(), &levelDigitsTex[i], TMP);
        G_ctx->MakeRender->LoadTextureFromFileDX11(HpNumPath[i].c_str(), &HpDigitsTex[i], TMP);
        G_ctx->MakeRender->LoadTextureFromFileDX11(ExpNumPath[i].c_str(), &ExpDigitsTex[i], TMP);
    }
}

// ------------------------------------------------------------
// 플레이어 이름 표시
// ------------------------------------------------------------
void PlayerRender::Draw_Name(Vec2 Pos, const std::wstring& Name)
{
    std::string convertedName = G_ctx->MakeRender->W_Calc_text_Tran(Name);
    Vec2 namePos = Pos + BackGroundPos::MainBar::Name::PlayerName::NamePosOffset;
    Vec2 textPos = namePos + Vec2(20.0f, 15.0f);
    Vec2 nameSize = BackGroundPos::MainBar::Name::PlayerName::NameBarSize;

    G_ctx->MakeRender->Draw_Back_Image(NameResource, namePos, nameSize, 0.0);
    G_ctx->MakeRender->Draw_Text(textPos, convertedName, Colors::Simple::lime);
    G_ctx->MakeRender->Draw_Text(BackGroundPos::MainBar::Name::NamePosOffset, convertedName, Colors::Simple::Legacy::White_font);
}

// ------------------------------------------------------------
// 레벨 숫자 그리기
// ------------------------------------------------------------
void PlayerRender::Draw_Lv(int Lv)
{
    Vec2 LvPos = BackGroundPos::MainBar::Lv::LvPosOffset;
    G_ctx->MakeRender->DrawLevelNumber(Lv, levelDigitsTex, LvPos, Vec2(4.0f, 4.0f));
}

// ------------------------------------------------------------
// 경험치 바 그리기
// ------------------------------------------------------------
void PlayerRender::Draw_Xp_Bar(float MaxState, float State)
{
    Vec2 root = BackGroundPos::Exp::ExpBarPosOffset;
    Vec2 barPos = root + Vec2(40.0f, 4.0f);
    Vec2 labelPos = root + Vec2(0.0f, 4.0f);
    Vec2 barSize = BackGroundPos::Exp::ExpBarSize;
    Vec2 labelSize = BackGroundPos::Exp::ExpStringSize;
    barSize.x -= 50.0f;

    float ratio = State / MaxState;
    Vec2 filled = { barSize.x * ratio, barSize.y - 8.0f };

    G_ctx->MakeRender->Draw_Back_Image(BarResource[2], barPos, filled, 0.0);
    G_ctx->MakeRender->Draw_Back_Image(BarResource[3], labelPos, labelSize, 0.0);

    // 현재 XP 숫자
    Vec2 xpNumPos = G_ctx->MakeRender->DrawLevelNumber(State, ExpDigitsTex, Vec2(barSize.x / 2.0f, barPos.y + 4.0f), Vec2(4.5f, 4.5f));
    xpNumPos.y -= 4.0f;

    // 슬래시 기호
    G_ctx->MakeRender->Draw_Back_Image(SlashResource, xpNumPos, Vec2(8.5f, 8.5f), 0.0);
    xpNumPos.x += 20.0f;
    xpNumPos.y += 4.0f;

    // Max XP
    G_ctx->MakeRender->DrawLevelNumber(MaxState, ExpDigitsTex, xpNumPos, Vec2(4.5f, 4.5f));
}

// ------------------------------------------------------------
// HP 바 그리기
// ------------------------------------------------------------
void PlayerRender::Draw_Hp_Bar(float MaxState, float State)
{
    Vec2 root = BackGroundPos::MainBar::Hp::HpBarPosOffset;
    Vec2 size = BackGroundPos::MainBar::Hp::HpBarSize;

    float ratio = State / MaxState;
    Vec2 filled = { size.x * ratio, size.y };

    G_ctx->MakeRender->Draw_Back_Image(BarResource[0], root, filled, 0.0);
    G_ctx->MakeRender->DrawLevelNumber(State, HpDigitsTex, Vec2((root.x + size.x) / 1.12f, root.y + 5.0f), Vec2(4.5f, 4.5f));
}

// ------------------------------------------------------------
// MP 바 그리기
// ------------------------------------------------------------
void PlayerRender::Draw_Mp_Bar(float MaxState, float State)
{
    Vec2 root = BackGroundPos::MainBar::Mp::MpBarPosOffset;
    Vec2 size = BackGroundPos::MainBar::Mp::MpBarSize;

    float ratio = State / MaxState;
    Vec2 filled = { size.x * ratio, size.y };

    G_ctx->MakeRender->Draw_Back_Image(BarResource[1], root, filled, 0.0);
}

// ------------------------------------------------------------
// 레벨업 애니메이션 출력
// ------------------------------------------------------------
void PlayerRender::Draw_LevelUp(Vec2 Pos, bool* Is_Level_Up)
{
    ID3D11ShaderResourceView* frame = LevelUpAnim.Start_Animation_Update_For_Skiil(LevelUpAnimation, Is_Level_Up);
    Vec2 drawPos = Pos + Vec2(10.0f, -300.0f);

    G_ctx->MakeRender->Draw_Image(frame, drawPos, Vec2(300.0f, 300.0f), 0.0);
}

// ------------------------------------------------------------
// 프레임 단위 플레이어 렌더링 업데이트
// ------------------------------------------------------------
void PlayerRender::Update(Vec2& Pos, double Direction)
{
    ObjectClass* player = G_ctx->G_ObjectManager->Get_Find_obj("Player");

    float DistanceOrSpeed = 0.0f;

    if (Current_AnimationType_State == AnimationType::Attack)
    {
        DistanceOrSpeed = player->Get_AttackSpeed();
    }
    else if (Pos.distance(BeforeVec) > 20.0f)
    {
        DistanceOrSpeed = Pos.distance(BeforeVec);
    }

    ID3D11ShaderResourceView* frame =
        Anim.Start_Animation_Update(Animations, Current_AnimationType_State, DistanceOrSpeed);

    if (!frame) return;

    G_ctx->MakeRender->Draw_Image(frame, Pos, Size, Direction);
    BeforeVec = Vec2(Pos);
}

// ------------------------------------------------------------
// 상태 조회/설정 및 크기 반환
// ------------------------------------------------------------
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
