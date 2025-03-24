#pragma once

#include <windows.h>
#include <string>
#include "Component.h"

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
	void SetText(const std::wstring text);
	void UpdateText(const std::wstring text);
	void SetPlaceholder(const std::wstring text);
	void SetKeepPlaceholder(bool keep) { keepPlaceholder = keep; };
	std::wstring GetText() const;
	void Create() override;
	void Clear();

private:
	std::wstring text;
	std::wstring placeholder;

	bool keepPlaceholder = false;
	InputFieldType type;
};
