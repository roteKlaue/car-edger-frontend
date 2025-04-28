#include "pch.h"
#include "Button.h"
#include "Window.h"

Button::Button() : Component(), text(L"Button") {}

void Button::SetText(const std::wstring& text)
{
    this->text = text;
    if (initialized && handle) {
        SetWindowText(handle, text.c_str());
    }
}

void Button::SetOnClick(std::function<void()> callback)
{
    onClick = std::move(callback);
}

void Button::Create()
{
    if (!window) return;

    handle = CreateWindowExW(
        0,
        L"BUTTON",
        text.c_str(),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x, y, width, height,
        window->GetWindowHandle(),
        (HMENU)(INT_PTR)id,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (!handle) return;

    if (hFont) {
        SendMessage(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
    }

    initialized = true;
}

void Button::HandleCommand(WPARAM wParam, LPARAM lParam)
{
    UINT notif = HIWORD(wParam);
    UINT ctrlId = LOWORD(wParam);

    if (notif == BN_CLICKED && ctrlId == id && onClick)
    {
        onClick();
    }
}
