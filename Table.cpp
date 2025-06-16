#include "pch.h"
#include "Table.h"
#include "Frame.h"
#include <iostream>
#include <CommCtrl.h>

Table::Table() {
	InitCommonControls(); // Required for ListView
}

void Table::Create()
{
	if (initialized || !window) return;

	handle = CreateWindowExW(
		0, WC_LISTVIEW, L"",
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
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
	if (nmhdr->code == NM_CLICK || nmhdr->code == LVN_ITEMACTIVATE)
	{
		LPNMITEMACTIVATE item = reinterpret_cast<LPNMITEMACTIVATE>(nmhdr);
		if (item->iItem >= 0)
		{
			// You can now use item->iItem to identify which row was clicked
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

	// Resize to fit parent window, keeping the original x/y offsets
	int newWidth = parentWidth - x;
	int newHeight = parentHeight - y;

	SetWindowPos(handle, nullptr, x, y, newWidth, newHeight, SWP_NOZORDER);
}

