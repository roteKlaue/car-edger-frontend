#include "pch.h"
#include "WindowDefenitions.h"
#include "Text.h"

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
	
}

LoginWindow::LoginWindow(HINSTANCE hInstance, int nCmdShow)
    : Window(hInstance, L"LoginWindowClass", L"Car Edger Login", nCmdShow,
        WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX) {
    SetMenuResource(IDC_MAIN_WINDOW_MENU);
    HFONT largeFont = Util::CreatePointFont(16);
    HFONT largeXXLFont = Util::CreatePointFont(22);

    usernameField = std::make_shared<InputField>();
    usernameField->SetPlaceholder(L"Username");
    usernameField->SetPosition(20, 100);
    usernameField->SetFont(largeFont);

    passwordField = std::make_shared<InputField>();
    passwordField->SetPlaceholder(L"Password");
    passwordField->SetType(InputFieldType::PASSWORD);
    passwordField->SetPosition(20, 200);
    passwordField->SetFont(largeFont);

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
        std::wcout << L"[DEBUG] Username: " << usernameField->GetText() << std::endl;
        std::wcout << L"[DEBUG] Password: " << passwordField->GetText() << std::endl;
    });

    AddComponent(loginText);
    AddComponent(usernameLabel);
    AddComponent(passwordLabel);
    AddComponent(passwordField);
    AddComponent(usernameField);
    AddComponent(loginButton);
}

bool LoginWindow::Init()
{
	if (!Window::Init()) return false;
    Resize(260, 350);
    passwordField->SetSize(220, 40);
    usernameField->SetSize(220, 40);
    return true;
}

LRESULT LoginWindow::HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(eventHandle, message, wParam, lParam);
}