#pragma once

#include <windows.h>
#include <string>
#include "Component.h"
#include <functional>

enum class InputFieldType
{
	TEXT,
	PASSWORD,
	NUMBER,
	EMAIL,
	DATETIME,
	DATE,
	TIME,
	TEL,
	URL
};

class Window;

class InputField : public Component
{
public:
	InputField();
	InputField(const InputField&) = delete;
	InputField& operator=(const InputField&) = delete;
	void Destroy() override;
	void SetText(const std::wstring& text) override;
	void SetType(const InputFieldType type) { this->type = type; };
	void SetPlaceholder(const std::wstring text);
	void SetKeepPlaceholder(bool keep) { keepPlaceholder = keep; };
	std::wstring GetText() const;
	void Create() override;
	void Clear();

	void HandleCommand(WPARAM wParam, LPARAM lParam) override;
	void SetOnChange(std::function<void()> callback) { onChange = std::move(callback); }

private:
	std::wstring text;
	std::wstring placeholder;

	bool keepPlaceholder = false;
	InputFieldType type;
	std::function<void()> onChange;
};
