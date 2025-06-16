#include "pch.h"
#include "InputField.h"
#include "Util.h"
#include "Frame.h"
#include <iostream>
#include <commctrl.h>

static std::wstring empty_str = L"";

InputField::InputField() : Component(),
	text(empty_str),
	placeholder(empty_str),
	type(InputFieldType::TEXT){ }

InputField::InputField(const InputFieldType type) : Component(),
	text(empty_str),
	placeholder(empty_str),
	type(type) {}

void InputField::Destroy()
{
	if (handle) {
		DestroyWindow(handle);
		handle = nullptr;
	}
	initialized = false;
}

void InputField::SetText(const std::wstring& text)
{
	this->text = text;
	if (initialized) SendMessage(handle, WM_SETTEXT, 0, (LPARAM)text.c_str());
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

void InputField::HandleCommand(WPARAM wParam, LPARAM lParam)
{
	UINT notif = HIWORD(wParam);
	UINT ctrlId = LOWORD(wParam);

	if (ctrlId != id) return;
	if (notif != EN_CHANGE) return;
	
	wchar_t buffer[512];
	GetWindowText(handle, buffer, 512);
	text = buffer;

	if (onChange) onChange();
}

void InputField::Create()
{
	if (initialized) return;

	DWORD style = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP;
	if (type == InputFieldType::PASSWORD) {
		style |= ES_PASSWORD;
	}
	else if (type == InputFieldType::NUMBER) {
		style |= ES_NUMBER;
	}

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
