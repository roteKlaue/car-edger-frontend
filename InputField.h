#pragma once

#include <windows.h>
#include <string>

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

class InputField
{
public:
	InputField();
	virtual ~InputField();
	InputField(const InputField&) = delete;
	InputField& operator=(const InputField&) = delete;
	void SetText(const std::wstring text);
	std::wstring GetText() const;
	void SetPosition(int x, int y);
	void SetSize(int width, int height);
	void SetFont(HFONT font);
	void SetParent(Window* window);
	void Show();
	void Hide() const;
	void Enable() const;
	void Disable() const;
	void SetFocus() const;
	void Clear();
	HWND GetHandle() const { return hWnd; }
	void Initialize();

private:
	std::wstring text;
	std::wstring placeholder;

	bool initialized = false;
	int width = 100;
	int height = 20;
	int x = 0;
	int y = 0;

	InputFieldType type;
	UINT id;
	HWND hWnd;
	Window* window;
	HFONT hFont;
};
