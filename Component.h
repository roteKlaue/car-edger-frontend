#pragma once

#include <windows.h>
#include "Window.h"
#include <string>

class Frame;

class Component
{
public:
	Component();
	virtual ~Component() = default;
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	virtual void Show(int cmdShow) const;
	virtual void Hide() const;
	virtual void Enable() const;
	virtual void Disable() const;

	virtual void SetPosition(int x, int y);
	virtual void SetSize(int width, int height);
	virtual void SetFont(HFONT font);
	virtual void SetVisible(bool visible);
	virtual void SetFocus() const;
	virtual void SetParent(Frame* window);
	virtual int GetID() const { return id; };
	virtual HWND GetHandle() const { return handle; };

	virtual void SetText(const std::wstring& text) = 0;
	virtual std::wstring GetText() const = 0;
	virtual void HandleCommand(WPARAM wParam, LPARAM lParam) {};

protected:
	UINT id;
	HWND handle;
	HFONT hFont;
	Frame* window;

	int width = 100;
	int height = 20;
	int x = 0;
	int y = 0;

	bool initialized = false;
};
