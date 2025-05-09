#include "pch.h"
#include "SubWindow.h"

void SubWindow::SetTitle(const std::wstring& title)
{
	if (!parent) {
		throw std::runtime_error("SubWindow requires parent before using it.");
		return;
	}
	parent->SetTitle(title);
}

void SubWindow::RunOnBackgroundThread(std::function<void()> func)
{
	if (!parent) {
		throw std::runtime_error("SubWindow requires parent before using it.");
		return;
	}
	parent->RunOnBackgroundThread(func);
}

void SubWindow::PostToUIThread(std::function<void()> func)
{
	if (!parent) {
		throw std::runtime_error("SubWindow requires parent before using it.");
		return;
	}
	parent->PostToUIThread(func);
}

void SubWindow::SetParent(Window* parent)
{
	this->parent = parent;
}

