// RenderObject.cpp - 전체 게임 렌더링 및 ImGui 출력 담당 클래스 구현

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "RenderingObject.h"
#include "../Schedule.h"
#include "../../../Global.h"
#include <locale>
#include <codecvt>

// 생성자 - 윈도우 핸들 및 클래스 정보를 저장
RenderObject::RenderObject()
{
    hwnd = G_ctx->MakeWindows->Get_HWND();
    wc = G_ctx->MakeWindows->GetstaticWNDCLASSEXW();
    window_pos = { 0, 0 };
}

// 소멸자 - ImGui 및 D3D 리소스 정리, 윈도우 제거
RenderObject::~RenderObject()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

// Direct3D 11 디바이스 및 스왑체인 생성
bool RenderObject::CreateDeviceD3D()
{
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL levelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        levelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice,
        &featureLevel, &g_pd3dDeviceContext);

    // 하드웨어 실패 시 소프트웨어로 재시도
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0,
            levelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice,
            &featureLevel, &g_pd3dDeviceContext);

    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}

// 렌더 타겟 생성
void RenderObject::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

// 렌더 타겟 정리
void RenderObject::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// ImGui 초기화
void RenderObject::SetupImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    DrawManagers.DrawManager_Init();
}

// 로그인 상태에 따라 UI or 게임 씬 렌더링
void RenderObject::DrawInterface()
{
    if (!Islogin)
        LoginMaps.Update(&Islogin); // 로그인 UI
    else
        DrawManagers.Draw();       // 전체 게임 오브젝트 출력
}

// 렌더링 메인 루프 - 프레임마다 호출
void RenderObject::Main_Loop()
{
    const float clear_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    DrawInterface(); // 실제 콘텐츠 그리기

    ImGui::Render();
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0); // VSync 켜고 출력
}

// D3D 리소스 해제
void RenderObject::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

// 렌더러 초기화 진입점
bool RenderObject::Init()
{
    if (!CreateDeviceD3D())
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return false;
    }
    SetupImGui();
    LoginMaps.init();
    return true;
}

// 텍스처 로드 (png 등) + 이미지 크기 반환
bool RenderObject::LoadTextureFromFileDX11(const char* fileName, ID3D11ShaderResourceView** out_texture, Vec2& Size)
{
    int w, h;
    unsigned char* data = stbi_load(fileName, &w, &h, NULL, 4);
    if (!data) return false;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = w;
    desc.Height = h;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA subResource = { data, w * 4, 0 };
    ID3D11Texture2D* pTex = nullptr;
    g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTex);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    g_pd3dDevice->CreateShaderResourceView(pTex, &srvDesc, out_texture);
    pTex->Release();
    stbi_image_free(data);

    Size = Vec2(w, h);
    return true;
}

// 텍스트 출력
void RenderObject::Draw_Text(const Vec2& pos, const std::string& text, Color color)
{
    ImGui::GetForegroundDrawList()->AddText(ImVec2(pos.x, pos.y), color, text.c_str());
}

// 이미지 출력 (정방향)
void RenderObject::Draw_Back_Image(ImTextureID tex, const Vec2& pos, const Vec2& size, double dir)
{
    ImVec2 uv0(0.0f, 0.0f), uv1(1.0f, 1.0f);
    if (dir == 1.0) std::swap(uv0.x, uv1.x);
    ImGui::GetForegroundDrawList()->AddImage(tex, ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), uv0, uv1);
}

// 이미지 출력 (중심 기준)
void RenderObject::Draw_Image(ImTextureID tex, const Vec2& pos, const Vec2& size, double dir)
{
    ImVec2 uv0(0.0f, 0.0f), uv1(1.0f, 1.0f);
    if (dir == 1.0) std::swap(uv0.x, uv1.x);
    ImVec2 tl(pos.x - size.x, pos.y - size.y);
    ImVec2 br(pos.x + size.x, pos.y + size.y);
    ImGui::GetForegroundDrawList()->AddImage(tex, tl, br, uv0, uv1);
}

// 유니코드 -> UTF-8 변환
std::string RenderObject::W_Calc_text_Tran(const std::wstring& text)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(text);
}

// 유니코드 문자열의 픽셀 크기 계산
Vec2 RenderObject::W_Calc_text_size(const std::wstring& text, int font_size)
{
    return Calc_text_size(W_Calc_text_Tran(text), font_size);
}

// 일반 문자열의 픽셀 크기 계산
Vec2 RenderObject::Calc_text_size(const std::string& text, int font_size)
{
    ImVec2 sz = ImGui::CalcTextSize(text.c_str());
    return Vec2(sz.x, sz.y);
}

// 직사각형 그리기 (선 or 채우기)
void RenderObject::screen_rectangle(const Vec2& p1, const Vec2& p2, uint32_t col, float rounding, float thickness, bool filled)
{
    ImVec2 a(p1.x, p1.y), b(p2.x, p2.y);
    if (!filled)
        ImGui::GetForegroundDrawList()->AddRect(a, b, col, rounding, 0, thickness);
    else
        ImGui::GetForegroundDrawList()->AddRectFilled(a, b, col, rounding, 0);
}

// (미사용) 두 지점 기준 직사각형 그리기
void RenderObject::rectangle_2points(const Vec2& start, const Vec2& end, uint32_t color, const int width, const float rounding, const float thickness)
{
    // 예약 함수 - 필요 시 구현
}

// 숫자 출력용 텍스처 배열로 레벨 숫자 그리기
Vec2 RenderObject::DrawLevelNumber(int value, ID3D11ShaderResourceView* digits[10], Vec2 pos, Vec2 size)
{
    std::string numStr = std::to_string(value);
    float spacing = 1.0f;
    for (char c : numStr)
    {
        int d = c - '0';
        Draw_Image(digits[d], pos, size, 0.0);
        pos.x += size.x * 2 + spacing;
    }
    return pos;
}
