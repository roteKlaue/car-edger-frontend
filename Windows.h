#pragma once

#include "Window.h"
#include "Resource.h"

class MainWindow : public Window {
public:
    MainWindow(HINSTANCE hInstance, int nCmdShow)
        : Window(hInstance, L"DefaultWindowClass", L"Car Edger", nCmdShow) {
        SetMenuResource(IDC_CAREDGER);
    }
protected:
    LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam);
};