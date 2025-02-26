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
Window::Window(HINSTANCE hInstance,
    WCHAR className[MAX_LOADSTRING],
    WCHAR title[MAX_LOADSTRING],
    int nCmdShow)
    : hInstance(hInstance), nCmdShow(nCmdShow), hWnd(nullptr) {
    wcscpy_s(szTitle, MAX_LOADSTRING, title);
    wcscpy_s(szWindowClass, MAX_LOADSTRING, className);
}

Window::~Window() {
}

/**
 * @brief Initializes the window by registering the window class, creating the window, and showing it.
 *
 * @return true if initialization was successful, false otherwise.
 */
bool Window::Init() {
    ClassRegisterer* registerer = ClassRegisterer::GetInstance();
	if (!registerer->classExists(szWindowClass, MAX_LOADSTRING)) {
        // auto windowClass = CreateWindowClasss(hInstance, szWindowClass);                           // Create the window class
        // registerer->registerClass(szWindowClass, MAX_LOADSTRING, windowClass); // Register the window class
    }

    auto windowClass = CreateWindowClasss(hInstance, szWindowClass);
    RegisterClassExW(&windowClass);

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);             // Create the window

    if (!hWnd) {
        return false;
    }

    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));                   // Store the Window object in the window's user data

    ShowWindow(hWnd, nCmdShow);                                                                // Show the window
    UpdateWindow(hWnd);                                                                        // Update the window

    return true;
}

void Window::RunMessageLoop() {
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAREDGER));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

WNDCLASSEXW Window::CreateWindowClasss(HINSTANCE hInstance, WCHAR className[MAX_LOADSTRING])
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAREDGER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CAREDGER);
    wcex.lpszClassName = className;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return wcex;
}

INT_PTR CALLBACK About1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDM_ABOUT:
            DialogBoxW(pWindow->hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About1);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    } break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
