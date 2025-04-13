#pragma once
#include "MapObject.h"

// ----------------------------------------
// 생성자: 맵 이름 설정, 배경음악 및 데미지 초기화
// ----------------------------------------
MapObject::MapObject(const std::string& _Name)
{
    Name = _Name;

    // 데미지 오브젝트 초기화 (텍스처 로딩 등)
    G_ctx->DmgObject.Init();

    // 배경 음악 재생
    G_ctx->G_SoundList.PlayBackgroundMusic("BackGroundMusic.mp3");
}

// ----------------------------------------
// 맵 이름 반환
// ----------------------------------------
std::string MapObject::Get_Name()
{
    return Name;
}

// ----------------------------------------
// 현재 프레임에서 데미지 애니메이션 출력
// ----------------------------------------
void MapObject::Draw_Dmg_Animation()
{
    G_ctx->DmgObject.Draw_Damage_impact();
}

// ----------------------------------------
// 매 프레임 업데이트 (맵 렌더 + 데미지 애니메이션)
// ----------------------------------------
void MapObject::Update()
{
    MapRender.Update(Pos, 0.0);  // 맵 위치 렌더링
    Draw_Dmg_Animation();        // 데미지 출력
}
