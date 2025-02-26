// Window.h : Header file for the Window class definition.
// Author: Jan Köck
// Created: 2025/02/26
//

#pragma once

#include <windows.h>
#define MAX_LOADSTRING 100

class Window {
public:
    Window(HINSTANCE hInstance, 
        WCHAR szWindowClass[MAX_LOADSTRING], 
        WCHAR title[MAX_LOADSTRING], 
        int nCmdShow);
    ~Window();

    bool Init();
    void RunMessageLoop();

private:
    static WNDCLASSEXW CreateWindowClasss(HINSTANCE hInstance, WCHAR className[MAX_LOADSTRING]);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE hInstance; // Handle to the application instance.
    int nCmdShow; // Flag that controls how the window is shown.
    HWND hWnd; // Handle to the window. 
    WCHAR szTitle[MAX_LOADSTRING]; // Title of the window.
    WCHAR szWindowClass[MAX_LOADSTRING]; // Name of the window class.
};
