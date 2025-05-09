#include "pch.h"
#include "FrameDefenitions.h"
#include "Text.h"
#include <nlohmann/json.hpp>
#include "Window.h"

using json = nlohmann::json;

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

const HttpClient* LoginFrame::httpClient = new HttpClient();

/*
MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow)
    : Frame(hInstance, L"DefaultWindowClass", L"Car Edger", nCmdShow, WS_OVERLAPPEDWINDOW, true) {
    SetMenuResource(IDC_CAREDGER);
}

LRESULT MainWindow::HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(eventHandle, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(eventHandle, &ps);
        return 0;
    }
    default:
        return DefWindowProc(eventHandle, message, wParam, lParam);
    }
}

LRESULT TestWindow::HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(eventHandle, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(eventHandle, &ps);
        return 0;
    }
    default:
        return DefWindowProc(eventHandle, message, wParam, lParam);
    }
}

TestWindow2::TestWindow2(HINSTANCE hInstance, int nCmdShow) 
    : Frame(hInstance, L"TestWindowClass", L"Car Edger Test", nCmdShow) {
    SetMenuResource(IDC_MAIN_WINDOW_MENU);

    inputField2 = std::make_shared<InputField>();
    text = std::make_shared<Text>();

	text->SetText(L"Welcome to Car Edger");
	text->SetPosition(20, 20);

    inputField2->SetPlaceholder(L"Choco");
    inputField2->SetPosition(20, 300);
    inputField2->SetSize(200, 20);

    AddComponent(text);
    AddComponent(inputField2);
}

LRESULT TestWindow2::HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(eventHandle, message, wParam, lParam);
}

void TestWindow2::HandleFileMenuPress(UINT id)
{
	switch (id) {
	case ID_FILE_EXIT:
		PostMessage(GetWindowHandle(), WM_CLOSE, 0, 0);
		break;
	case ID_HELP_ABOUT:
		DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUTBOX), GetWindowHandle(), About);
		break;
	default:
		break;
	}
} */

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

    try {
        std::cout << "[DEBUG] Requesting with body: " << j.dump() << std::endl;
        auto response = LoginFrame::httpClient->post(L"localhost", L"/car-edgers/auth/login", j.dump(), "application/json", 8080);
        auto res = json::parse(response);   
        std::cout << "[DEBUG] Response body: " << res.dump() << std::endl;

        if(res.value("success", true)) {
            std::cout << "[DEBUG] Response body: " << res.dump() << std::endl;
            auto main = std::make_shared<MainFrame>();
            win->LoadFrame(main, res);
        }
    }
    catch (const std::exception& e) {
        std::cout << "[ERROR] Exception: " << e.what() << '\n';
    }
    catch (...) {
        std::cout << "[ERROR] Unknown exception occurred.\n";
    }
}

void LoginFrame::WakeUp(Window* win, const json& options)
{
    Frame::WakeUp(win, options);

    win->SetTitle(L"Car Edger - Login");
    win->SetStyle(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
    win->SetSize(260, 350);
    win->SetMenuResource(IDC_LOGIN_MENU);    
}

MainFrame::MainFrame() : Frame() {}

void MainFrame::WakeUp(Window* win, const json& options)
{
    std::cout << "[DEBUG] Options body: " << options.dump() << std::endl;
    Frame::WakeUp(win, options);
    // Configure window appearance when frame loads
    win->SetTitle(L"Car Edger - Mainpage");
    win->SetStyle(WS_OVERLAPPEDWINDOW);
    // win->SetSize(-1, -1);
    std::cout << "[DEBUG] Finished resetting Frame." << std::endl;
}
