// car_edger.cpp : Window Class implementation.
// Author: Jan Köck
// Created: 2025/02/26
//

#include "Window.h"
#include "resource.h"
#include "ClassRegisterer.h"

/**
 * @brief Constructor for the Window class. Initializes the window class name, title, and command show flag.
 *
 * @param hInstance Handle to the application instance.
 * @param className Name of the window class.
 * @param title Title of the window.
 * @param nCmdShow Flag that controls how the window is to be shown.
 */
Window::Window(HINSTANCE hInstance, const std::wstring& className, const std::wstring& title, int nCmdShow, DWORD style)
    : hInstance(hInstance), szWindowClass(className), szTitle(title), nCmdShow(nCmdShow), dwStyle(style), hWnd(nullptr) {
}

Window::~Window() {
    if (hWnd) {
        DestroyWindow(hWnd);
    }
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

    hWnd = CreateWindowW(szWindowClass.c_str(), szTitle.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, this);                // Create the window
    if (!hWnd) return false;

    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));                   // Store the Window object in the window's user data

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

WNDCLASSEXW Window::CreateWindowClass(HINSTANCE hInstance, std::wstring className)
{
    WNDCLASSEXW wcex { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = dwStyle;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAREDGER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CAREDGER);                        // Load the menu
    wcex.lpszClassName = szWindowClass.c_str();                                // Set the class name
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));       // Load a small icon

    return wcex;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window* pWindow = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pWindow = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
    } else {
        pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pWindow)  return pWindow->HandleMessage(message, wParam, lParam);
    return DefWindowProc(hWnd, message, wParam, lParam);
}
