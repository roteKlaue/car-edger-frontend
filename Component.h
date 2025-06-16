#pragma once

#include <windows.h>
#include "Window.h"
#include <string>

class Frame;

/**
 * @brief Abstract base class representing a GUI component.
 *
 * Provides common functionality and interface for all GUI components
 * (e.g., buttons, text fields). Derived classes must implement `Create`,
 * `Destroy`, `SetText`, and `GetText`.
 */
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
	virtual void OnParentResize(unsigned int width, unsigned int height) {};
	virtual void HandleNotify(NMHDR* nmhdr) {};

protected:
	UINT id;					///< Unique identifier for the component.
	HWND handle;				///< Native window handle.
	HFONT hFont;				///< Handle to the font used by the component.
	Frame* window;				///< Pointer to the parent frame.

	int width = 100;			///< Width of the component.
	int height = 20;			///< Height of the component.
	int x = 0;					///< X-coordinate of the component.
	int y = 0;					///< Y-coordinate of the component.

	bool initialized = false;	///< True if the component has been created.
};
