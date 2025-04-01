#include "pch.h"
#include "Windows.h"

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
