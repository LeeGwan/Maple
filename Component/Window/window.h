#pragma once

#include "../../pch.h"
#include "../Schedule/Engine/Timer/Timer.h"

// ImGui Win32 메시지 처리 핸들러 선언
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// window 클래스: 전체 프로그램의 Win32 창 생성, 메시지 처리, 렌더 루프를 담당
class window
{
private:
    // 윈도우의 전체 해상도 (화면 크기)
    static ImVec2 WindowSize;

    // 게임 맵 영역의 해상도 (여백 제외한 영역)
    static ImVec2 MapSize;

    // 생성된 윈도우 핸들
    static HWND Hwnd;

    // 등록된 윈도우 클래스 정보
    static WNDCLASSEXW Wc;

public:
    // 생성자: 해상도 및 크기 초기 설정
    window();

    /**
     * @brief Win32 메시지 콜백 처리 함수
     * @param hWnd    윈도우 핸들
     * @param msg     메시지 타입 (예: 마우스/키보드/크기조절 등)
     * @param wParam  메시지 추가 정보
     * @param lParam  메시지 추가 정보
     * @return 처리된 메시지 결과값
     */
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    /**
     * @brief 윈도우 및 클래스 초기화 함수
     * @param hInstance 프로그램 인스턴스 핸들
     * @return 성공 시 true, 실패 시 false
     */
    static bool init(HINSTANCE hInstance);

    /**
     * @brief 윈도우 메인 루프를 실행 (렌더링 포함)
     * @param hInstance 프로그램 인스턴스 핸들
     */
    static void Update(HINSTANCE hInstance);

    /**
     * @brief 생성된 윈도우 핸들을 반환
     * @return HWND
     */
    static HWND Get_HWND();

    /**
     * @brief 전체 윈도우 크기 반환 (ImGui 좌표계 기준)
     * @return ImVec2
     */
    static ImVec2 Get_WindowSize();

    /**
     * @brief 맵이 차지하는 영역의 크기 반환
     * @return ImVec2
     */
    ImVec2 Get_MapSize();

    /**
     * @brief 등록된 윈도우 클래스 정보 반환
     * @return WNDCLASSEXW
     */
    static WNDCLASSEXW GetstaticWNDCLASSEXW();
};
