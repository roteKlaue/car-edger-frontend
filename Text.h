#pragma once
#include "Component.h"
class Text :
    public Component
{
public:
	Text(const std::wstring& text) : Component(), text(text) {};
	Text() : Component(), text(L"") {};

	int GetX() { return x; };
	int GetY() { return y; };
	void Create() override;
	void Destroy() override;
	HFONT GetFont() { return hFont; };

	void SetText(const std::wstring& text) override;
	std::wstring GetText() const override { return text; };
	bool GetVisible() const { return visible; };
	void SetVisible(bool visible) override { this->visible = visible; };

private:
	COLOR16 color = 0;
	bool visible = true;
	std::wstring text = L"";
};

