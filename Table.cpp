#include "pch.h"
#include "Table.h"
#include "Frame.h"
#include <iostream>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

Table::Table()
	: Component(),
	onRowClick(nullptr) {
	InitCommonControls();
}

void Table::Create()
{
	if (initialized || !window) return;

	handle = CreateWindowExW(
		0, WC_LISTVIEW, L"",
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES,
		x, y, width, height,
		window->GetWindowHandle(),
		(HMENU)(UINT_PTR)id,
		GetModuleHandle(nullptr),
		nullptr
	);

	if (!handle) return;

	SetFont(hFont);
	initialized = true;
}

void Table::Destroy()
{
	if (!initialized || !handle) return;
	DestroyWindow(handle);
	handle = nullptr;
	initialized = false;
}

void Table::SetText(const std::wstring& text) { /* Not applicable to ListView; could be used as a tooltip */ }

std::wstring Table::GetText() const {
	// Not applicable
	return L"";
}

void Table::AddColumn(const std::wstring& header, int colWidth, int index)
{
	if (!initialized) return;

	LVCOLUMNW lvc{};
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.cx = colWidth;
	lvc.pszText = const_cast<LPWSTR>(header.c_str());
	lvc.iSubItem = index < 0 ? static_cast<int>(columns.size()) : index;

	ListView_InsertColumn(handle, lvc.iSubItem, &lvc);
	columns.push_back(header);
}

void Table::AddRow(const std::vector<std::wstring>& rowData)
{
	if (!initialized || rowData.empty()) return;

	LVITEMW item{};
	item.mask = LVIF_TEXT;
	item.iItem = ListView_GetItemCount(handle);
	item.pszText = const_cast<LPWSTR>(rowData[0].c_str());
	ListView_InsertItem(handle, &item);

	for (size_t i = 1; i < rowData.size(); ++i)
	{
		ListView_SetItemText(handle, item.iItem, static_cast<int>(i), const_cast<LPWSTR>(rowData[i].c_str()));
	}
}

void Table::HandleNotify(NMHDR* nmhdr)
{
	std::wcout << L"Table clicked" << std::endl;
	switch (nmhdr->code) {
		case NM_CLICK: std::wcout << L"NM_CLICK\n"; break;
		case NM_DBLCLK: std::wcout << L"NM_DBLCLK\n"; break;
		case NM_RETURN: std::wcout << L"NM_RETURN\n"; break;
		case LVN_ITEMACTIVATE: std::wcout << L"LVN_ITEMACTIVATE\n"; break;
		case LVN_ITEMCHANGED: std::wcout << L"LVN_ITEMCHANGED\n"; break;
		default: std::wcout << L"Other code: " << nmhdr->code << std::endl; break;
	}

	if (nmhdr->code == NM_CLICK 
		|| nmhdr->code == LVN_ITEMACTIVATE 
		|| nmhdr->code == NM_RETURN)
	{
		LPNMITEMACTIVATE item = reinterpret_cast<LPNMITEMACTIVATE>(nmhdr);
		std::wcout << L"Clicked iItem: " << item->iItem << std::endl;
		if (item->iItem >= 0)
		{
			std::wcout << L"Row clicked: " + std::to_wstring(item->iItem) << std::endl;

			if (onRowClick) onRowClick(item->iItem);
		}
	}
}

void Table::Clear()
{
	if (!initialized) return;
	ListView_DeleteAllItems(handle);
}

void Table::SetOnRowClick(const std::function<void(int)>& callback)
{
	onRowClick = callback;
}

void Table::OnParentResize(unsigned int parentWidth, unsigned int parentHeight)
{
	if (!initialized || !handle) return;
	Adjust(parentWidth, parentHeight);
}

void Table::Adjust(int parentWidth, int parentHeight) {
	int newWidth = parentWidth - x - rightOffset;
	int newHeight = parentHeight - y - bottomOffset;

	SetWindowPos(handle, nullptr, x, y, newWidth, newHeight, SWP_NOZORDER);
}

void Table::SetOffsets(int right, int bottom)
{
	rightOffset = right;
	bottomOffset = bottom;
}

void Table::SetRightOffset(int offset)
{
	rightOffset = offset;
}

void Table::SetBottomOffset(int offset)
{
	bottomOffset = offset;
}
