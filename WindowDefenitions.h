#pragma once

#include "Window.h"
#include "Resource.h"
#include <iostream>
#include "Menu.h"
#include "InputField.h"
#include "Text.h"
    
class MainWindow : public Window {
public:
    MainWindow(HINSTANCE hInstance, int nCmdShow);
protected:
    LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam);
};

class TestWindow : public Window {
public:
    TestWindow(HINSTANCE hInstance, int nCmdShow)
        : Window(hInstance, L"TestWindowClass", L"Car Edger Test", nCmdShow) {
        Menu* men = new Menu();
        Menu submen(MenuType::PopupMenu);
        Option open(L"&Open");
        Option exit(L"&Exit");

        submen.SetLabel(L"File");
        submen.AddOption(&open);
        submen.AddOption(&exit);

        men->AddSubMenu(&submen);
        // RegisterMenu(men);
    }

protected:
    LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam);
};

class TestWindow2 : public Window {
public:
    TestWindow2(HINSTANCE hInstance, int nCmdShow);

protected:
    LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam);
    std::shared_ptr<Text> text;
    std::shared_ptr<InputField> inputField2;
};


class LoginWindow : public Window {
public:
    LoginWindow(HINSTANCE hInstance, int nCmdShow)
        : Window(hInstance, L"LoginWindowClass", L"Car Edger Login", nCmdShow) {
        SetMenuResource(IDC_MAIN_WINDOW_MENU);
    }

protected:
    LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam);
};
