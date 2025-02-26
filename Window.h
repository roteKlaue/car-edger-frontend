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

    HINSTANCE hInstance;
    int nCmdShow;
    HWND hWnd;
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
};
