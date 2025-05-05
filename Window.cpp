// Window.cpp : Window Class implementation.
// Author: Jan Koeck
// Created: 2025/02/26
//

#include "pch.h"
#include "Text.h"
#include "Window.h"
#include "resource.h"
#include "ClassRegisterer.h"
#include <iostream>

const HttpClient* Window::client = new HttpClient();

/**
 * @brief Constructor for the Window class. Initializes the window class name, title, and command show flag.
 *
 * @param hInstance Handle to the application instance.
 * @param className Name of the window class.
 * @param title Title of the window.
 * @param nCmdShow Flag that controls how the window is to be shown.
 */
Window::Window(HINSTANCE hInstance, const std::wstring& className, const std::wstring& title, int nCmdShow, DWORD style, bool stop)
    : hInstance(hInstance), szWindowClass(className), szTitle(title), nCmdShow(nCmdShow), dwStyle(style), hWnd(nullptr), menu(nullptr), stopApplicationOnClose(stop) {
}

Window::~Window() {
    if (!hWnd) return;
    DestroyWindow(hWnd);
}

/**
 * @brief Initializes the window by registering the window class, creating the window, and showing it.
 *
 * @return true if initialization was successful, false otherwise.
 */
bool Window::Init() {
    ClassRegisterer* registerer = ClassRegisterer::GetInstance();
    if (!registerer->classExists(szWindowClass)) {
        auto windowClass = CreateWindowClass(hInstance, szWindowClass);                        // Create the window class
        if (!registerer->registerClass(szWindowClass, windowClass)) return false;              // Register the window class
    }

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect(&rc, dwStyle, menuResource != -1);

    int totalW = rc.right - rc.left;
    int totalH = rc.bottom - rc.top;

    hWnd = CreateWindowW(szWindowClass.c_str(), szTitle.c_str(), 
        dwStyle,
        (width == -1 ? CW_USEDEFAULT : 0),
        (height == -1 ? CW_USEDEFAULT : 0),
        (width == -1 ? CW_USEDEFAULT : totalW),
        (height == -1 ? CW_USEDEFAULT : totalH),
        nullptr, 
        nullptr, 
        hInstance, 
        this);                // Create the window
    if (!hWnd) return false;

    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));                   // Store the Window object in the window's user data

    RegisterComponents();                                                                      // Register the components
    ShowWindow(hWnd, nCmdShow);                                                                // Show the window
    UpdateWindow(hWnd);                                                                        // Update the window

    return true;
}

void Window::RunMessageLoop() const {
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAREDGER));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::RegisterComponents() {
    if (menu) {
        std::cout << menu->GetHandle() << std::endl;
        menu->Register(this, nullptr);
        SetMenu(hWnd, menu->GetHandle());
        DrawMenuBar(hWnd);
    }

    for (auto& pair : components) {
        std::cout << "Component ID: " << pair.first << std::endl;
        if (pair.second) {
            pair.second->SetParent(this);
            pair.second->Create();
        }
    }
}

void Window::HandleCommandEvent(WPARAM wParam, LPARAM lParam)
{
    HandleFileMenuPress(LOWORD(wParam));
    auto it = components.find(LOWORD(wParam));
    if (it != components.end()) {
        it->second->HandleCommand(wParam, lParam);
    }
}

WNDCLASSEXW Window::CreateWindowClass(HINSTANCE hInstance, std::wstring className)
{
    WNDCLASSEXW wcex{ 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAREDGER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass.c_str();                                 // Set the class name
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));        // Load a small icon
    if (menuResource != -1) wcex.lpszMenuName = MAKEINTRESOURCEW(menuResource); // Load the menu

    return wcex;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window* pWindow = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pWindow = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
    }
    else {
        pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

	if (!pWindow) return DefWindowProc(hWnd, message, wParam, lParam);

    if (message == WM_DESTROY && pWindow->stopApplicationOnClose) {
        PostQuitMessage(0);
        return 0;
    }

    if (message == WM_CREATE) {
        // pWindow->RegisterComponents();
        // UpdateWindow(pWindow->GetWindowHandle());
    }

	if (message == WM_COMMAND) {
        pWindow->HandleCommandEvent(wParam, lParam);
	}

    if (message == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		for (auto& pair : pWindow->components) {
			if (!pair.second) {
                continue;
			}

            if (auto derived = std::dynamic_pointer_cast<Text>(pair.second)) {
                if (!derived->GetVisible()) continue;

                std::wstring text = derived->GetText();
				SetBkMode(hdc, TRANSPARENT);

                HFONT hFont = derived->GetFont();
                HFONT hOldFont = nullptr;

                if (hFont) {
                    hOldFont = (HFONT)SelectObject(hdc, hFont);
                }

                TextOut(hdc, derived->GetX(), derived->GetY(), text.c_str(), text.length());

                if (hOldFont) {
                    SelectObject(hdc, hOldFont);
                }
			}
		}

        EndPaint(hWnd, &ps);
        return 0;
    }

    return pWindow->HandleMessage(hWnd, message, wParam, lParam);
}

void Window::Resize(int clientWidth, int clientHeight) {
    width = clientWidth;
    height = clientHeight;

    if (!hWnd) return;

    RECT rc = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&rc, dwStyle, menuResource != -1);

    int totalW = rc.right - rc.left;
    int totalH = rc.bottom - rc.top;

    SetWindowPos(
        hWnd,
        nullptr,
        0, 0,
        totalW, totalH,
        SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
    );
}
