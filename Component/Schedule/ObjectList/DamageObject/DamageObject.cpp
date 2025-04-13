#include "DamageObject.h"
#include "../../../../Global.h"


// ---------------------------------------
// 생성자: 크리티컬 섹션 초기화
// ---------------------------------------
DamageObject::DamageObject()
{
    InitializeCriticalSection(&cs);  // 멀티스레드 동기화용
}

// ---------------------------------------
// 소멸자: 크리티컬 섹션 제거
// ---------------------------------------
DamageObject::~DamageObject()
{
    DeleteCriticalSection(&cs);
}

// ---------------------------------------
// 데미지 숫자 이미지 초기화
// ---------------------------------------
void DamageObject::Init()
{
    std::string PathExt = ".png";

    for (int i = 0; i < 10; i++)
    {
        // 레드 데미지 텍스처 경로 및 로딩
        RedDmgNumPath[i] = Dmgs + std::string("RedDamage0_0") + std::to_string(i) + PathExt;
        G_ctx->MakeRender->LoadTextureFromFileDX11(RedDmgNumPath[i].c_str(), &RedDmgDigitsTex[i], TMP);

        // 블루 데미지 텍스처 경로 및 로딩
        BlueDmgNumPath[i] = Dmgs + std::string("BlueDamage0_0") + std::to_string(i) + PathExt;
        G_ctx->MakeRender->LoadTextureFromFileDX11(BlueDmgNumPath[i].c_str(), &BlueDmgDigitsTex[i], TMP);
    }
}

// ---------------------------------------
// 현재 데미지 리스트 위치 갱신 (매 프레임)
// ---------------------------------------
void DamageObject::Set_NowDmg_Calc(double DeltaTime)
{
    if (NowDmg.empty()) return;

    for (auto& it : NowDmg)
    {
        // 오른쪽으로 조금 이동
        double Velocity_x = 5.0;
        float F_Velocity_x = static_cast<float>(Velocity_x * DeltaTime);
        it.Pos.x += F_Velocity_x;

        // 위로 살짝 떠오르게
        it.Pos.y -= 10.5f * DeltaTime;

        // 생존 시간 증가
        it.DmgEc += DeltaTime;
    }
}

// ---------------------------------------
// 새 데미지를 NowDmg 리스트에 추가
// ---------------------------------------
void DamageObject::Set_NowDmg(Animaion_Dmg& _Animaion_Dmg)
{
    EnterCriticalSection(&cs);

    // 화면에서 약간 위에 표시되도록 위치 보정
    _Animaion_Dmg.Pos.y -= 41.0f;

    NowDmg.push_back(_Animaion_Dmg);

    LeaveCriticalSection(&cs);
}

// ---------------------------------------
// 일정 시간 지난 데미지 삭제
// ---------------------------------------
void DamageObject::Check_Delete_NowDmg(double DeltaTime)
{
    if (NowDmg.empty()) return;

    for (auto it = NowDmg.begin(); it != NowDmg.end(); )
    {
        if (it->DmgEc > 2.5) // 2.5초 지나면 삭제
        {
            it = NowDmg.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// ---------------------------------------
// 데미지 숫자 렌더링
// ---------------------------------------
void DamageObject::Draw_Damage_impact()
{
    if (NowDmg.empty()) return;

    Vec2 Size = BackGroundPos::MainBar::Damage::DamageSize;

    for (auto& it : NowDmg)
    {
        if (!it.IsPlayer)
        {
            // 몬스터 피해 (빨간색)
            G_ctx->MakeRender->DrawLevelNumber(std::round(it.Dmg), RedDmgDigitsTex, it.Pos, Size);
        }
        else
        {
            // 플레이어 피해 (파란색)
            G_ctx->MakeRender->DrawLevelNumber(std::round(it.Dmg), BlueDmgDigitsTex, it.Pos, Size);
        }
    }
}

