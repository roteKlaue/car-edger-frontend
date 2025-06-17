#include "pch.h"
#include "FrameDefenitions.h"
#include "Text.h"
#include <nlohmann/json.hpp>
#include "Window.h"
#include <cmath>

using json = nlohmann::json;

static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

static void Close(Window* win) {
	SendMessage(win->GetWindowHandle(), WM_CLOSE, 0, 0);
}

static void OpenAbout(Window* win) {
	DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUTBOX), win->GetWindowHandle(), About);
}

template <typename FrameType>
concept DerivedFromFrame = std::is_base_of_v<Frame, FrameType>;

template <DerivedFromFrame FrameType>

static auto openFrame = []() {
    return [](Window* win) {
        auto frame = std::make_shared<FrameType>();
        return win->LoadFrame(frame);
    };
};

template <typename FrameType1>
concept DerivedFromFrame1 = std::is_base_of_v<Frame, FrameType1>;

template <DerivedFromFrame1 FrameType1>

static auto openDialog = []() {
    return [](Window* win) {
        auto frame = std::make_shared<FrameType1>();
        return win->ShowDialog(frame);
    };
};

const static auto DEFAULT_STYLE_WITHOUT_RESIZE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

LoginFrame::LoginFrame() {
    auto largeFont = Util::CreatePointFont(16);
    auto largeXXLFont = Util::CreatePointFont(22);

    usernameField = std::make_shared<InputField>();
    usernameField->SetPlaceholder(L"Username");
    usernameField->SetPosition(20, 100);
    usernameField->SetFont(largeFont);
    usernameField->SetSize(220, 40);

    passwordField = std::make_shared<InputField>();
    passwordField->SetPlaceholder(L"Password");
    passwordField->SetType(InputFieldType::PASSWORD);
    passwordField->SetPosition(20, 200);
    passwordField->SetFont(largeFont);
    passwordField->SetSize(220, 40);

    loginText = std::make_shared<Text>();
    loginText->SetText(L"Login to Car Edger");
    loginText->SetPosition(10, 20);
    loginText->SetFont(largeXXLFont);

    usernameLabel = std::make_shared<Text>();
    usernameLabel->SetPosition(20, 80);
    usernameLabel->SetText(L"Username:");

    passwordLabel = std::make_shared<Text>();
    passwordLabel->SetPosition(20, 180);
    passwordLabel->SetText(L"Password:");

    loginButton = std::make_shared<Button>();
    loginButton->SetText(L"Login");
    loginButton->SetPosition(50, 260);
    loginButton->SetSize(150, 40);
    loginButton->SetFont(largeFont);
    loginButton->SetOnClick([this]() { OnLoginClick(); });

    RegisterComponent(loginText);
    RegisterComponent(usernameLabel);
    RegisterComponent(passwordLabel);
    RegisterComponent(passwordField);
    RegisterComponent(usernameField);
    RegisterComponent(loginButton);
}

void LoginFrame::OnLoginClick()
{
    auto u = usernameField->GetText();
    auto p = passwordField->GetText();
    json j = {
        {"username", Util::to_utf8(u)},
        {"password", Util::to_utf8(p)}
    };

    auto win = this->win;

    win->LoadInBackground(
        L"localhost",
        L"/car-edgers/auth/login",
        j.dump(),
        "application/json",
        8080,
        [win](const json& res) {
            if (res.value("success", false)) {
                auto main = std::make_shared<MainFrame>();
                win->LoadFrame(main, res);
            } else {
                auto retry = std::make_shared<LoginFrame>();
                win->LoadFrame(retry, res);
            }
        },
        [win](const std::string& err) {
            std::cout << "[ERROR] Login Error: " << err << std::endl;
            MessageBoxA(nullptr, err.c_str(), "Login Error", MB_ICONERROR);
            auto retry = std::make_shared<LoginFrame>();
            win->LoadFrame(retry);
        }
    );
}

void LoginFrame::WakeUp(Window* win, const json& options)
{
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - Login");
    win->SetStyle(DEFAULT_STYLE_WITHOUT_RESIZE);
    win->SetSize(260, 350);
    win->SetMenuResource(IDC_LOGIN_MENU);

    win->RegisterMenuButton(ID_LOGIN_FILE_EXIT, Close);
	win->RegisterMenuButton(ID_LOGIN_HELP_ABOUT, OpenAbout);
	win->RegisterMenuButton(ID_LOGIN_FILE_REGISTER, openFrame<RegisterFrame>());

    usernameField->SetText(L"");
	passwordField->SetText(L"");
	usernameField->SetFocus();
}

MainFrame::MainFrame() : Frame() {
    table = std::make_shared<Table>();
    RegisterComponent(table);
}

void MainFrame::WakeUp(Window* win, const json& options)
{
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - Mainpage");
    win->SetStyle(WS_OVERLAPPEDWINDOW);
	win->SetMenuResource(IDC_MAIN_WINDOW_MENU);

    win->RegisterMenuButton(ID_WINDOW_EXIT, Close);
    win->RegisterMenuButton(ID_HELP_ABOUT, OpenAbout);
    win->RegisterMenuButton(ID_WINDOW_LOGOUT, openFrame<LoginFrame>());
    win->RegisterMenuButton(ID_VIEWALLDRIVES, openFrame<DrivingFrame>());
    win->RegisterMenuButton(ID_VIEWALLDRIVERS, openFrame<DriverFrame>());

    table->AddColumn(L"ID", 40);
    table->AddColumn(L"Username", 150);
    table->AddColumn(L"Email", 150);

    table->AddRow({ L"1", L"matthias", L"matthias@example.com" });
    table->SetPosition(0, 50);
    table->Adjust(win->GetWidth(), win->GetHeight());
    win->SetSize(1000, 500);
}

RegisterFrame::RegisterFrame()
{
    auto largeFont = Util::CreatePointFont(16);
    auto largeXXLFont = Util::CreatePointFont(22);

    usernameField = std::make_shared<InputField>();
    usernameField->SetPlaceholder(L"Username");
    usernameField->SetPosition(20, 100);
    usernameField->SetFont(largeFont);
    usernameField->SetSize(250, 40);

    passwordField = std::make_shared<InputField>();
    passwordField->SetPlaceholder(L"Password");
    passwordField->SetType(InputFieldType::PASSWORD);
    passwordField->SetPosition(20, 200);
    passwordField->SetFont(largeFont);
    passwordField->SetSize(250, 40);

    registerText = std::make_shared<Text>(L"Register to Car Edger");
    registerText->SetPosition(10, 20);
    registerText->SetFont(largeXXLFont);

    usernameLabel = std::make_shared<Text>(L"Username:");
    usernameLabel->SetPosition(20, 80);

    passwordLabel = std::make_shared<Text>(L"Password:");
    passwordLabel->SetPosition(20, 180);

    registerButton = std::make_shared<Button>();
    registerButton->SetText(L"Register");
    registerButton->SetPosition(50, 260);
    registerButton->SetSize(180, 40);
    registerButton->SetFont(largeFont);
    registerButton->SetOnClick([this]() { OnRegisterClick(); });

    RegisterComponent(registerText);
    RegisterComponent(usernameLabel);
    RegisterComponent(passwordLabel);
    RegisterComponent(passwordField);
    RegisterComponent(usernameField);
    RegisterComponent(registerButton);
}

void RegisterFrame::WakeUp(Window* win, const json& options)
{
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - Register");
    win->SetStyle(DEFAULT_STYLE_WITHOUT_RESIZE);
    win->SetSize(290, 350);
    win->SetMenuResource(IDC_REGISTER_MENU);

    win->RegisterMenuButton(ID_REGISTER_EXIT, Close);
    win->RegisterMenuButton(ID_REGISTER_HELP, OpenAbout);
    win->RegisterMenuButton(ID_REGISTER_LOGIN, openFrame<LoginFrame>());

    usernameField->SetText(L"");
    passwordField->SetText(L"");
    usernameField->SetFocus();
}

void RegisterFrame::OnRegisterClick()
{
    auto u = usernameField->GetText();
    auto p = passwordField->GetText();
    json j = {
        { "username", Util::to_utf8(u) },
        { "password", Util::to_utf8(p) }
    };

    auto win = this->win;

    win->LoadInBackground(
        L"localhost",
        L"/car-edgers/auth/register",
        j.dump(),
        "application/json",
        8080,
        [win](const json& res) {
            if (res.value("success", false)) {
                auto main = std::make_shared<LoginFrame>();
                win->LoadFrame(main, res);
            }
            else {
                auto retry = std::make_shared<RegisterFrame>();
                win->LoadFrame(retry, res);
            }
        },
        [win](const std::string& err) {
            std::cout << "[ERROR] Registering Error: " << err << std::endl;
            MessageBoxA(nullptr, err.c_str(), "Registering Error", MB_ICONERROR);
            auto retry = std::make_shared<RegisterFrame>();
            win->LoadFrame(retry);
        }
    );
}

void LoaderFrame::WakeUp(Window* win, const json& options) {
    Frame::WakeUp(win, options);
    win->SetTitle(L"Car Edger - Loading");
    win->SetStyle(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	win->SetSize(300, 300);
    win->SetMenuResource(-1);
    SetTimer(GetWindowHandle(), TIMER_ID, 1000 / FPS, nullptr);
}

void LoaderFrame::Unload(Window* win) {
    KillTimer(GetWindowHandle(), TIMER_ID);
    Frame::Unload(win);
}

LRESULT LoaderFrame::HandleMessage(Window* win, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_ERASEBKGND) return 1;
    if (msg == WM_TIMER && wp == TIMER_ID) {
        UpdateAnimation();
        InvalidateRect(GetWindowHandle(), nullptr, TRUE);
        return 1;
    }
    return Frame::HandleMessage(win, msg, wp, lp);
}

void LoaderFrame::UpdateAnimation() {
    rotation += 3.0f;
    if (rotation >= 360.0f) rotation -= 360.0f;

    frameCounter = (frameCounter + 1) % (FPS * 2);

    if (frameCounter < FPS / 2) fillPercent = 0.0f;
    else if (frameCounter < FPS) fillPercent = (frameCounter - FPS / 2) / float(FPS / 2);
    else if (frameCounter < FPS + FPS / 2) fillPercent = 1.0f;
    else fillPercent = 1.0f - ((frameCounter - (FPS + FPS / 2)) / float(FPS / 2));
}

void LoaderFrame::OnPaint(HDC hdc) {
    RECT rect;
    GetClientRect(GetWindowHandle(), &rect);
    FillRect(hdc, &rect, CreateSolidBrush(RGB(36, 47, 63)));
    DrawLoader(hdc, rect);
    Frame::OnPaint(hdc);
}

void LoaderFrame::DrawLoader(HDC hdc, RECT rect) const {
    int cx = (rect.right - rect.left) / 2;
    int cy = (rect.bottom - rect.top) / 2;
    int size = 60;
    int half = size / 2 + 4;

    XFORM xForm;
    SetGraphicsMode(hdc, GM_ADVANCED);
    GetWorldTransform(hdc, &xForm);
    XFORM oldXForm = xForm;

    float angle = rotation * 3.14159f / 180.0f;
    xForm.eM11 = cos(angle);
    xForm.eM12 = sin(angle);
    xForm.eM21 = -sin(angle);
    xForm.eM22 = cos(angle);
    xForm.eDx = (FLOAT)cx;
    xForm.eDy = (FLOAT)cy;

    SetWorldTransform(hdc, &xForm);

    RECT borderRect = { -half, -half, half, half };
    FrameRect(hdc, &borderRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    int fillHeight = (int)(size * fillPercent);
    RECT fillRect = { -half + 4, half - fillHeight - 4, half - 4, half - 4 };
    FillRect(hdc, &fillRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    SetWorldTransform(hdc, &oldXForm);
}

DrivingFrame::DrivingFrame()
{
    table = std::make_shared<Table>();
    RegisterComponent(table);
}

void DrivingFrame::WakeUp(Window* win, const json& options)
{
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - All Drives");
    win->SetStyle(WS_OVERLAPPEDWINDOW);
    win->SetMenuResource(IDC_MAIN_WINDOW_MENU);

    win->RegisterMenuButton(ID_WINDOW_EXIT, Close);
    win->RegisterMenuButton(ID_HELP_ABOUT, OpenAbout);
    win->RegisterMenuButton(ID_VIEWALLDRIVERS, openFrame<DriverFrame>());
    win->RegisterMenuButton(ID_WINDOW_LOGOUT, openFrame<LoginFrame>());
    win->RegisterMenuButton(ID_VIEWALLCARS, openFrame<MainFrame>());

    table->AddColumn(L"ID", 40);
    table->AddColumn(L"Username", 150);
    table->AddColumn(L"Email", 150);

    table->AddRow({ L"1", L"matthias", L"matthias@example.com" });
    table->SetPosition(0, 50);
    table->Adjust(win->GetWidth(), win->GetHeight());
    win->SetSize(1000, 500);
}

DriverFrame::DriverFrame()
{
    table = std::make_shared<Table>();
    RegisterComponent(table);
}

void DriverFrame::WakeUp(Window* win, const json& options)
{
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - All Drivers");
    win->SetStyle(WS_OVERLAPPEDWINDOW);
    win->SetMenuResource(IDC_MAIN_WINDOW_MENU);

    win->RegisterMenuButton(ID_WINDOW_EXIT, Close);
    win->RegisterMenuButton(ID_HELP_ABOUT, OpenAbout);
    win->RegisterMenuButton(ID_VIEWALLDRIVES, openFrame<DrivingFrame>());
    win->RegisterMenuButton(ID_WINDOW_LOGOUT, openFrame<LoginFrame>());
    win->RegisterMenuButton(ID_VIEWALLCARS, openFrame<MainFrame>());
    win->RegisterMenuButton(ID_NEW_DRIVER, openDialog<CreateDriverFrame>());
    win->RegisterMenuButton(ID_NEW_DRIVE, openDialog<CreateDriveFrame>());
    win->RegisterMenuButton(ID_NEW_CAR, openDialog<CreateCarFrame>());

    table->AddColumn(L"ID", 40);
    table->AddColumn(L"Username", 150);
    table->AddColumn(L"Email", 150);

    table->AddRow({ L"1", L"matthias", L"matthias@example.com" });
    table->SetPosition(0, 50);
    table->Adjust(win->GetWidth(), win->GetHeight());
    win->SetSize(1000, 500);
}
