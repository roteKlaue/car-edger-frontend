#include "pch.h"
#include "Frame.h"
#include "Window.h"
#include "Component.h"
#include "Text.h"

void Frame::WakeUp(Window* win, const json& options)
{
    this->win = win;
    for (auto& comp : components) {
        comp->SetParent(this);
        comp->Create();
        comp->Show(SW_SHOW);
    }
}

void Frame::Unload(Window*)
{
    for (auto& comp : components) {
        comp->Destroy();
    }
    components.clear();
}

LRESULT Frame::HandleMessage(Window*, UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg != WM_COMMAND) return NOT_HANDLED;
    UINT id = LOWORD(wp);

    for (auto& comp : components) {
        if (comp->GetID() != id) continue;
        comp->HandleCommand(wp, lp);
        return 0;
    }

    return NOT_HANDLED;
}

void Frame::RegisterComponent(std::shared_ptr<Component> comp)
{
    if (comp) components.push_back(std::move(comp));
}

void Frame::OnPaint(HDC hdc)
{
    for (auto& comp : components) {
        auto text = std::dynamic_pointer_cast<Text>(comp);
        if (!text || !text->GetVisible()) continue;

        std::wstring str = text->GetText();
        SetBkMode(hdc, TRANSPARENT);
        HFONT old = nullptr;
        if (auto f = text->GetFont()) old = (HFONT) SelectObject(hdc, f);
        TextOut(hdc, text->GetX(), text->GetY(), str.c_str(), str.length());
        
        if (old) SelectObject(hdc, old);
    }
}

HWND Frame::GetWindowHandle() const {
    return win ? win->GetWindowHandle() : nullptr;
}
