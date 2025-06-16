#include "pch.h"
#include "CustomOwnerDrawListView.h"

CustomOwnerDrawListView::CustomOwnerDrawListView(HWND parent, int x, int y, int width, int height, int id)
    : parent_(parent), x_(x), y_(y), width_(width), height_(height), id_(id) {
}

bool CustomOwnerDrawListView::Create()
{
    if (hwnd_) return false;
    hwnd_ = CreateWindowExW(
        0, WC_LISTVIEWW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED,
        x_, y_, width_, height_,
        parent_, (HMENU)(INT_PTR)id_,
        GetModuleHandleW(nullptr), nullptr);
    if (!hwnd_) return false;

    ListView_SetExtendedListViewStyle(hwnd_, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    return true;
}

HWND CustomOwnerDrawListView::Handle() const
{
    return hwnd_;
}
