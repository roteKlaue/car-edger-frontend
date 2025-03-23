#include "InputField.h"
#include "Util.h"
#include "Window.h"

static std::wstring empty_str = L"";

InputField::InputField() : text(empty_str),
	placeholder(empty_str),
	type(InputFieldType::TEXT),  // Initialize with a default type
	id(Util::GenerateId()),            // Ensure GenerateId() is implemented
	hWnd(nullptr),
	window(nullptr),
	hFont(nullptr) {}

void InputField::SetText(const std::wstring text)
{
	if (!hWnd) return;
	this->text = text;
	if (initialized) SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)text.c_str());
}

std::wstring InputField::GetText() const
{
	return text;
}

void InputField::SetPosition(int x, int y)
{
	if (!hWnd) return;
	this->x = x;
	this->y = y;
	if (initialized) SetWindowPos(hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void InputField::SetSize(int width, int height)
{
	if (!hWnd) return;
	this->width = width;
	this->height = height;
	if (initialized) SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void InputField::SetFont(HFONT font)
{
	if (!hWnd) return;
	hFont = font;
	if (initialized) SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void InputField::SetParent(Window* parent)
{
	if (!parent) return;
	window = parent;
	if (initialized) ::SetParent(hWnd, parent->GetWindowHandle());
}

void InputField::Clear()
{
	if (!hWnd) return;
	text = empty_str;
	SetWindowText(hWnd, empty_str.c_str());
}

void InputField::Initialize()
{
	if (initialized) return;

	DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL;
	if (type == InputFieldType::PASSWORD) {
		style |= ES_PASSWORD;
	}
	else if (type == InputFieldType::NUMBER) {
		style |= ES_NUMBER;
	}

	hWnd = CreateWindowEx(
		0,
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

	if (hWnd)
	{
		initialized = true;
		SetFont(hFont);
	}
}

InputField::~InputField()
{
	if (hWnd) {
		DestroyWindow(hWnd);
		hWnd = nullptr;
		initialized = false;
	}
}

void InputField::Show()
{
	if (!hWnd) return;
	ShowWindow(hWnd, SW_SHOW);
}

void InputField::Hide() const
{
	if (!hWnd) return;
	ShowWindow(hWnd, SW_HIDE);
}

void InputField::Enable() const
{
	if (!hWnd) return;
	EnableWindow(hWnd, TRUE);
}

void InputField::Disable() const
{
	if (!hWnd) return;
	EnableWindow(hWnd, FALSE);
}

void InputField::SetFocus() const
{
	if (!hWnd) return;
	::SetFocus(hWnd);
}
