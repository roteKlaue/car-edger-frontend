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
	/// Anchor flags for positioning relative to parent window edges.
	enum Anchor {
		ANCHOR_LEFT = 1 << 0,   ///< Anchor to the left edge of the parent.
		ANCHOR_RIGHT = 1 << 1,  ///< Anchor to the right edge of the parent.
		ANCHOR_TOP = 1 << 2,    ///< Anchor to the top edge of the parent.
		ANCHOR_BOTTOM = 1 << 3  ///< Anchor to the bottom edge of the parent.
	};

	Component();
	virtual ~Component() = default;
	
	/// Creates the GUI component (must be implemented by derived class).
	virtual void Create() = 0;
	/// Destroys the GUI component (must be implemented by derived class).
	virtual void Destroy() = 0;
	/// Shows the component using the specified command.
	virtual void Show(int cmdShow) const;
	/// Hides the component.
	virtual void Hide() const;
	/// Enables the component for user interaction.
	virtual void Enable() const;
	/// Disables the component.
	virtual void Disable() const;

	/// Sets the position of the component.
	virtual void SetPosition(int x, int y);
	/// Sets the size of the component.
	virtual void SetSize(int width, int height);
	/// Sets the font used by the component.
	virtual void SetFont(HFONT font);
	/// Shows or hides the component.
	virtual void SetVisible(bool visible);
	/// Sets focus to the component.
	virtual void SetFocus() const;
	/// Sets the parent window of the component.
	virtual void SetParent(Frame* window);
	/// Returns the unique identifier of the component.
	virtual int GetID() const { return id; };
	/// Returns the native window handle of the component.
	virtual HWND GetHandle() const { return handle; };

	/// Sets the text content of the component (must be implemented by derived class).
	virtual void SetText(const std::wstring& text) = 0;
	/// Gets the text content of the component (must be implemented by derived class).
	virtual std::wstring GetText() const = 0;
	/// Handles command messages (optional override).
	virtual void HandleCommand(WPARAM wParam, LPARAM lParam) {};
	/// Called when the parent window is resized.
	virtual void OnParentResize(unsigned int width, unsigned int height);
	/// Handles notification messages (optional override).
	virtual void HandleNotify(NMHDR* nmhdr) {};

	/// Sets width/height relative to parent as a ratio (0.0 to 1.0).
	void SetSizeRatio(float widthRatio, float heightRatio);
	/// Sets X/Y position relative to parent as a ratio (0.0 to 1.0).
	void SetPositionRatio(float xRatio, float yRatio);
	/// Sets anchor flags and optional margins.
	void SetAnchors(int flags, int left = 0, int top = 0, int right = 0, int bottom = 0);

protected:
	UINT id;					///< Unique identifier for the component.
	HWND handle;				///< Native window handle.
	HFONT hFont;				///< Handle to the font used by the component.
	Frame* window;				///< Pointer to the parent frame.

	int width = 100;			///< Width of the component.
	int height = 20;			///< Height of the component.
	int x = 0;					///< X-coordinate of the component.
	int y = 0;					///< Y-coordinate of the component.

	// Fraction of parent's width/height to occupy, e.g., 0.5 = 50% of parent
	float widthRatio = -1.0f;   ///< Width ratio relative to parent (-1 if unused).
	float heightRatio = -1.0f;  ///< Height ratio relative to parent (-1 if unused).
	float xRatio = -1.0f;       ///< X-position ratio relative to parent (-1 if unused).
	float yRatio = -1.0f;       ///< Y-position ratio relative to parent (-1 if unused).

	bool initialized = false;	///< True if the component has been created.

	int anchorFlags = 0;		///< Combination of Anchor flags.

	// Margins used for anchoring calculations
	int marginLeft = 0;
	int marginRight = 0;
	int marginTop = 0;
	int marginBottom = 0;
};
