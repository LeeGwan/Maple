#include "LoginMap.h"
#include "../../../../Global.h"

LoginMap::LoginMap()
{
	CheckSound = true;
	IsFinish = true;
	
}

LoginMap::~LoginMap()
{
}

void LoginMap::init()
{
	
	LoadingMaps.init();

	LoginUIPath = LoginMapPath + std::string("LoginUi.png");
	LoginBackPath= LoginMapPath + std::string("LoginBackground.png");
	G_ctx->MakeRender->LoadTextureFromFileDX11(LoginUIPath.c_str(), &LoginUi, Temp);
	G_ctx->MakeRender->LoadTextureFromFileDX11(LoginBackPath.c_str(), &LoginBackground, Temp);

}

void LoginMap::Update(bool* Islogin)
{
	if (IsFinish)
	{
		LoadingMaps.Update(&IsFinish);
		return;
	}


	if (CheckSound)
	{
		G_ctx->G_SoundList.PlayBackgroundMusic("WelcometoMapleStory.mp3");
		CheckSound = false;

	}
	// 배경 이미지
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
	ImGui::Begin("BackgroundWindow", nullptr,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBackground);

	ImGui::Image((void*)LoginBackground, ImGui::GetIO().DisplaySize); // 배경 이미지를 화면 전체에
	ImGui::End();


	// 로그인 UI 이미지 + 입력 필드
	ImVec2 loginPos = ImVec2(950.0f, 300.0f);
	ImVec2 loginSize = ImVec2(300.0f, 250.0f);

	ImGui::SetNextWindowPos(loginPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(loginSize, ImGuiCond_Always);
	ImGui::Begin("LoginUI", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoBackground| ImGuiWindowFlags_NoScrollbar);

	ImGui::Image((void*)LoginUi, loginSize); // 로그인 박스 이미지
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));         // 배경 투명
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));  // 호버 시도 투명
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
	ImGui::SetCursorPos(ImVec2(25.0f, 50.0f)); // 이미지 안쪽 위치에 맞춰 UI 배치
	ImGui::Text("Username");
	ImGui::SetCursorPosX(30.0f);
	ImGui::PushItemWidth(220.0f); // 텍스트박스 너비
	ImGui::InputText("##username", _xGlobalStr_Account, IM_ARRAYSIZE(_xGlobalStr_Account));
	ImGui::PopItemWidth();
	
	ImGui::SetCursorPos(ImVec2(25.0f, 100.0f));
	ImGui::Text("Password");
	ImGui::SetCursorPosX(30.0f);
	ImGui::PushItemWidth(220.0f);
	ImGui::InputText("##password", _xGlobalStr_Password, IM_ARRAYSIZE(_xGlobalStr_Password), ImGuiInputTextFlags_Password);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(4);
	ImGui::SetCursorPos(ImVec2(30.0f, 150.0f));
	
	if (ImGui::Button("Login", ImVec2(260.0f, 0))) 
	{
		*Islogin = G_ctx->LoginCheck.Login(_xGlobalStr_Account,_xGlobalStr_Password);
		if(*Islogin)
		{
		G_ctx->G_ObjectManager->AddObjectManager("Maps");
		G_ctx->G_ObjectManager->AddObjectManager("Player");
		G_ctx->CanUseCalc = true;
		}
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::SetCursorPos(ImVec2(30.0f, 180.0f));
	// 버튼 스타일
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.3f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.9f, 0.4f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.7f, 0.2f, 1.0f));

	if (ImGui::Button("Register", ImVec2(260.0f, 0))) {
		G_ctx->LoginCheck.Register(_xGlobalStr_Account, _xGlobalStr_Password);
	}

	ImGui::PopStyleColor(3);
	ImGui::End();


}
