#include "pch.h"
#include "InputField.h"
#include "Util.h"
#include "Window.h"
#include <iostream>
#include <commctrl.h>

static std::wstring empty_str = L"";

InputField::InputField() : Component(),
	text(empty_str),
	placeholder(empty_str),
	type(InputFieldType::TEXT){ }

void InputField::SetText(const std::wstring text)
{
	if (!handle) return;
	this->text = text;
	if (initialized) SendMessage(handle, WM_SETTEXT, 0, (LPARAM)text.c_str());
}

void InputField::UpdateText(const std::wstring text)
{
	if (!handle) return;
	this->text = text;
}

void InputField::SetPlaceholder(const std::wstring text)
{
	placeholder = text;
	if (initialized) SendMessage(handle, EM_SETCUEBANNER, keepPlaceholder, (LPARAM)text.c_str());
}

std::wstring InputField::GetText() const
{
	return text;
}

void InputField::Clear()
{
	if (!handle) return;
	text = empty_str;
	SetWindowText(handle, empty_str.c_str());
}

void InputField::Create()
{
	if (initialized) return;

	std::cout << "Create called" << std::endl;

	DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL;
	if (type == InputFieldType::PASSWORD) {
		style |= ES_PASSWORD;
	}
	else if (type == InputFieldType::NUMBER) {
		style |= ES_NUMBER;
	}

	std::cout << "Style: " << style << std::endl;
	std::cout << "X,Y: " << x << "," << y << std::endl;
	std::cout << "Width,Height: " << width << "," << height << std::endl;

	handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		text.c_str(),
		style,
		x, y,
		width, height,
		window ? window->GetWindowHandle() : nullptr,
		(HMENU)id,
		GetModuleHandle(nullptr),
		nullptr
	);

	if (handle)
	{
		initialized = true;
		SetFont(hFont);
		if (placeholder.length() > 0) SendMessage(handle, EM_SETCUEBANNER, keepPlaceholder, (LPARAM)placeholder.c_str());
	}
}
