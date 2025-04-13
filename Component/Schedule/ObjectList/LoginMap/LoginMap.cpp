#include "LoginMap.h"
#include "../../../../Global.h"

#include "LoginMap.h"
#include "../../../../Global.h"

// -------------------------------------
// 생성자: 초기값 설정
// -------------------------------------
LoginMap::LoginMap()
{
    CheckSound = true;  // 최초 1회만 배경음 재생
    IsFinish = true;    // 로딩 맵 먼저 출력
}

// -------------------------------------
// 소멸자 (리소스 해제 필요 시 사용 가능)
// -------------------------------------
LoginMap::~LoginMap()
{
}

// -------------------------------------
// 로그인 맵 초기화
// -------------------------------------
void LoginMap::init()
{
    // 로딩 맵 초기화
    LoadingMaps.init();

    // 배경 및 UI 이미지 경로 지정 및 로딩
    LoginUIPath   = LoginMapPath + std::string("LoginUi.png");
    LoginBackPath = LoginMapPath + std::string("LoginBackground.png");

    G_ctx->MakeRender->LoadTextureFromFileDX11(LoginUIPath.c_str(), &LoginUi, Temp);
    G_ctx->MakeRender->LoadTextureFromFileDX11(LoginBackPath.c_str(), &LoginBackground, Temp);
}

// -------------------------------------
// 매 프레임 업데이트 및 UI 출력
// -------------------------------------
void LoginMap::Update(bool* Islogin)
{
    // 로딩 애니메이션 출력이 끝나지 않은 경우 먼저 출력
    if (IsFinish)
    {
        LoadingMaps.Update(&IsFinish);
        return;
    }

    // 최초 진입 시 배경음 재생
    if (CheckSound)
    {
        G_ctx->G_SoundList.PlayBackgroundMusic("WelcometoMapleStory.mp3");
        CheckSound = false;
    }

    // ------------------------------
    // 배경 이미지 (전체 창 배경)
    // ------------------------------
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("BackgroundWindow", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground);

    ImGui::Image((void*)LoginBackground, ImGui::GetIO().DisplaySize);  // 배경 출력
    ImGui::End();

    // ------------------------------
    // 로그인 UI
    // ------------------------------
    ImVec2 loginPos = ImVec2(950.0f, 300.0f);
    ImVec2 loginSize = ImVec2(300.0f, 250.0f);

    ImGui::SetNextWindowPos(loginPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(loginSize, ImGuiCond_Always);
    ImGui::Begin("LoginUI", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);

    ImGui::Image((void*)LoginUi, loginSize); // 로그인 박스 출력

    // 텍스트 필드 디자인 변경 (투명 배경)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));

    // 사용자 입력
    ImGui::SetCursorPos(ImVec2(25.0f, 50.0f));
    ImGui::Text("Username");
    ImGui::SetCursorPosX(30.0f);
    ImGui::PushItemWidth(220.0f);
    ImGui::InputText("##username", _xGlobalStr_Account, IM_ARRAYSIZE(_xGlobalStr_Account));
    ImGui::PopItemWidth();

    // 비밀번호 입력
    ImGui::SetCursorPos(ImVec2(25.0f, 100.0f));
    ImGui::Text("Password");
    ImGui::SetCursorPosX(30.0f);
    ImGui::PushItemWidth(220.0f);
    ImGui::InputText("##password", _xGlobalStr_Password, IM_ARRAYSIZE(_xGlobalStr_Password), ImGuiInputTextFlags_Password);
    ImGui::PopItemWidth();

    ImGui::PopStyleColor(4); // 스타일 복원

    // ------------------------------
    // 로그인 버튼
    // ------------------------------
    ImGui::SetCursorPos(ImVec2(30.0f, 150.0f));
    if (ImGui::Button("Login", ImVec2(260.0f, 0)))
    {
        *Islogin = G_ctx->LoginCheck.Login(_xGlobalStr_Account, _xGlobalStr_Password);
        if (*Islogin)
        {
            G_ctx->G_ObjectManager->AddObjectManager("Maps");
            G_ctx->G_ObjectManager->AddObjectManager("Player");
            G_ctx->CanUseCalc = true;
        }
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));  // 버튼 간 간격

    // ------------------------------
    // 회원가입 버튼
    // ------------------------------
    ImGui::SetCursorPos(ImVec2(30.0f, 180.0f));
    ImGui::PushStyleColor(ImGuiCol_Button,         ImVec4(0.2f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(0.3f, 0.9f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,   ImVec4(0.1f, 0.7f, 0.2f, 1.0f));

    if (ImGui::Button("Register", ImVec2(260.0f, 0)))
    {
        G_ctx->LoginCheck.Register(_xGlobalStr_Account, _xGlobalStr_Password);
    }

    ImGui::PopStyleColor(3); // 버튼 스타일 복원
    ImGui::End();
}
