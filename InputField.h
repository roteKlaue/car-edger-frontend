#pragma once

#include <windows.h>
#include <string>
#include "Component.h"
#include <functional>

/**
 * @brief Enum representing different types of input fields.
 */
enum class InputFieldType
{
	TEXT,       ///< Default text input.
	PASSWORD,   ///< Obscured password input.
	NUMBER,     ///< Numeric input only.
	EMAIL,      ///< Email input (not enforced by WinAPI).
	DATETIME,   ///< Date and time input.
	DATE,       ///< Date only input.
	TIME,       ///< Time only input.
	TEL,        ///< Telephone number input.
	URL         ///< URL input.
};

class Window;

/**
 * @brief GUI component for user text input.
 *
 * Supports various types such as text, number, password, etc. with placeholder text
 * and change event callbacks.
 */
class InputField : public Component
{
public:
	InputField();
	InputField(const InputFieldType type);
	InputField(const InputField&) = delete;
	InputField& operator=(const InputField&) = delete;
	/// Destroys the input field and releases associated resources.
	void Destroy() override;
	/// Sets the displayed text content.
	void SetText(const std::wstring& text) override;
	/// Sets the type of input field (e.g., PASSWORD, NUMBER).
	void SetType(const InputFieldType type) { this->type = type; };
	/// Sets placeholder text shown when field is empty.
	void SetPlaceholder(const std::wstring text);
	/// Keeps the placeholder even when focused (Windows 10+).
	void SetKeepPlaceholder(bool keep) { keepPlaceholder = keep; };
	/// Returns the current text content.
	std::wstring GetText() const;
	/// Creates the input field window and initializes styles.
	void Create() override;
	/// Clears the current content of the input field.
	void Clear();

	/// Handles WM_COMMAND messages from parent.
	void HandleCommand(WPARAM wParam, LPARAM lParam) override;
	/// Sets the callback function to call when text changes.
	void SetOnChange(std::function<void()> callback) { onChange = std::move(callback); }

private:
	std::wstring text;                ///< Current text in the field.
	std::wstring placeholder;         ///< Placeholder (cue banner) text.

	bool keepPlaceholder = false;    ///< Whether placeholder remains visible on focus.
	InputFieldType type;             ///< Type of the input field.
	std::function<void()> onChange;  ///< Callback function for EN_CHANGE.
};
