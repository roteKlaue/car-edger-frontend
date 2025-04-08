#include "pch.h"
#include "Text.h"
#include "Window.h"
#include <iostream>

void Text::SetText(const std::wstring& text)
{
	this->text = text;
	Window* window = this->window;

	if (!window || !window->GetWindowHandle()) return;

	InvalidateRect(window->GetWindowHandle(), nullptr, TRUE);
	UpdateWindow(window->GetWindowHandle());
}

void Text::Create() {}