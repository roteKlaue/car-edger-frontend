// Window.h : Header file for the Window class definition.
// Author: Jan Köck
// Created: 2025/02/26
//

#pragma once

#include <windows.h>
#include <string>
#define MAX_LOADSTRING 100

class Window {
public:
    Window(HINSTANCE hInstance, 
        const std::wstring& className, 
        const std::wstring& title, 
        int nCmdShow, 
        DWORD style = WS_OVERLAPPEDWINDOW);
    virtual ~Window();

    bool Init();
    void RunMessageLoop() const;
    HWND GetHandle() const { return hWnd; }

protected:
    virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:
    WNDCLASSEXW CreateWindowClass(HINSTANCE hInstance, std::wstring className);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE hInstance; // Handle to the application instance.
    int nCmdShow; // Flag that controls how the window is shown.
    HWND hWnd; // Handle to the window. 
    std::wstring szTitle;
    std::wstring szWindowClass;
    DWORD dwStyle;
};
