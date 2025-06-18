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
    std::shared_ptr<Text> titleText;
    std::shared_ptr<Table> table;
};

class DrivingFrame : public Frame {

public:
    DrivingFrame();

    std::string GetUniqueIdentifier() const override { return "DrivingFrame"; }

    void WakeUp(Window* win, const json& options) override;
private:
    std::shared_ptr<Text> titleText;
    std::shared_ptr<Table> table;
};

class DriverFrame : public Frame {

public:
    DriverFrame();

    std::string GetUniqueIdentifier() const override { return "DriverFrame"; }

    void WakeUp(Window* win, const json& options) override;
private:
    std::shared_ptr<Table> table;
	std::shared_ptr<Text> titleText;
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

class CreateCarFrame : public Frame {
public:
	CreateCarFrame();
    std::string GetUniqueIdentifier() const override {
        return "CreateCarFrame";
    }
	void WakeUp(Window* win, const json& options) override;
private:
    void OnCreateClick();

    std::shared_ptr<Button> createButton;

    std::shared_ptr<Text> titleText;
    std::shared_ptr<Text> makeLabel;
    std::shared_ptr<Text> yearLabel;
    std::shared_ptr<Text> fuelLabel;
    std::shared_ptr<Text> modelLabel;
    std::shared_ptr<Text> numBladeLabel;
    std::shared_ptr<Text> avgConsumptionLabel;

    std::shared_ptr<InputField> makeField;
    std::shared_ptr<InputField> yearField;
    std::shared_ptr<InputField> fuelField;
    std::shared_ptr<InputField> modelField;
    std::shared_ptr<InputField> numBladeField;
    std::shared_ptr<InputField> avgConsumptionField;
};

class CreateDriveFrame : public Frame {
public:
    using Frame::Frame;
    std::string GetUniqueIdentifier() const override {
        return "CreateDriveFrame";
    }
};

class CreateDriverFrame : public Frame {
public:
    CreateDriverFrame();
    std::string GetUniqueIdentifier() const override {
        return "CreateDriverFrame";
    }
    void WakeUp(Window* win, const json& options) override;
private:
    std::shared_ptr<InputField> firstNameField;
    std::shared_ptr<InputField> lastNameField;
    // std::shared_ptr<InputField> birthdayField;
    std::shared_ptr<Text> titleText;
    std::shared_ptr<Text> firstNameLabel;
    std::shared_ptr<Text> lastNameLabel;
    // std::shared_ptr<Text> birthdayLabel;
    std::shared_ptr<Button> createButton;
    void OnCreateClick();
};
