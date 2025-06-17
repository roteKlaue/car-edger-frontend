#include "pch.h"
#include "Component.h"
#include "Util.h"
#include "Frame.h"

Component::Component() : id(Util::GenerateId()),
	handle(nullptr),
	window(nullptr),
	hFont(nullptr) {
}

void Component::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	if (initialized) SetWindowPos(handle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Component::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
	if (initialized) SetWindowPos(handle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void Component::SetFont(HFONT font)
{
	hFont = font;
	if (initialized) SendMessage(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void Component::SetVisible(bool visible)
{
	if (!handle) return;
	ShowWindow(handle, visible ? SW_SHOW : SW_HIDE);
}

void Component::SetParent(Frame* parent)
{
	if (!parent) return;
	window = parent;
	if (initialized) ::SetParent(handle, parent->GetWindowHandle());
}

void Component::Show(int cmdShow) const
{
	if (!handle) return;
	ShowWindow(handle, cmdShow);
}

void Component::Hide() const
{
	if (!handle) return;
	ShowWindow(handle, SW_HIDE);
}

void Component::Enable() const
{
	if (!handle) return;
	EnableWindow(handle, TRUE);
}

void Component::Disable() const
{
	if (!handle) return;
	EnableWindow(handle, FALSE);
}

void Component::SetFocus() const
{
	if (!handle) return;
	::SetFocus(handle);
}

void Component::OnParentResize(unsigned int parentWidth, unsigned int parentHeight)
{
	if (!initialized || !handle) return;

	// Proportional positioning
	if (xRatio >= 0.0f) x = static_cast<int>(parentWidth * xRatio);
	if (yRatio >= 0.0f) y = static_cast<int>(parentHeight * yRatio);

	// Proportional sizing
	if (widthRatio >= 0.0f) width = static_cast<int>(parentWidth * widthRatio);
	if (heightRatio >= 0.0f) height = static_cast<int>(parentHeight * heightRatio);

	// Anchoring overrides
	if (anchorFlags & ANCHOR_LEFT && anchorFlags & ANCHOR_RIGHT)
	{
		x = marginLeft;
		width = parentWidth - marginLeft - marginRight;
	}
	else if (anchorFlags & ANCHOR_LEFT)
	{
		x = marginLeft;
	}
	else if (anchorFlags & ANCHOR_RIGHT)
	{
		x = parentWidth - marginRight - width;
	}

	if (anchorFlags & ANCHOR_TOP && anchorFlags & ANCHOR_BOTTOM)
	{
		y = marginTop;
		height = parentHeight - marginTop - marginBottom;
	}
	else if (anchorFlags & ANCHOR_TOP)
	{
		y = marginTop;
	}
	else if (anchorFlags & ANCHOR_BOTTOM)
	{
		y = parentHeight - marginBottom - height;
	}

	SetWindowPos(handle, nullptr, x, y, width, height, SWP_NOZORDER);
}

void Component::SetSizeRatio(float widthR, float heightR) {
	widthRatio = widthR;
	heightRatio = heightR;
}

void Component::SetPositionRatio(float xR, float yR) {
	xRatio = xR;
	yRatio = yR;
}

void Component::SetAnchors(int flags, int left, int top, int right, int bottom)
{
	anchorFlags = flags;
	marginLeft = left;
	marginTop = top;
	marginRight = right;
	marginBottom = bottom;
}
