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

LRESULT LoginWindow::HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(eventHandle, message, wParam, lParam);
}