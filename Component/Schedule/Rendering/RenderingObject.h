#pragma once
#include "../../../pch.h"
#include"../../Window/window.h"
#include "../Rendering/DrawManager.h"
#include "Color/Color.h"

#include "../ObjectList/LoginMap/LoginMap.h"

class RenderObject
{
private:
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	UINT                     g_ResizeWidth, g_ResizeHeight;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	WNDCLASSEXW wc;
	HWND hwnd;
	ImVec2 window_pos;
	string Path;
	LoginMap LoginMaps;
	DrawManager DrawManagers;
	
private:
	bool Islogin = false;
	

public:
	RenderObject();
	~RenderObject();
	bool CreateDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	void SetupImGui();
	void DrawInterface();
	void Main_Loop();
	void CleanupDeviceD3D();
	bool Init();

	

public:

	bool LoadTextureFromFileDX11(const char* fileName, ID3D11ShaderResourceView** out_texture, Vec2& Size);
	void Draw_Text(const Vec2& pos, const std::string& text, Color color);
	void Draw_Back_Image(ImTextureID user_texture_id, const Vec2& Pos, const Vec2& Size, double direction);
	void Draw_Image(ImTextureID user_texture_id, const Vec2& p_min, const Vec2& p_max,double direction);
	std::string W_Calc_text_Tran(const std::wstring& text);
	Vec2 W_Calc_text_size(const std::wstring& text, int font_size);
	Vec2 Calc_text_size(const std::string& text, int font_size);
	void screen_rectangle(const Vec2& p1, const Vec2& p2, uint32_t col, float rounding, float thickness, bool filled);
	void rectangle_2points(const Vec2& start, const Vec2& end, uint32_t color, const int width, const float rounding, const float thickness);
	static RenderObject* Get_RenderObject(RenderObject* G_RenderObject);
	static void Delete_RenderObject(RenderObject* G_RenderObject);
	Vec2 DrawLevelNumber(int Value, ID3D11ShaderResourceView* digitTextures[10], Vec2 Pos, Vec2 Size);
};
