#include "pch.h"
#include "Text.h"
#include "Frame.h"
#include <iostream>

void Text::SetText(const std::wstring& text)
{
	this->text = text;
	Frame* window = this->window;

	if (!window || !window->GetWindowHandle()) return;

	InvalidateRect(window->GetWindowHandle(), nullptr, TRUE);
	UpdateWindow(window->GetWindowHandle());
}

void Text::Create() {}
void Text::Destroy() {}
