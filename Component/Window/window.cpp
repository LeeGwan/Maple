#include "window.h"
#include "../Schedule/Rendering/RenderingObject.h"
#include "../Schedule/Schedule.h"
#include "../../Global.h"

// 전역 static 멤버 변수 초기화
ImVec2 window::WindowSize;
ImVec2 window::MapSize;
HWND window::Hwnd;
WNDCLASSEXW window::Wc;

// 생성자: 윈도우 및 맵 사이즈 초기 설정
window::window()
{
    WindowSize.x = 1775.0f;
    WindowSize.y = 670.0f;
    MapSize.x = WindowSize.x - 100.0f; // 맵은 오른쪽 여백 100
    MapSize.y = WindowSize.y;
}

// Win32 메시지 처리 콜백 함수
LRESULT __stdcall window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool isMinimized = false;
    static bool isDragging = false;
    static POINT dragStartPoint;
    static POINT windowStartPoint;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SYSCOMMAND:
        // ALT 키를 눌렀을 때 시스템 메뉴 비활성화
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;

        // 최소화 시 플래그 설정
        if (wParam == SC_MINIMIZE) 
        {
            ::ShowWindow(hWnd, SW_MINIMIZE);
            isMinimized = true;
            return 0;
        }
        break;

    case WM_ENTERSIZEMOVE:
        // 드래그 상태 진입 시 복원
        isMinimized = false;
        ::ShowWindow(hWnd, SW_RESTORE);
        return 0;

    case WM_NCHITTEST:
    {
        // 창 타이틀바 드래그 가능하게 설정
        UINT hitTest = DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
        return (hitTest == HTCAPTION || hitTest == HTSYSMENU) ? HTCAPTION : hitTest;
    }

    case WM_CLOSE:
        ::DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        ::PostQuitMessage(0); // 종료 메시지 전송
        return 0;

    case WM_LBUTTONDOWN:
        // 마우스 클릭 시 창 이동 위한 초기 위치 기록
        if (!isMinimized)
        {
            GetCursorPos(&windowStartPoint);
            RECT windowRect;
            GetWindowRect(hWnd, &windowRect);
            dragStartPoint.x = windowStartPoint.x - windowRect.left;
            dragStartPoint.y = windowStartPoint.y - windowRect.top;
            isDragging = true;
        }
        return 0;

    case WM_LBUTTONUP:
        // 드래그 종료
        isDragging = false;
        ReleaseCapture();
        return 0;

    case WM_MOUSEMOVE:
        // 마우스 이동 중일 때 창 위치 변경
        if (!isMinimized && isDragging)
        {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            SetWindowPos(hWnd, nullptr, cursorPos.x - dragStartPoint.x, cursorPos.y - dragStartPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        return 0;
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// 메인 윈도우 루프
void window::Update(HINSTANCE hInstance)
{
    if (!init(hInstance)) return;

    // 렌더링 객체와 오브젝트 매니저 생성
    G_ctx->MakeRender = make_shared<RenderObject>();
    G_ctx->G_ObjectManager = make_shared<ObjectManager>();
    G_ctx->MakeRender->Init();

    // 메시지 루프 시작
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }

        if (done)
            break;

        G_ctx->MakeRender->Main_Loop(); // 렌더링 루프

        timeBeginPeriod(1); Sleep(16); timeEndPeriod(1); // 약 60 FPS 유지
    }
}

// 창 핸들 반환
HWND window::Get_HWND() { return Hwnd; }

// 전체 윈도우 사이즈 반환
ImVec2 window::Get_WindowSize() { return WindowSize; }

// 게임 맵의 크기 반환
ImVec2 window::Get_MapSize() { return MapSize; }

// 등록된 윈도우 클래스 반환
WNDCLASSEXW window::GetstaticWNDCLASSEXW() { return Wc; }

// 윈도우 생성 및 초기화
bool window::init(HINSTANCE hInstance)
{
    // Win32 윈도우 클래스 정의
    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);

    // 전역 저장용 구조체 복사
    Wc = windowClass;
    wstring ClassName = L"MapleStory";

    Wc.lpszClassName = ClassName.c_str();
    ::RegisterClassExW(&Wc);

    // 창 생성
    Hwnd = ::CreateWindowW(
        Wc.lpszClassName, ClassName.c_str(),
        WS_POPUP | CW_USEDEFAULT,
        0, 0, WindowSize.x, WindowSize.y,
        NULL, NULL, Wc.hInstance, NULL);

    if (!Hwnd)
    {
        MessageBoxA(NULL, "Faild to CreateWindowW", "ERROR", MB_ERR_INVALID_CHARS);
        return false;
    }

    // 화면 중앙에 위치 조정
    RECT rc = { 0 };
    if (!GetWindowRect(Hwnd, &rc))
    {
        MessageBoxA(NULL, "Faild to GetWindowRect", "ERROR", MB_ERR_INVALID_CHARS);
        return false;
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - WindowSize.x) / 2;
    int posY = (screenHeight - WindowSize.y) / 2;

    ::SetWindowPos(Hwnd, NULL, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    ::ShowWindow(Hwnd, SW_SHOWDEFAULT);

    if (!::UpdateWindow(Hwnd))
    {
        MessageBoxA(NULL, "Faild to UpdateWindow", "ERROR", MB_ERR_INVALID_CHARS);
        return false;
    }

    return true;
}
