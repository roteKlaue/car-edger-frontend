#include "Component.h"
#include "Util.h"
#include "Window.h"

Component::Component() : id(Util::GenerateId()), 
	handle(nullptr), 
	window(nullptr), 
	hFont(nullptr) {}

Component::~Component()
{
	if (handle) {
		DestroyWindow(handle);
		handle = nullptr;
		initialized = false;
	}
}

void Component::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	if (initialized) SetWindowPos(handle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Component::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
	if (initialized) SetWindowPos(handle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void Component::SetFont(HFONT font)
{
	hFont = font;
	if (initialized) SendMessage(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void Component::SetVisible(bool visible)
{
	if (!handle) return;
	ShowWindow(handle, visible ? SW_SHOW : SW_HIDE);
}

void Component::SetParent(Window* parent)
{
	if (!parent) return;
	window = parent;
	if (initialized) ::SetParent(handle, parent->GetWindowHandle());
}

void Component::Show(int cmdShow) const
{
	if (!handle) return;
	ShowWindow(handle, cmdShow);
}

void Component::Hide() const
{
	if (!handle) return;
	ShowWindow(handle, SW_HIDE);
}

void Component::Enable() const
{
	if (!handle) return;
	EnableWindow(handle, TRUE);
}

void Component::Disable() const
{
	if (!handle) return;
	EnableWindow(handle, FALSE);
}

void Component::SetFocus() const
{
	if (!handle) return;
	::SetFocus(handle);
}