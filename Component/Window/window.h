#pragma once
#include "../../pch.h"
#include "../Schedule/Engine/Timer/Timer.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
class window
{
private:
	
	static ImVec2 WindowSize;
	static ImVec2 MapSize;
	static HWND Hwnd;
	static WNDCLASSEXW Wc;




	
public:


	window();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool init(HINSTANCE hInstance);
	static void Update(HINSTANCE hInstance);
	static HWND Get_HWND();
	static ImVec2 Get_WindowSize();
	ImVec2 Get_MapSize();
	static WNDCLASSEXW GetstaticWNDCLASSEXW();

};

