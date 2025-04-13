#pragma once

#include "../../../pch.h"
#include "../../Window/window.h"
#include "../Rendering/DrawManager.h"
#include "Color/Color.h"
#include "../ObjectList/LoginMap/LoginMap.h"

// RenderObject 클래스는 DirectX11을 기반으로 하는 렌더링 및 ImGui UI 처리를 담당하는 주 핵심 클래스입니다.
class RenderObject
{
private:
    // Direct3D 관련 장치 및 컨텍스트
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    // 윈도우 정보
    UINT g_ResizeWidth, g_ResizeHeight;
    WNDCLASSEXW wc;
    HWND hwnd;
    ImVec2 window_pos;
    
    // 리소스 경로
    string Path;

    // 로그인 처리 및 드로우 매니저
    LoginMap LoginMaps;
    DrawManager DrawManagers;

    // 로그인 상태 여부
    bool Islogin = false;

public:
    // 생성자 및 소멸자
    RenderObject();  // 객체 생성 시 윈도우 핸들 및 클래스 정보 초기화
    ~RenderObject(); // 객체 소멸 시 리소스 해제 및 윈도우 제거

    // DirectX 장치 생성 및 렌더타겟 관련 함수
    bool CreateDeviceD3D();            // DirectX 장치와 스왑 체인 생성
    void CreateRenderTarget();        // 렌더타겟 뷰 생성
    void CleanupRenderTarget();       // 렌더타겟 뷰 해제
    void CleanupDeviceD3D();          // DirectX 장치 관련 전체 정리

    // ImGui 초기화 및 렌더링 인터페이스 처리
    void SetupImGui();                // ImGui 설정 및 폰트 로딩
    void DrawInterface();             // 인터페이스 그리기 (로그인/메인 화면 처리)
    void Main_Loop();                 // 메인 루프 (프레임 갱신 처리)

    // 렌더링 초기화
    bool Init();                      // 장치 및 UI 초기화 (초기 진입 시 호출)

    // 렌더링 및 텍스처 관련 유틸 함수들
    bool LoadTextureFromFileDX11(const char* fileName, ID3D11ShaderResourceView** out_texture, Vec2& Size); // 이미지 로딩
    void Draw_Text(const Vec2& pos, const std::string& text, Color color);                                  // 텍스트 출력
    void Draw_Back_Image(ImTextureID user_texture_id, const Vec2& Pos, const Vec2& Size, double direction); // 배경 이미지 출력
    void Draw_Image(ImTextureID user_texture_id, const Vec2& p_min, const Vec2& p_max, double direction);   // 일반 이미지 출력
    Vec2 DrawLevelNumber(int Value, ID3D11ShaderResourceView* digitTextures[10], Vec2 Pos, Vec2 Size);       // 숫자 이미지 출력

    // 텍스트 계산 관련 유틸 함수
    std::string W_Calc_text_Tran(const std::wstring& text);    // wide -> utf8 변환
    Vec2 W_Calc_text_size(const std::wstring& text, int font_size); // wide 문자열의 크기 계산
    Vec2 Calc_text_size(const std::string& text, int font_size);    // 일반 문자열의 크기 계산

    // 도형 렌더링 유틸
    void screen_rectangle(const Vec2& p1, const Vec2& p2, uint32_t col, float rounding, float thickness, bool filled); // 사각형 그리기
    void rectangle_2points(const Vec2& start, const Vec2& end, uint32_t color, const int width, const float rounding, const float thickness); // 2점 기반 사각형

    // 싱글턴 객체 생성 및 삭제
    static RenderObject* Get_RenderObject(RenderObject* G_RenderObject); // 생성
    static void Delete_RenderObject(RenderObject* G_RenderObject);       // 해제
};
