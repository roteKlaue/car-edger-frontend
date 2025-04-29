#include "pch.h"
#include "WindowDefenitions.h"
#include "Text.h"
#include <nlohmann/json.hpp>

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

MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow)
    : Window(hInstance, L"DefaultWindowClass", L"Car Edger", nCmdShow) {
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
    : Window(hInstance, L"TestWindowClass", L"Car Edger Test", nCmdShow) {
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
}

LoginWindow::LoginWindow(HINSTANCE hInstance, int nCmdShow)
    : Window(hInstance, L"LoginWindowClass", L"Car Edger Login", nCmdShow,
        WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX,
        true) {
    SetMenuResource(IDC_LOGIN_MENU);
    HFONT largeFont = Util::CreatePointFont(16);
    HFONT largeXXLFont = Util::CreatePointFont(22);
    Resize(260, 350);

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
    loginButton->SetOnClick([this]() {
        std::wcout << L"[DEBUG] Login button clicked." << std::endl;

        std::wstring username = usernameField->GetText();
        std::wstring password = passwordField->GetText();

        std::wcout << L"[DEBUG] Username: " << username << std::endl;
        std::wcout << L"[DEBUG] Password: " << password << std::endl;

        std::string usernameStr = Util::to_utf8(username);
        std::string passwordStr = Util::to_utf8(password);

		json jsonObject = {
			{"username", usernameStr },
			{"password", passwordStr }
        };

        std::cout << "[DEBUG] Resulting JSON: " << jsonObject.dump(2) << std::endl;
    });

    AddComponent(loginText);
    AddComponent(usernameLabel);
    AddComponent(passwordLabel);
    AddComponent(passwordField);
    AddComponent(usernameField);
    AddComponent(loginButton);
}

void LoginWindow::HandleFileMenuPress(UINT id)
{
    switch (id) {
    case ID_LOGIN_FILE_EXIT:
        PostMessage(GetWindowHandle(), WM_CLOSE, 0, 0);
        break;
    case ID_LOGIN_HELP_ABOUT:
        DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUTBOX), GetWindowHandle(), About);
        break;
    default:
        break;
    }
}

LRESULT LoginWindow::HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(eventHandle, message, wParam, lParam);
}