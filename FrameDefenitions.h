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
#include "Table.h"

class MainFrame : public Frame {

public:
    MainFrame();

    std::string GetUniqueIdentifier() const override {
        return "MainFrame";
    }

    void WakeUp(Window* win, const json& options) override;
private:
    std::shared_ptr<Table> table;
};

class DrivingFrame : public Frame {

public:
    DrivingFrame();

    std::string GetUniqueIdentifier() const override { return "DrivingFrame"; }

    void WakeUp(Window* win, const json& options) override;
private:
    std::shared_ptr<Table> table;
};

class DriverFrame : public Frame {

public:
    DriverFrame();

    std::string GetUniqueIdentifier() const override { return "DriverFrame"; }

    void WakeUp(Window* win, const json& options) override;
private:
    std::shared_ptr<Table> table;
};

class LoginFrame : public Frame {
private:

public:
    LoginFrame();

    void WakeUp(Window* win, const json& options) override;

    std::string GetUniqueIdentifier() const override {
        return "LoginFrame";
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

class RegisterFrame : public Frame {
private:

public:
    RegisterFrame();

    void WakeUp(Window* win, const json& options) override;

    std::string GetUniqueIdentifier() const override {
        return "RegisterFrame";
    }

protected:
    std::shared_ptr<InputField> passwordField;
    std::shared_ptr<InputField> usernameField;
    std::shared_ptr<Text> registerText;
    std::shared_ptr<Text> usernameLabel;
    std::shared_ptr<Text> passwordLabel;
    std::shared_ptr<Button> registerButton;

    void OnRegisterClick();
};

class LoaderFrame : public Frame {
public:
    std::string GetUniqueIdentifier() const override { return "LoaderFrame"; }

    void WakeUp(Window* win, const json& options) override;
    void Unload(Window* win) override;
    LRESULT HandleMessage(Window* win, UINT msg, WPARAM wp, LPARAM lp) override;
    void OnPaint(HDC hdc) override;

private:
    void UpdateAnimation();
    void DrawLoader(HDC hdc, RECT rect) const;

    float rotation = 0.0f;
    float fillPercent = 0.0f;
    int frameCounter = 0;
    static constexpr int FPS = 45;
    static constexpr int TIMER_ID = 1001;
};
