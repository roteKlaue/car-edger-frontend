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
    table->AddColumn(L"Make", 150);
    table->AddColumn(L"Fuel", 100);
    table->AddColumn(L"Avg Consumption", 150);

    table->SetPosition(0, 50);
    table->Adjust(win->GetWidth(), win->GetHeight());
    win->SetSize(1000, 500);

    if (options.is_array()) {
        for (const auto& vehicle : options) {
            std::wstring licensePlate = L"";
            if (!vehicle["num_Blade"].is_null()) {
                licensePlate = Util::to_utf16(vehicle["num_Blade"].get<std::string>());
            }
            std::wstring year = vehicle["year"].is_null() ? L"" : Util::to_utf16(std::to_string(vehicle["year"].get<int>()));
            std::wstring model = vehicle["Model"].is_null() ? L"" : Util::to_utf16(vehicle["Model"].get<std::string>());
            std::wstring make = vehicle["make"].is_null() ? L"" : Util::to_utf16(vehicle["make"].get<std::string>());
            std::wstring fuel = vehicle["fuel"].is_null() ? L"" : Util::to_utf16(vehicle["fuel"].get<std::string>());
            std::wstring avgConsumption = vehicle["avg_consumption"].is_null() ? L"" : Util::to_utf16(vehicle["avg_consumption"].get<std::string>());
            table->AddRow({ licensePlate, year, model, make, fuel, avgConsumption });
        }
    }
    else {
        return;
        win->LoadInBackground(
            L"localhost",
            L"/car-edgers/cars/all",
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
    table->AddColumn(L"Username", 150);
    table->AddColumn(L"Email", 150);

    table->AddRow({ L"1", L"matthias", L"matthias@example.com" });
    table->AddRow({ L"1", L"matthias", L"matthias@example.com" });
    table->AddRow({ L"1", L"matthias", L"matthias@example.com" });
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
                std::cout << "[DEBUG] Driver created successfully: " << res.dump() << std::endl;
                MessageBox(nullptr, L"Driver created successfully!", L"Success", MB_OK);
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

    makeLabel = std::make_shared<Text>(L"Make:");
    makeLabel->SetPosition(20, 290);

    makeField = std::make_shared<InputField>();
    makeField->SetPosition(20, 310);
    makeField->SetFont(largeFont);
    makeField->SetSize(250, 40);

    fuelLabel = std::make_shared<Text>(L"Fuel:");
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
    auto numBlade = numBladeField->GetText();
    auto year = yearField->GetText();
    auto model = modelField->GetText();
    auto make = makeField->GetText();
    auto fuel = fuelField->GetText();
    auto avgConsumption = avgConsumptionField->GetText();
    json j = {
        {"num_Blade", Util::to_utf8(numBlade)},
        {"year", Util::to_utf8(year)},
        {"Model", Util::to_utf8(model)},
        {"make", Util::to_utf8(make)},
        {"fuel", Util::to_utf8(fuel)},
        {"avg_consumption", Util::to_utf8(avgConsumption)}
    };
    auto win = dynamic_cast<SubWindow*>(this->win);
    win->onCloseCallback = [j](Window* parent) {
        parent->LoadInBackground(
            L"localhost",
            L"/car-edgers/cars/create",
            j.dump(),
            "application/json",
            8080,
            [parent](const json& res) {
                std::cout << "[DEBUG] Car created successfully: " << res.dump() << std::endl;
                MessageBox(nullptr, L"Car created successfully!", L"Success", MB_OK);
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