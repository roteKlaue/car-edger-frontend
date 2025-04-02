#include "pch.h"
#include "WindowsDefenitions.h"

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

LRESULT TestWindow2::HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE) {
        // inputField->Show(SW_SHOW);
        inputField2->Show(SW_SHOW);
    }

    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(GetWindowHandle(), &ps);

        const wchar_t* text = L"Welcome";
        int x = 50;
        int y = 50;

        TextOut(hdc, x, y, text, wcslen(text));

        EndPaint(GetWindowHandle(), &ps);
        return 0;
    }

    return DefWindowProc(eventHandle, message, wParam, lParam);
}
