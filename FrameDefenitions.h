#pragma once

#include "Frame.h"
#include "Resource.h"
#include <iostream>
#include "Menu.h"
#include "InputField.h"
#include "Text.h"
#include "Button.h"
#include "Util.h"
#include "HttpClient.h"

/*
class MainFrame : public Frame {
public:
    MainFrame(HINSTANCE hInstance, int nCmdShow);
protected:
    LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam);
};

class TestFrame : public Frame {
public:
    TestFrame(HINSTANCE hInstance, int nCmdShow)
        : Frame(hInstance, L"TestFrameClass", L"Car Edger Test", nCmdShow) {
        /* Menu* men = new Menu();
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

class TestFrame2 : public Frame {
public:
    TestFrame2(HINSTANCE hInstance, int nCmdShow);

protected:
    LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam);
	void HandleFileMenuPress(UINT id) override;
    std::shared_ptr<Text> text;
    std::shared_ptr<InputField> inputField2;
};*/

class MainFrame : public Frame {

public:
    MainFrame();

    std::string GetUniqueIdentifier() const override {
        return "MainFrame";
    }

    void WakeUp(Window* win, const json& options) override;

    void Unload(Window* win) override {
        Frame::Unload(win);
    }
};


class LoginFrame : public Frame {
private:
	static const HttpClient* httpClient;

public:
    LoginFrame();

    void WakeUp(Window* win, const json& options) override;

    std::string GetUniqueIdentifier() const override {
        return "LoginFrame";
    }

    void Unload(Window* win) override {
        Frame::Unload(win);
    }

protected:
    std::shared_ptr<InputField> passwordField;
    std::shared_ptr<InputField> usernameField;
    std::shared_ptr<Text> loginText;
    std::shared_ptr<Text> usernameLabel;
    std::shared_ptr<Text> passwordLabel;
	std::shared_ptr<Button> loginButton;

    void OnLoginClick();
};
