#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "RenderingObject.h"
#include"../Schedule.h"
#include"../../../Global.h"
#include <locale>
#include <codecvt>

RenderObject::RenderObject()
{
        hwnd = G_ctx->MakeWindows->Get_HWND();
        wc = G_ctx->MakeWindows->GetstaticWNDCLASSEXW();
        window_pos = { 0,0 };

}

RenderObject::~RenderObject()
{

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    //CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

bool RenderObject::CreateDeviceD3D()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void RenderObject::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void RenderObject::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

void RenderObject::SetupImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());


   
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    DrawManagers.DrawManager_Init();

  
  
}

void RenderObject::DrawInterface()
{

    if (!Islogin)
    {
        LoginMaps.Update(&Islogin);//true 면 게임화면
    }
    else
    {
       
        DrawManagers.Draw();
    }

}

void RenderObject::Main_Loop()
{
    const float clear_color_with_alpha[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        DrawInterface();
    }


    ImGui::Render();

    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0); // Present with vsync
}

void RenderObject::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

bool RenderObject::Init()
{

    if (!CreateDeviceD3D())
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return false;
    }
    SetupImGui();
   // DrawManagers.DrawManager_Init();
    LoginMaps.init();
    
}

RenderObject* RenderObject::Get_RenderObject(RenderObject* G_RenderObject)
{
    if (!G_RenderObject)
        G_RenderObject = new RenderObject();
    return G_RenderObject;
}
void RenderObject::Delete_RenderObject(RenderObject* G_RenderObject)
{
    delete G_RenderObject;
    G_RenderObject = nullptr;

}


Vec2 RenderObject::DrawLevelNumber(int Value, ID3D11ShaderResourceView * digitTextures[10],Vec2 Pos, Vec2 Size)
{
        std::string levelStr = std::to_string(Value);
        float spacing = 1.0f;              // 숫자 간 간격 (픽셀 단위, 원하면 조절 가능)
        Vec2 digitSize = Size;
        for (char ch : levelStr)
        {
            if (!isdigit(ch)) continue;

            int digit = ch - '0'; // '0' ~ '9' → 0 ~ 9
            Draw_Image(digitTextures[digit], Pos, Size, 0.0);
            ImGui::SameLine();
            Pos.x+= digitSize.x * 2 + spacing;
        }
        ImGui::NewLine();
        return Pos;
}






bool RenderObject::LoadTextureFromFileDX11(const char* fileName, ID3D11ShaderResourceView** out_texture, Vec2& Size)
{
    // Load from disk into a raw RGBA buffer
    int image_width;
    int image_height;
    unsigned char* image_data = stbi_load(fileName, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
   
    g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_texture);
    pTexture->Release();
    Size.x= image_width;
    Size.y = image_height;

    stbi_image_free(image_data);

    return true;
 
}
void RenderObject::Draw_Text(const Vec2& pos, const std::string& text, Color color)
{


    ImGui::GetForegroundDrawList()->AddText(ImVec2(pos.x, pos.y), color, text.c_str());
}

void RenderObject::Draw_Back_Image(ImTextureID user_texture_id, const Vec2& Pos, const Vec2& Size, double direction)
{
   
    ImVec2 uv0 = ImVec2(0.0f, 0.0f);
    ImVec2 uv1 = ImVec2(1.0f, 1.0f);
    ImVec2 top_left(Pos.x, Pos.y);  // 왼쪽 상단
    ImVec2 bottom_right(Pos.x + Size.x, Pos.y + Size.y);  // 오른쪽 하단
    if (direction == 1.0) {
        // 좌우 반전: uv0.x와 uv1.x를 뒤집기
        uv0.x = 1.0f;
        uv1.x = 0.0f;
    }
    ImGui::GetForegroundDrawList()->AddImage(user_texture_id, top_left, bottom_right, uv0, uv1);


}
void RenderObject::Draw_Image(ImTextureID user_texture_id, const Vec2& Pos, const Vec2& Size, double direction)
{
    ImVec2 uv0 = ImVec2(0.0f, 0.0f);
    ImVec2 uv1 = ImVec2(1.0f, 1.0f);
    ImVec2 top_left(Pos.x- Size.x, Pos.y- Size.y);  // 왼쪽 상단
    ImVec2 bottom_right(Pos.x + Size.x, Pos.y + Size.y);  // 오른쪽 하단
    if (direction == 1.0) {
        // 좌우 반전: uv0.x와 uv1.x를 뒤집기
        uv0.x = 1.0f;
        uv1.x = 0.0f;
    }
    ImGui::GetForegroundDrawList()->AddImage(user_texture_id, top_left, bottom_right,uv0,uv1);
 
  


}
std::string RenderObject::W_Calc_text_Tran(const std::wstring& text)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(text);
}
Vec2 RenderObject::W_Calc_text_size(const std::wstring& text, int font_size)
{
   
    
    std::string Text = W_Calc_text_Tran(text);
    auto ret = ImGui::CalcTextSize(Text.c_str());

    return Vec2{ ret.x, ret.y };
}
Vec2 RenderObject::Calc_text_size(const std::string& text, int font_size)
{
    //int fontIdx = font_size >= g_dx->fonts.size() ? g_dx->fonts.size() - 1 : (font_size < 1 ? 1 : font_size);
   // ImGui::PushFont(g_dx->fonts.at(fontIdx));

    auto ret = ImGui::CalcTextSize(text.c_str());

  

    return Vec2{ ret.x, ret.y };
}
void RenderObject::screen_rectangle(const Vec2& p1, const Vec2& p2, uint32_t col, float rounding, float thickness, bool filled)
{
    if (!p1.is_valid() && !p2.is_valid())
        return;

    ImVec2 coord1{ p1.x, p1.y };
    ImVec2 coord2{ p2.x, p2.y };
    if (!filled)
        ImGui::GetForegroundDrawList()->AddRect(coord1, coord2, col, rounding, 0, thickness);
    else
        ImGui::GetForegroundDrawList()->AddRectFilled(coord1, coord2, col, rounding, 0);
}
void RenderObject::rectangle_2points(const Vec2& start, const Vec2& end, uint32_t color, const int width, const float rounding, const float thickness)
{
    if (!start.is_valid() && !end.is_valid())
        return;



}



