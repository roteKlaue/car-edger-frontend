#include "pch.h"
#include "Window.h"
#include <windows.h>
#include "Resource.h"
#include "Frame.h"
#include <iostream>
#include "SubWindow.h"
#include "FrameDefenitions.h"
#include "Component.h"
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

inline static void EnableBlur(Window *window) {
    DWM_BLURBEHIND bb = {};
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = TRUE;
    bb.hRgnBlur = nullptr;
    DwmEnableBlurBehindWindow(window->GetWindowHandle(), &bb);
}

const std::wstring Window::GENERIC_CLASS_NAME = L"GenericWndClass";
bool               Window::classRegistered    = false;

Window::Window(HINSTANCE instance, int nCmdShow)
    : instance(instance), nCmdShow(nCmdShow) {
}

Window::~Window()
{
    Cleanup();
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
    AssertUIThread(); 
    if (!windowHandle) return;
    ShowWindow(windowHandle, nCmdShow);
    Update();
}

void Window::Update() const
{
    AssertUIThread(); 
    InvalidateRect(windowHandle, nullptr, TRUE);
    UpdateWindow(windowHandle);
    DrawMenuBar(windowHandle);
}

void Window::Cleanup()
{
    if (windowHandle) {
        DestroyWindow(windowHandle);
        windowHandle = nullptr;
    }

    for (auto& subWindow : subWindows) {
        if (subWindow) {
            subWindow->Cleanup();
        }
    }

    subWindows.clear();

    if (currentlyLoadedFrame) {
		currentlyLoadedFrame->Unload(this);
		currentlyLoadedFrame.reset();
	}

    initialized = false;
	title = L"";
    menuResource = -1;
	menuButtons.clear();
}

bool Window::Init() {
    RegisterWindowClass();

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect(&rc, dwStyle | LVS_REPORT, menuResource != -1);

    int totalW = rc.right - rc.left;
    int totalH = rc.bottom - rc.top;
    windowHandle = CreateWindowW(
        GENERIC_CLASS_NAME.c_str(), title.c_str(), dwStyle | LVS_REPORT,
        (width < 0 ? CW_USEDEFAULT : 0),
        (height < 0 ? CW_USEDEFAULT : 0),
        (width < 0 ? CW_USEDEFAULT : rc.right - rc.left),
        (height < 0 ? CW_USEDEFAULT : rc.bottom - rc.top),
        nullptr, nullptr, instance, this);

    if (!windowHandle) return false;
    // EnableBlur(this);
    return initialized = true;
}

void Window::RunMessageLoop() const
{
    HACCEL hAccelTable = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_CAREDGER));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::RegisterSubWindow(std::shared_ptr<SubWindow> subWindow) {
	subWindow->SetParent(this);
    subWindows.push_back(std::move(subWindow));
}

void Window::RegisterMenuButton(UINT id, std::function<void(Window *)> func)
{
	if (!windowHandle) {
		std::cerr << "[ERROR] Window handle is null. Cannot register menu button." << std::endl;
		return;
	}
	if (id && func) menuButtons[id] = func;
}

void Window::LoadFrame(std::shared_ptr<Frame> frame, const json& options) {
    AssertUIThread(); 
    if (!frame || (currentlyLoadedFrame && currentlyLoadedFrame->GetUniqueIdentifier() == frame->GetUniqueIdentifier())) {
        return;
    }

    if (currentlyLoadedFrame) {
        currentlyLoadedFrame->Unload(this);
        menuButtons.clear();
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

void Window::LoadInBackground(
    const std::wstring& host,
    const std::wstring& path,
    const std::string& body,
    const std::string& contentType,
    int port,
    Callback onSuccess,
    ErrorCallback onError,
    bool usePost
) {
    AssertUIThread(); 
    auto win = this;
    auto loaderFrame = std::make_shared<LoaderFrame>();
    win->LoadFrame(loaderFrame, {});

    RunOnBackgroundThread([=]() {
        try {
            std::string response;

            if (usePost) {
                response = httpClient->post(host, path, body, contentType, port);
            }
            else {
                response = httpClient->get(host, path, port);
            }

            if (response.empty()) {
                win->PostToUIThread([win, onSuccess]() { onSuccess({}); });
                return;
			}

            auto res = json::parse(response);
            win->PostToUIThread([win, res, onSuccess]() { onSuccess(res); });
        }
        catch (const std::exception& e) {
            std::cerr << "[ERROR] Exception in LoadInBackground: " << e.what() << std::endl;
            if (!onError) return;
            win->PostToUIThread([e, onError]() { onError(e.what()); });
        }
        catch (...) {
            std::cerr << "[ERROR] Unknown exception in LoadInBackground" << std::endl;
            if (!onError) return;
            win->PostToUIThread([onError](){ onError("Unknown error"); });
        }
    });
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

    if (msg == WM_COMMAND) {
		auto id = LOWORD(wp);
		auto it = menuButtons.find(id);
		if (it != menuButtons.end()) {
			it->second(this);
			return 0;
		}
    }

	if (msg == WM_SIZE) {
		width = LOWORD(lp);
		height = HIWORD(lp);
		if (currentlyLoadedFrame) {
			currentlyLoadedFrame->OnParentResize(width, height);
		}
	}

    if (msg == WM_NOTIFY) {
        std::wcout << L"WM_NOTIFY from: " << ((NMHDR*)lp)->idFrom
            << L", code: " << ((NMHDR*)lp)->code << std::endl;
        currentlyLoadedFrame->HandleNotify((NMHDR*)lp);
    }

    if (currentlyLoadedFrame) {
        LRESULT res = currentlyLoadedFrame->HandleMessage(this, msg, wp, lp);
        if (res != Frame::NOT_HANDLED) return res;
    }

    return DefWindowProc(windowHandle, msg, wp, lp);
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

