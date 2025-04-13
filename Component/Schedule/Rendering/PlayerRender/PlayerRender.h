#pragma once

#include "../RenderingObject.h"
#include "../EachImageRender.h"
#include <map>

// ------------------------------------------------------------
// PlayerRender
// - 플레이어의 애니메이션과 UI 요소를 렌더링하는 클래스
// ------------------------------------------------------------
class PlayerRender : public EachImageRender
{
private:
    // 애니메이션 관련
    std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>> Animations; // 상태별 애니메이션 프레임
    Animation Anim;                        // 애니메이션 처리 객체
    AnimationType Current_AnimationType_State; // 현재 상태
    Vec2 BeforeVec;                        // 이전 위치 (속도 계산용)
    Vec2 Size;                             // 플레이어 렌더링 크기

    // UI 텍스처 경로
    std::string Path[4];                  // [0]=HP, [1]=MP, [2]=XP bar, [3]=XP label
    std::string LevelNumPath[10];         // 레벨 숫자 텍스처
    std::string HpNumPath[10];            // HP 숫자 텍스처
    std::string ExpNumPath[10];           // XP 숫자 텍스처
    std::string SlashPath;                // 슬래시 ("/") 텍스처
    std::string NamePath;                 // 이름 칸 텍스처

    // UI 텍스처 핸들
    ID3D11ShaderResourceView* BarResource[4];     // HP/MP/XP/XP라벨 바 텍스처
    ID3D11ShaderResourceView* levelDigitsTex[10]; // 레벨 숫자 텍스처
    ID3D11ShaderResourceView* HpDigitsTex[10];    // HP 숫자 텍스처
    ID3D11ShaderResourceView* ExpDigitsTex[10];   // XP 숫자 텍스처
    ID3D11ShaderResourceView* SlashResource;      // 슬래시 기호 텍스처
    ID3D11ShaderResourceView* NameResource;       // 이름 칸 텍스처

    Vec2 TMP;      // 텍스처 로드용 임시 벡터

    // 레벨업 애니메이션
    std::map<int, ID3D11ShaderResourceView*> LevelUpAnimation;
    Animation LevelUpAnim;
    Vec2 TMP1;

public:
    PlayerRender();
    virtual ~PlayerRender() override = default;

    // 초기화: 애니메이션 및 텍스처 로딩
    void Init(std::string Name, Vec2& Pos, std::vector<int> AnimationCout) override;

    // 플레이어 상태 바 및 UI 요소 렌더링
    void Draw_Lv(int Lv);
    void Draw_Hp_Bar(float MaxState, float State);
    void Draw_Mp_Bar(float MaxState, float State);
    void Draw_Xp_Bar(float MaxState, float State);
    void Draw_LevelUp(Vec2 Pos, bool* Is_Level_Up);
    void Draw_Name(Vec2 Pos, const std::wstring& Name);

    // 애니메이션 업데이트 및 캐릭터 렌더링
    void Update(Vec2& Pos, double Direction) override;

    // 상태 접근자
    AnimationType Get_Current_AnimationType_State();
    void Set_Current_AnimationType_State(AnimationType ChangeState);
    Vec2 Get_Size() override;
};
