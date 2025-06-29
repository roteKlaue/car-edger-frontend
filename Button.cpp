#include "pch.h"
#include "Button.h"
#include "Frame.h"

Button::Button() : Component(), text(L"Button") {}

Button::Button(std::wstring text) : Component(), text(std::move(text)) {}

void Button::SetText(const std::wstring& text)
{
    this->text = text;
    if (initialized && handle) {
        SetWindowText(handle, text.c_str());
    }
}

void Button::Destroy()
{
    if (handle) {
		DestroyWindow(handle);
		handle = nullptr;
	}
	initialized = false;
}

void Button::SetOnClick(std::function<void()> callback)
{
    onClick = std::move(callback);
}

std::function<void()> Button::GetOnClick() const
{
	return onClick;
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
