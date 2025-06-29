#pragma once

#include "Window.h"

class SubWindow : 
	public Window
{
public:
	using Window::Window;
	~SubWindow() = default;

	void SetTitle(const std::wstring& title) override;
	void SetParent(Window* parent);

	void RunOnBackgroundThread(std::function<void()> func) override;
	void PostToUIThread(std::function<void()> func) override;
	void AssertBGThread() const override;
	void AssertUIThread() const override;

	std::function<void(Window*)> onCloseCallback = nullptr;

	Window* GetParent() const {
		return parent;
	}
private:
	Window* parent = nullptr;
};

