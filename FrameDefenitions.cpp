#include "pch.h"
#include "FrameDefenitions.h"
#include "Text.h"
#include <nlohmann/json.hpp>
#include "Window.h"
#include "SubWindow.h"
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
    auto largeFont = Util::CreatePointFont(16);

    table = std::make_shared<Table>();
    titleText = std::make_shared<Text>(L"All Cars");
    titleText->SetPosition(10, 10);
    titleText->SetFont(largeFont);

    RegisterComponent(table);
    RegisterComponent(titleText);
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
    win->RegisterMenuButton(ID_NEW_DRIVER, openDialog<CreateDriverFrame>());
    win->RegisterMenuButton(ID_NEW_DRIVE, openDialog<CreateDriveFrame>());
    win->RegisterMenuButton(ID_NEW_CAR, openDialog<CreateCarFrame>());
    table->Clear();

    table->AddColumn(L"License Plate", 120);
    table->AddColumn(L"Year", 60);
    table->AddColumn(L"Model", 150);
    table->AddColumn(L"Brand", 150);
    table->AddColumn(L"Fuel Type", 100);
    table->AddColumn(L"Avg Consumption", 150);

    table->SetPosition(0, 50);
    table->Adjust(win->GetWidth(), win->GetHeight());
    win->SetSize(1000, 500);

    if (options.is_array()) {
        for (const auto& vehicle : options) {
            std::wstring licensePlate = vehicle["num_Blade"].is_null() ? L"" : Util::to_utf16(vehicle["num_Blade"].get<std::string>());
            std::wstring year = vehicle["year"].is_null() ? L"" : Util::to_utf16(std::to_string(vehicle["year"].get<int>()));
            std::wstring model = vehicle["model"].is_null() ? L"" : Util::to_utf16(vehicle["model"].get<std::string>());
            std::wstring make = vehicle["make"].is_null() ? L"" : Util::to_utf16(vehicle["make"].get<std::string>());
            std::wstring fuel = vehicle["fuel"].is_null() ? L"" : Util::to_utf16(vehicle["fuel"].get<std::string>());
            std::wstring avgConsumption = vehicle["avg_consumption"].is_null() ? L"" : Util::to_utf16(vehicle["avg_consumption"].get<std::string>());
            table->AddRow({ licensePlate, year, model, make, fuel, avgConsumption });
        }
    }
    else {
        win->LoadInBackground(
            L"localhost",
            L"/car-edgers/vehicles/all",
            {},
            "application/json",
            8080,
            [win](const nlohmann::json& res) {
                auto mainFrame = std::make_shared<MainFrame>();
                win->LoadFrame(mainFrame, res);
            },
            [win](const std::string& err) {
                std::cout << "[ERROR] Loading Vehicles Error: " << err << std::endl;
                MessageBoxA(nullptr, err.c_str(), "Vehicle Loading Error", MB_ICONERROR);
                auto loginFrame = std::make_shared<LoginFrame>();
                win->LoadFrame(loginFrame);
            },
            false
        );
        return;
    }
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

#include "pch.h"
#include "FrameDefenitions.h"
#include "Text.h"
#include <nlohmann/json.hpp>
#include "Window.h"
#include "SubWindow.h"
#include <cmath>

using json = nlohmann::json;

// ... Existing code for other frames ...

DrivingFrame::DrivingFrame()
{
    auto largeFont = Util::CreatePointFont(16);

    table = std::make_shared<Table>();
    titleText = std::make_shared<Text>(L"All Drives");
    titleText->SetPosition(10, 10);
    titleText->SetFont(largeFont);

    RegisterComponent(table);
    RegisterComponent(titleText);
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
    win->RegisterMenuButton(ID_NEW_DRIVER, openDialog<CreateDriverFrame>());
    win->RegisterMenuButton(ID_NEW_DRIVE, openDialog<CreateDriveFrame>());
    win->RegisterMenuButton(ID_NEW_CAR, openDialog<CreateCarFrame>());

    table->Clear();

    table->AddColumn(L"ID", 40);
    table->AddColumn(L"Location Begin", 120);
    table->AddColumn(L"Location End", 120);
    table->AddColumn(L"Date", 100);
    table->AddColumn(L"Time", 80);
    table->AddColumn(L"Km Begin", 90);
    table->AddColumn(L"Km End", 90);
    table->AddColumn(L"Consumption", 100);
    table->AddColumn(L"Vehicle Num Blade", 150);
    table->AddColumn(L"Driver ID", 60);

    if (options.is_array()) {
        int rowId = 1;

        for (int i = 0; i < options.size(); i++) {
            auto drive = options.at(i);
            std::cout << drive.dump(5) << std::endl;

            std::wstring locationBegin = drive["location_begin"].is_null() ? L"" : Util::to_utf16(drive["location_begin"].get<std::string>());
            std::wstring locationEnd = drive["location_end"].is_null() ? L"" : Util::to_utf16(drive["location_end"].get<std::string>());
            std::wstring date = drive["date"].is_null() ? L"" : Util::to_utf16(drive["date"].get<std::string>());
            std::wstring time = drive["time"].is_null() ? L"" : Util::to_utf16(drive["time"].get<std::string>());
            std::wstring kmBegin = drive["km_begin"].is_null() ? L"" : Util::to_utf16(drive["Km_begin"].get<std::string>());
            std::wstring kmEnd = drive["km_end"].is_null() ? L"" : Util::to_utf16(drive["Km_end"].get<std::string>());
            std::wstring consumption;
            if (drive["consumtion"].is_number()) {
                consumption = Util::to_utf16(std::to_string(drive["consumtion"].get<double>()));
            }
            else {
                consumption = L"";
            }

            std::wstring vehicleNumBlade = L"";
            if (drive.contains("vehicle") && drive["vehicle"].is_object() && !drive["vehicle"]["num_blade"].is_null()) {
                vehicleNumBlade = Util::to_utf16(drive["vehicle"]["num_blade"].get<std::string>());
            }

            std::wstring driverId = L"";
            if (drive.contains("driver") && drive["driver"].is_object() && !drive["driver"]["driverId"].is_null()) {
                driverId = Util::to_utf16(std::to_string(drive["driver"]["driverId"].get<int>()));
            }

            table->AddRow({
                Util::to_utf16(std::to_string(rowId++)),
                locationBegin,
                locationEnd,
                date,
                time,
                kmBegin,
                kmEnd,
                consumption,
                vehicleNumBlade,
                driverId
                });
        }

        table->SetOnRowClick([options, win](int indexInverse) {
            int actual = options.size() - 1 - indexInverse;
            auto frame = std::make_shared<DriveDetailFrame>();
            win->LoadFrame(frame, options.at(actual));
            });
    }
    else {
        win->LoadInBackground(
            L"localhost",
            L"/car-edgers/drives/all",
            {},
            "application/json",
            8080,
            [win](const json& res) {
                auto frame = std::make_shared<DrivingFrame>();
                win->LoadFrame(frame, res);
            },
            [win](const std::string& err) {
                std::cout << "[ERROR] Loading Drives Error: " << err << std::endl;
                MessageBoxA(nullptr, err.c_str(), "Drive Loading Error", MB_ICONERROR);
                auto loginFrame = std::make_shared<LoginFrame>();
                win->LoadFrame(loginFrame);
            },
            false
        );
        return;
    }

    table->SetPosition(0, 50);
    table->Adjust(win->GetWidth(), win->GetHeight());
    win->SetSize(1000, 500);
}

DriverFrame::DriverFrame()
{
    auto largeFont = Util::CreatePointFont(16);
    
    table = std::make_shared<Table>();
	titleText = std::make_shared<Text>(L"All Drivers");
	titleText->SetPosition(10, 10);
	titleText->SetFont(largeFont);
    
    RegisterComponent(table);
	RegisterComponent(titleText);
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
    table->Clear();

    table->AddColumn(L"Driver ID", 60);
    table->AddColumn(L"First Name", 150);
    table->AddColumn(L"Last Name", 150);
    table->AddColumn(L"Birthday", 100);

    if (options.is_array()) {
        for (auto& elm : options) {
            std::wstring birthday = L"1990-03-04";
            if (!elm["birthday"].is_null()) {
                auto day = elm["birthday"].get<std::string>();
                birthday = Util::to_utf16(day);
            }
            table->AddRow({
                Util::to_utf16(std::to_string(elm["driverId"].get<int>())),
                Util::to_utf16(elm["firstname"].get<std::string>()),
                Util::to_utf16(elm["lastname"].get<std::string>()),
                birthday
            });
        }
    }
    else {
        win->LoadInBackground(
            L"localhost",
            L"/car-edgers/drivers/all",
            {},
            "application/json",
            8080,
            [win](const json& res) {
                auto main = std::make_shared<DriverFrame>();
                win->LoadFrame(main, res);
            },
            [win](const std::string& err) {
                std::cout << "[ERROR] Registering Error: " << err << std::endl;
                MessageBoxA(nullptr, err.c_str(), "Registering Error", MB_ICONERROR);
                auto retry = std::make_shared<LoginFrame>();
                win->LoadFrame(retry);
            },
            false
        );
        return;
    }

    table->SetPosition(0, 50);
    table->Adjust(win->GetWidth(), win->GetHeight());
    win->SetSize(1000, 500);
}

CreateDriverFrame::CreateDriverFrame() {
    auto largeFont = Util::CreatePointFont(16);
    auto largeXXLFont = Util::CreatePointFont(22);

    titleText = std::make_shared<Text>(L"Create New Driver");
    titleText->SetPosition(30, 20);
    titleText->SetFont(largeXXLFont);

    firstNameLabel = std::make_shared<Text>(L"First Name:");
    firstNameLabel->SetPosition(20, 80);

    firstNameField = std::make_shared<InputField>();
    firstNameField->SetPosition(20, 100);
    firstNameField->SetFont(largeFont);
    firstNameField->SetSize(250, 40);
    firstNameField->SetPlaceholder(L"Driver's first name");

    lastNameLabel = std::make_shared<Text>(L"Last Name:");
    lastNameLabel->SetPosition(20, 150);

    lastNameField = std::make_shared<InputField>();
    lastNameField->SetPosition(20, 170);
    lastNameField->SetFont(largeFont);
    lastNameField->SetSize(250, 40);
    lastNameField->SetPlaceholder(L"Driver's last name");

    //    birthdayLabel = std::make_shared<Text>(L"Birthday (YYYY-MM-DD):");
    //    birthdayLabel->SetPosition(20, 220);
    //    birthdayField = std::make_shared<InputField>();
    //    birthdayField->SetPosition(20, 240);
    //    birthdayField->SetFont(largeFont);
    //    birthdayField->SetSize(250, 40);
    //    birthdayField->SetPlaceholder(L"Driver's birthday");
    createButton = std::make_shared<Button>();
    createButton->SetText(L"Create Driver");
    createButton->SetPosition(50, 230);
    createButton->SetSize(180, 40);
    createButton->SetFont(largeFont);
    createButton->SetOnClick([this]() { OnCreateClick(); });

    RegisterComponent(titleText);
    RegisterComponent(firstNameLabel);
    RegisterComponent(firstNameField);
    RegisterComponent(lastNameLabel);
    RegisterComponent(lastNameField);
//    RegisterComponent(birthdayLabel);
//    RegisterComponent(birthdayField);
    RegisterComponent(createButton);
}

void CreateDriverFrame::WakeUp(Window* win, const json& options) {
    Frame::WakeUp(win, options);
    win->SetTitle(L"Car Edger - Create Driver");
    win->SetStyle(DEFAULT_STYLE_WITHOUT_RESIZE);
    win->SetSize(300, 300);
    win->SetMenuResource(-1);
    firstNameField->SetText(L"");
    lastNameField->SetText(L"");
//    birthdayField->SetText(L"");
    firstNameField->SetFocus();
}

void CreateDriverFrame::OnCreateClick() {
    auto firstName = firstNameField->GetText();
    auto lastName = lastNameField->GetText();
    // auto birthday = birthdayField->GetText();
    json j = {
        {"firstname", Util::to_utf8(firstName)},
        {"lastname", Util::to_utf8(lastName)},
        // {"birthday", Util::to_utf8(birthday)}
    };
    auto win = dynamic_cast<SubWindow*>(this->win);
	win->onCloseCallback = [j](Window* parent) {
        parent->LoadInBackground(
            L"localhost",
            L"/car-edgers/drivers/create",
            j.dump(),
            "application/json",
            8080,
            [parent](const json& res) {
                auto driverFrame = std::make_shared<DriverFrame>();
                parent->LoadFrame(driverFrame, {});
            },
            [parent](const std::string& err) {
                MessageBoxA(nullptr, err.c_str(), "Error", MB_ICONERROR);
            }
        );
	};
    Close(this->win);
}

CreateCarFrame::CreateCarFrame() {
    auto largeFont = Util::CreatePointFont(16);
    auto largeXXLFont = Util::CreatePointFont(22);
    titleText = std::make_shared<Text>(L"Create New Car");
    titleText->SetPosition(40, 20);
    titleText->SetFont(largeXXLFont);

    numBladeLabel = std::make_shared<Text>(L"License Plate:");
    numBladeLabel->SetPosition(20, 80);

    numBladeField = std::make_shared<InputField>();
    numBladeField->SetPosition(20, 100);
    numBladeField->SetFont(largeFont);
    numBladeField->SetSize(250, 40);
    numBladeField->SetPlaceholder(L"Car's number blade");

    yearLabel = std::make_shared<Text>(L"Year:");
    yearLabel->SetPosition(20, 150);

    yearField = std::make_shared<InputField>();
    yearField->SetPosition(20, 170);
    yearField->SetFont(largeFont);
    yearField->SetSize(250, 40);
    yearField->SetPlaceholder(L"Car's year");

    modelLabel = std::make_shared<Text>(L"Model:");
    modelLabel->SetPosition(20, 220);

    modelField = std::make_shared<InputField>();
    modelField->SetPosition(20, 240);
    modelField->SetFont(largeFont);
    modelField->SetSize(250, 40);
    modelField->SetPlaceholder(L"Car's model");

    makeLabel = std::make_shared<Text>(L"Brand:");
    makeLabel->SetPosition(20, 290);

    makeField = std::make_shared<InputField>();
    makeField->SetPosition(20, 310);
    makeField->SetFont(largeFont);
    makeField->SetSize(250, 40);

    fuelLabel = std::make_shared<Text>(L"Fuel Type:");
    fuelLabel->SetPosition(20, 360);

    fuelField = std::make_shared<InputField>();
    fuelField->SetPosition(20, 380);
    fuelField->SetFont(largeFont);
    fuelField->SetSize(250, 40);
    fuelField->SetPlaceholder(L"Car's fuel");

    avgConsumptionLabel = std::make_shared<Text>(L"Average Consumption:");
    avgConsumptionLabel->SetPosition(20, 430);

    avgConsumptionField = std::make_shared<InputField>();
    avgConsumptionField->SetPosition(20, 450);
    avgConsumptionField->SetFont(largeFont);
    avgConsumptionField->SetSize(250, 40);
    avgConsumptionField->SetPlaceholder(L"Car's average consumption");

    createButton = std::make_shared<Button>();
    createButton->SetText(L"Create Car");
    createButton->SetPosition(50, 510);
    createButton->SetSize(180, 40);
    createButton->SetFont(largeFont);
    createButton->SetOnClick([this]() { OnCreateClick(); });

    RegisterComponent(titleText);
    RegisterComponent(numBladeLabel);
    RegisterComponent(numBladeField);
    RegisterComponent(yearLabel);
    RegisterComponent(yearField);
    RegisterComponent(modelLabel);
    RegisterComponent(modelField);
    RegisterComponent(makeLabel);
    RegisterComponent(makeField);
    RegisterComponent(fuelLabel);
    RegisterComponent(fuelField);
    RegisterComponent(avgConsumptionLabel);
    RegisterComponent(avgConsumptionField);
    RegisterComponent(createButton);
}

void CreateCarFrame::WakeUp(Window* win, const json& options) {
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - Create Car");
    win->SetStyle(DEFAULT_STYLE_WITHOUT_RESIZE);
    win->SetSize(290, 570);
    win->SetMenuResource(-1);

    avgConsumptionField->SetText(L"");
    numBladeField->SetText(L"");
    modelField->SetText(L"");
    yearField->SetText(L"");
    makeField->SetText(L"");
    fuelField->SetText(L"");

    numBladeField->SetFocus();
}

void CreateCarFrame::OnCreateClick() {
    auto avgConsumption = avgConsumptionField->GetText();
    auto numBlade = numBladeField->GetText();
    auto model = modelField->GetText();
    auto year = yearField->GetText();
    auto make = makeField->GetText();
    auto fuel = fuelField->GetText();

    json j = {
        { "num_Blade", Util::to_utf8(numBlade) },
        { "year", Util::to_utf8(year) },
        { "Model", Util::to_utf8(model) },
        { "make", Util::to_utf8(make) },
        { "fuel", Util::to_utf8(fuel) },
        { "avg_consumption", Util::to_utf8(avgConsumption) }
    };

    auto win = dynamic_cast<SubWindow*>(this->win);
    win->onCloseCallback = [j](Window* parent) {
        parent->LoadInBackground(
            L"localhost",
            L"/car-edgers/vehicles/create",
            j.dump(),
            "application/json",
            8080,
            [parent](const json& res) {
                auto mainFrame = std::make_shared<MainFrame>();
                parent->LoadFrame(mainFrame, {});
            },
            [parent](const std::string& err) {
                MessageBoxA(nullptr, err.c_str(), "Error", MB_ICONERROR);
            }
        );
    };
    Close(this->win);
}

CreateDriveFrame::CreateDriveFrame() {
    auto largeFont = Util::CreatePointFont(16);
    auto largeXXLFont = Util::CreatePointFont(22);

    titleText = std::make_shared<Text>(L"Create New Drive");
    titleText->SetPosition(30, 20);
    titleText->SetFont(largeXXLFont);

    locationBeginLabel = std::make_shared<Text>(L"Location Begin:");
    locationBeginLabel->SetPosition(20, 80);

    locationBeginField = std::make_shared<InputField>();
    locationBeginField->SetPosition(20, 100);
    locationBeginField->SetFont(largeFont);
    locationBeginField->SetSize(250, 40);
    locationBeginField->SetPlaceholder(L"Start location");

    locationEndLabel = std::make_shared<Text>(L"Location End:");
    locationEndLabel->SetPosition(20, 150);

    locationEndField = std::make_shared<InputField>();
    locationEndField->SetPosition(20, 170);
    locationEndField->SetFont(largeFont);
    locationEndField->SetSize(250, 40);
    locationEndField->SetPlaceholder(L"End location");

    dateLabel = std::make_shared<Text>(L"Date (YYYY-MM-DD):");
    dateLabel->SetPosition(20, 220);

    dateField = std::make_shared<InputField>();
    dateField->SetPosition(20, 240);
    dateField->SetFont(largeFont);
    dateField->SetSize(250, 40);
    dateField->SetPlaceholder(L"Date");

    timeLabel = std::make_shared<Text>(L"Time (HH:MM):");
    timeLabel->SetPosition(20, 290);

    timeField = std::make_shared<InputField>();
    timeField->SetPosition(20, 310);
    timeField->SetFont(largeFont);
    timeField->SetSize(250, 40);
    timeField->SetPlaceholder(L"Time");

    kmBeginLabel = std::make_shared<Text>(L"Km Begin:");
    kmBeginLabel->SetPosition(20, 360);

    kmBeginField = std::make_shared<InputField>();
    kmBeginField->SetPosition(20, 380);
    kmBeginField->SetFont(largeFont);
    kmBeginField->SetSize(250, 40);
    kmBeginField->SetPlaceholder(L"Kilometers at start");

    kmEndLabel = std::make_shared<Text>(L"Km End:");
    kmEndLabel->SetPosition(20, 440);

    kmEndField = std::make_shared<InputField>();
    kmEndField->SetPosition(20, 460);
    kmEndField->SetFont(largeFont);
    kmEndField->SetSize(250, 40);
    kmEndField->SetPlaceholder(L"Kilometers at end");

    consumptionLabel = std::make_shared<Text>(L"Consumption (L/100km):");
    consumptionLabel->SetPosition(20, 520);

    consumptionField = std::make_shared<InputField>();
    consumptionField->SetPosition(20, 540);
    consumptionField->SetFont(largeFont);
    consumptionField->SetSize(250, 40);
    consumptionField->SetPlaceholder(L"Fuel consumption");

    carLabel = std::make_shared<Text>(L"Car (license plate):");
    carLabel->SetPosition(20, 600);

    carField = std::make_shared<InputField>();
    carField->SetPosition(20, 620);
    carField->SetFont(largeFont);
    carField->SetSize(250, 40);
    carField->SetPlaceholder(L"Car license plate");

    driverLabel = std::make_shared<Text>(L"Driver (driver id):");
    driverLabel->SetPosition(20, 680);

    driverField = std::make_shared<InputField>();
    driverField->SetPosition(20, 700);
    driverField->SetFont(largeFont);
    driverField->SetSize(250, 40);
    driverField->SetPlaceholder(L"Driver (driver id)");

    createButton = std::make_shared<Button>();
    createButton->SetText(L"Create Drive");
    createButton->SetPosition(50, 750);
    createButton->SetSize(180, 40);
    createButton->SetFont(largeFont);
    createButton->SetOnClick([this]() { OnCreateClick(); });

    RegisterComponent(titleText);
    RegisterComponent(locationBeginLabel);
    RegisterComponent(locationBeginField);
    RegisterComponent(locationEndLabel);
    RegisterComponent(locationEndField);
    RegisterComponent(dateLabel);
    RegisterComponent(dateField);
    RegisterComponent(timeLabel);
    RegisterComponent(timeField);
    RegisterComponent(kmBeginLabel);
    RegisterComponent(kmBeginField);
    RegisterComponent(kmEndLabel);
    RegisterComponent(kmEndField);
    RegisterComponent(consumptionLabel);
    RegisterComponent(consumptionField);
    RegisterComponent(carLabel);
    RegisterComponent(carField);
    RegisterComponent(driverLabel);
    RegisterComponent(driverField);
    RegisterComponent(createButton);
}

void CreateDriveFrame::WakeUp(Window* win, const json& options) {
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - Create Drive");
    win->SetStyle(DEFAULT_STYLE_WITHOUT_RESIZE);
    win->SetSize(290, 800);
    win->SetMenuResource(-1);

    locationBeginField->SetText(L"");
    locationEndField->SetText(L"");
    dateField->SetText(L"");
    timeField->SetText(L"");
    kmBeginField->SetText(L"");
    kmEndField->SetText(L"");
    consumptionField->SetText(L"");

    locationBeginField->SetFocus();
}

void CreateDriveFrame::OnCreateClick() {
    auto locationBegin = locationBeginField->GetText();
    auto locationEnd = locationEndField->GetText();
    auto date = dateField->GetText();
    auto time = timeField->GetText();
    auto kmBegin = kmBeginField->GetText();
    auto kmEnd = kmEndField->GetText();
    auto consumption = consumptionField->GetText();
    auto car = carField->GetText();
    auto driver = driverField->GetText();
    auto driverNum = Util::WStringToNumber(driver);
    auto consumptionNum = Util::WStringToNumber(consumption);

    json j = {
        { "location_begin", Util::to_utf8(locationBegin) },
        { "location_end", Util::to_utf8(locationEnd) },
        { "date", Util::to_utf8(date) },
        { "time", Util::to_utf8(time) },
        { "km_begin", Util::to_utf8(kmBegin) },
        { "km_end", Util::to_utf8(kmEnd) },
        { "consumtion", consumptionNum },
        { "vehicle", { { "num_blade", Util::to_utf8(car) } } },
        { "driver", { { "driverId", driverNum } } }
    };

    std::cout << j.dump(5) << std::endl;

    auto win = dynamic_cast<SubWindow*>(this->win);
    win->onCloseCallback = [j](Window* parent) {
        parent->LoadInBackground(
            L"localhost",
            L"/car-edgers/drives/create",
            j.dump(),
            "application/json",
            8080,
            [parent](const json& res) {
                auto drivingFrame = std::make_shared<DrivingFrame>();
                parent->LoadFrame(drivingFrame, {});
            },
            [parent](const std::string& err) {
                MessageBoxA(nullptr, err.c_str(), "Error", MB_ICONERROR);
            }
        );
        };
    Close(this->win);
}

DriveDetailFrame::DriveDetailFrame() {
    auto largeFont = Util::CreatePointFont(16);
    auto titleFont = Util::CreatePointFont(22);

    titleText = std::make_shared<Text>(L"Drive Details");
    titleText->SetPosition(30, 20);
    titleText->SetFont(titleFont);

    locationBeginText = std::make_shared<Text>();
    locationBeginText->SetPosition(20, 70);
    locationBeginText->SetFont(largeFont);

    locationEndText = std::make_shared<Text>();
    locationEndText->SetPosition(20, 110);
    locationEndText->SetFont(largeFont);

    dateText = std::make_shared<Text>();
    dateText->SetPosition(20, 150);
    dateText->SetFont(largeFont);

    timeText = std::make_shared<Text>();
    timeText->SetPosition(20, 190);
    timeText->SetFont(largeFont);

    kmBeginText = std::make_shared<Text>();
    kmBeginText->SetPosition(20, 230);
    kmBeginText->SetFont(largeFont);

    kmEndText = std::make_shared<Text>();
    kmEndText->SetPosition(20, 270);
    kmEndText->SetFont(largeFont);

    consumptionText = std::make_shared<Text>();
    consumptionText->SetPosition(20, 310);
    consumptionText->SetFont(largeFont);

    vehicleNumBladeText = std::make_shared<Text>();
    vehicleNumBladeText->SetPosition(20, 350);
    vehicleNumBladeText->SetFont(largeFont);

    driverIdText = std::make_shared<Text>();
    driverIdText->SetPosition(20, 390);
    driverIdText->SetFont(largeFont);

    RegisterComponent(titleText);
    RegisterComponent(locationBeginText);
    RegisterComponent(locationEndText);
    RegisterComponent(dateText);
    RegisterComponent(timeText);
    RegisterComponent(kmBeginText);
    RegisterComponent(kmEndText);
    RegisterComponent(consumptionText);
    RegisterComponent(vehicleNumBladeText);
    RegisterComponent(driverIdText);
}

void DriveDetailFrame::WakeUp(Window* win, const json& options) {
    Frame::WakeUp(win, options);
    win->SetTitle(L"Car Edger - Drive Details");
    win->SetStyle(DEFAULT_STYLE_WITHOUT_RESIZE);
    win->SetSize(400, 450);
    win->SetMenuResource(-1);

    std::wstring locationBegin = L"N/A";
    std::wstring locationEnd = L"N/A";
    std::wstring date = L"N/A";
    std::wstring time = L"N/A";
    std::wstring kmBegin = L"N/A";
    std::wstring kmEnd = L"N/A";
    std::wstring consumption = L"N/A";
    std::wstring vehicleNumBlade = L"N/A";
    std::wstring driverId = L"N/A";

    if (options.is_object()) {
        if (!options["location_begin"].is_null())
            locationBegin = Util::to_utf16(options["location_begin"].get<std::string>());
        if (!options["location_end"].is_null())
            locationEnd = Util::to_utf16(options["location_end"].get<std::string>());
        if (!options["date"].is_null())
            date = Util::to_utf16(options["date"].get<std::string>());
        if (!options["time"].is_null())
            time = Util::to_utf16(options["time"].get<std::string>());
        if (!options["Km_begin"].is_null())
            kmBegin = Util::to_utf16(options["Km_begin"].get<std::string>());
        if (!options["Km_end"].is_null())
            kmEnd = Util::to_utf16(options["Km_end"].get<std::string>());
        if (options["consumtion"].is_number()) {
            consumption = Util::to_utf16(std::to_string(options["consumtion"].get<double>()));
        }
        if (options.contains("vehicle") 
            && options["vehicle"].is_object() 
            && !options["vehicle"]["num_blade"].is_null()) {
            vehicleNumBlade = Util::to_utf16(options["vehicle"]["num_blade"].get<std::string>());
        }
        if (options.contains("driver") 
            && options["driver"].is_object() 
            && !options["driver"]["driverId"].is_null()) {
            driverId = Util::to_utf16(std::to_string(options["driver"]["driverId"].get<int>()));
        }
    }

    locationBeginText->SetText(L"Location Begin: " + locationBegin);
    locationEndText->SetText(L"Location End: " + locationEnd);
    dateText->SetText(L"Date: " + date);
    timeText->SetText(L"Time: " + time);
    kmBeginText->SetText(L"Km Begin: " + kmBegin);
    kmEndText->SetText(L"Km End: " + kmEnd);
    consumptionText->SetText(L"Consumption: " + consumption);
    vehicleNumBladeText->SetText(L"Vehicle Number Plate: " + vehicleNumBlade);
    driverIdText->SetText(L"Driver ID: " + driverId);
}
