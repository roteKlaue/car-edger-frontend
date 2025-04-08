#pragma once

#include <functional>
#include "Component.h"

class Button :
    public Component
{
public:
    Button();
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;

    void SetText(const std::wstring& text) override;
    std::wstring GetText() const override { return text; };

    void SetOnClick(std::function<void()> callback);
    void Create() override;

private:
    std::wstring text;
    std::function<void()> onClick;

    void HandleCommand(WPARAM wParam, LPARAM lParam) override;
};

