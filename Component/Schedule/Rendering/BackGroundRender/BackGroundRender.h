#pragma once
#include "../EachImageRender.h"
#include <string>

// ------------------------------------------------------------
// BackGroundRender: 게임 배경 및 하단 UI 렌더링 클래스
// - 배경 이미지
// - 메인 하단바
// - 경험치 바
// ------------------------------------------------------------
class BackGroundRender : public EachImageRender
{
private:
    // 텍스처 경로
    std::string Path;           // 배경 이미지 경로
    std::string MainBarName;    // 하단 UI 바 이미지 경로
    std::string ExpBarName;     // 경험치 바 이미지 경로

    // DirectX 텍스처 리소스
    ID3D11ShaderResourceView* BackGroundResource = nullptr;
    ID3D11ShaderResourceView* MainBarResource    = nullptr;
    ID3D11ShaderResourceView* ExpBarResource     = nullptr;

    // 화면 크기 기준 렌더 사이즈
    Vec2 Size;

    // UI 바 이미지의 크기 (디자인 기준으로 설정)
    Vec2 MainBar_2D;
    Vec2 ExpBarBar_2D;

public:
    BackGroundRender();
    ~BackGroundRender() override;

    // 오버라이드 함수 (기본 구조 유지)
    void Init(std::string Name, Vec2& Pos, std::vector<int> AnimationCout) override;

    // 배경 + UI 바 그리기
    void Update(Vec2& Pos, double Direction) override;

    // 배경 사이즈 반환
    Vec2 Get_Size() override;
};
