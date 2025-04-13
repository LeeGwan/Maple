#include "window.h"
#include"../Schedule/Rendering/RenderingObject.h"
#include "../Schedule/Schedule.h"
#include "../../Global.h"
ImVec2 window::WindowSize;
ImVec2 window::MapSize;
HWND window::Hwnd;
WNDCLASSEXW window::Wc;



window::window()
{
    WindowSize.x = 1775.0f;
    WindowSize.y = 670.0f;
    MapSize.x = 1775.0f-100.0f;
    MapSize.y = 670.0f;
   
}



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
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;

        if (wParam == SC_MINIMIZE) // Minimize the window
        {
            ::ShowWindow(hWnd, SW_MINIMIZE);
            isMinimized = true;
            return 0;
        }
        break;
    case WM_ENTERSIZEMOVE:
        isMinimized = false;
        ::ShowWindow(hWnd, SW_RESTORE);
        return 0;
    case WM_EXITSIZEMOVE:
        return 0;
    case WM_NCHITTEST:
    {
        // Handle hit-testing for resizing and moving the window
        UINT hitTest = DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
        if (hitTest == HTCAPTION || hitTest == HTSYSMENU)
            return HTCAPTION;
        else
            return hitTest;
    }
    case WM_CLOSE:

        ::DestroyWindow(hWnd); // Close the window
        return 0;
    case WM_DESTROY:
      

        ::PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
    {
        if (!isMinimized)
        {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            windowStartPoint.x = cursorPos.x;
            windowStartPoint.y = cursorPos.y;
            RECT windowRect;
            GetWindowRect(hWnd, &windowRect);
            dragStartPoint.x = cursorPos.x - windowRect.left;
            dragStartPoint.y = cursorPos.y - windowRect.top;

            isDragging = true;
        }
    }
    return 0;
    case WM_LBUTTONUP:
    {
        isDragging = false;
        ReleaseCapture();
    }
    return 0;
    case WM_MOUSEMOVE:
    {
        if (!isMinimized && isDragging)
        {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            int deltaX = cursorPos.x - windowStartPoint.x;
            int deltaY = cursorPos.y - windowStartPoint.y;
            SetWindowPos(hWnd, nullptr, cursorPos.x - dragStartPoint.x, cursorPos.y - dragStartPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
    }
    return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);

}

void window::Update(HINSTANCE hInstance)
{
    if (!init(hInstance))return;

    G_ctx->MakeRender = make_shared<RenderObject>();
    G_ctx->G_ObjectManager = make_shared<ObjectManager>();
    G_ctx->MakeRender->Init();
 

    

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

       
       

        G_ctx->MakeRender->Main_Loop();
        timeBeginPeriod(1);
        Sleep(16);
        timeEndPeriod(1);
      
    }
    
}

HWND window::Get_HWND()
{
    return Hwnd;
}

ImVec2 window::Get_WindowSize()
{
    return WindowSize;
}
ImVec2 window::Get_MapSize()
{
    return MapSize;
}

WNDCLASSEXW window::GetstaticWNDCLASSEXW()
{
    return Wc;
}





bool window::init(HINSTANCE hInstance)//윈도우 초기화 설정
{
    WNDCLASSEX windowClass = { };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.hInstance = hInstance;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = NULL;
    windowClass.cbWndExtra = NULL;
    windowClass.hInstance = GetModuleHandleA(0);
    windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = NULL;
    wstring ClassName = L"MapleStory";
    Wc = { sizeof(Wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, ClassName.c_str(), NULL };
    ::RegisterClassExW(&Wc);
    Hwnd = ::CreateWindowW(Wc.lpszClassName, ClassName.c_str(), WS_POPUP | CW_USEDEFAULT, 0, 0, WindowSize.x, WindowSize.y, NULL, NULL, Wc.hInstance, NULL);
    if (!Hwnd)
    {
        MessageBoxA(NULL, "Faild to CreateWindowW", "ERROR", MB_ERR_INVALID_CHARS);
        return false;
    }
    RECT rc = { 0 };
    if (!GetWindowRect(Hwnd, &rc))
    {
        MessageBoxA(NULL, "Faild to GetWindowRect", "ERROR", MB_ERR_INVALID_CHARS);
        return false;
    }

    // 스크린의 가운데 위치 계산
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = WindowSize.x;
    int windowHeight = WindowSize.y;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // 센터에 포지션 설정
    ::SetWindowPos(Hwnd, NULL, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    ::ShowWindow(Hwnd, SW_SHOWDEFAULT);
    if(!::UpdateWindow(Hwnd))
    {
        MessageBoxA(NULL, "Faild to UpdateWindow", "ERROR", MB_ERR_INVALID_CHARS);
        return false;
    }
   
    return true;

}
