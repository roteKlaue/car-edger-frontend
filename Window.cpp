#include "pch.h"
#include "Window.h"
#include <windows.h>
#include "Resource.h"
#include "Frame.h"
#include <iostream>

const std::wstring Window::GENERIC_CLASS_NAME = L"GenericWndClass";
bool               Window::classRegistered    = false;

Window::Window(HINSTANCE instance, int nCmdShow)
    : instance(instance), nCmdShow(nCmdShow) {
}

Window::~Window()
{
    if (windowHandle) {
        DestroyWindow(windowHandle);
        windowHandle = nullptr;
    }
}

void Window::RegisterWindowClass() const {
    if (classRegistered) return;
    WNDCLASS wc = {};
    wc.lpfnWndProc = StaticWndProc;
    wc.hInstance = instance;
    wc.lpszClassName = GENERIC_CLASS_NAME.c_str();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);
    classRegistered = true;
}

void Window::Show() const {
    if (!windowHandle) return;
    ShowWindow(windowHandle, nCmdShow);
    Update();
}

void Window::Update() const
{
    InvalidateRect(windowHandle, nullptr, TRUE);
    UpdateWindow(windowHandle);
    DrawMenuBar(windowHandle);
}

bool Window::Init() {
    RegisterWindowClass();

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect(&rc, dwStyle, menuResource != -1);

    int totalW = rc.right - rc.left;
    int totalH = rc.bottom - rc.top;
    windowHandle = CreateWindowW(
        GENERIC_CLASS_NAME.c_str(), title.c_str(), dwStyle,
        (width < 0 ? CW_USEDEFAULT : 0),
        (height < 0 ? CW_USEDEFAULT : 0),
        (width < 0 ? CW_USEDEFAULT : rc.right - rc.left),
        (height < 0 ? CW_USEDEFAULT : rc.bottom - rc.top),
        nullptr, nullptr, instance, this);

    if (!windowHandle) return false;
    return initialized = true;
}

void Window::RunMessageLoop() const
{
    /*
    HACCEL hAccelTable = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_CAREDGER));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    */

    HACCEL hAccelTable = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_CAREDGER));
    MSG msg;
    std::wcout << L"[LOOP] Entering message loop" << std::endl;

    while (true) {
        BOOL got = GetMessage(&msg, nullptr, 0, 0);
        std::wcout << L"[LOOP] GetMessage returned " << got << std::endl;
        if (got == 0) break;
        if (got == -1) {
            std::wcout << L"[LOOP] GetMessage error: " << GetLastError() << std::endl;
            break;
        }

        if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) continue;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    std::wcout << L"[LOOP] Exited message loop" << std::endl;
}

void Window::RegisterSubWindow(std::shared_ptr<SubWindow> subWindow) {
    subWindows.push_back(std::move(subWindow));
}

void Window::LoadFrame(std::shared_ptr<Frame> frame, const json& options) {
    if (!frame || (currentlyLoadedFrame && currentlyLoadedFrame->GetUniqueIdentifier() == frame->GetUniqueIdentifier())) {
        return;
    }

    if (currentlyLoadedFrame) {
        currentlyLoadedFrame->Unload(this);
    }

    currentlyLoadedFrame = std::move(frame);
    currentlyLoadedFrame->WakeUp(this, options);
    Update();
}

LRESULT CALLBACK Window::StaticWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    Window* self = nullptr;
    if (msg == WM_NCCREATE) {
        auto cs = reinterpret_cast<CREATESTRUCT*>(lp);
        self = static_cast<Window*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)self);
    } else {
        self = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (self) {
        self->windowHandle = hWnd;
        return self->HandleMessage(msg, wp, lp);
    }
    return DefWindowProc(hWnd, msg, wp, lp);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(windowHandle, &ps);
        FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
        if (currentlyLoadedFrame) {
            currentlyLoadedFrame->OnPaint(hdc);
        }
        EndPaint(windowHandle, &ps);
        return 0;
    }

    if (currentlyLoadedFrame) {
        LRESULT res = currentlyLoadedFrame->HandleMessage(this, msg, wp, lp);
        if (res != Frame::NOT_HANDLED) return res;
    }

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(windowHandle, msg, wp, lp);
    }
}

void Window::SetTitle(const std::wstring& t) {
    title = t;
    if (!initialized || !windowHandle) return;
    SetWindowTextW(windowHandle, title.c_str());
}

void Window::SetSize(int w, int h) {
    width = w;
    height = h;
    if (!initialized || !windowHandle) return;

    if (width < 0 || height < 0) {
        SetWindowPos(windowHandle, nullptr,
            CW_USEDEFAULT, CW_USEDEFAULT,
            0, 0,
            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
        return;
    }

    RECT rect = { 0, 0, width, height };
    AdjustWindowRect(&rect, dwStyle, menuResource != 0);
    SetWindowPos(windowHandle, nullptr,
        0, 0,
        rect.right - rect.left,
        rect.bottom - rect.top,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Window::SetStyle(DWORD style) {
    dwStyle = style;
    std::cout << "[DEBUG] Style: " << style << std::endl;
    if (!initialized || !windowHandle) return;
    SetWindowLongPtrW(windowHandle, GWL_STYLE, style);
    SetWindowPos(windowHandle, nullptr,
        0, 0, 0, 0,
        SWP_FRAMECHANGED
        | SWP_NOMOVE
        | SWP_NOSIZE
        | SWP_NOZORDER
        | SWP_SHOWWINDOW
        | SWP_DRAWFRAME);
    Update();
}

void Window::SetMenuResource(int res) {
    menuResource = res;
    if (!initialized || !windowHandle) return;
    if (res > 0) {
        HMENU hMenu = LoadMenu(instance, MAKEINTRESOURCE(res));
        SetMenu(windowHandle, hMenu);
    } else {
        SetMenu(windowHandle, nullptr);
    }
    DrawMenuBar(windowHandle);
}

