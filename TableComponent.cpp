#include "pch.h"
#include "TableComponent.h"
#include <algorithm>

TableComponent::~TableComponent() {
    Destroy();
}

void TableComponent::Create() {
    if (!window || initialized) return;

    listHandle = CreateWindowExW(0, WC_LISTVIEW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED,
        x, y, width, height,
        window->GetWindowHandle(), (HMENU)(INT_PTR)id,
        GetModuleHandleW(nullptr), nullptr);
    if (!listHandle) return;

    ListView_SetExtendedListViewStyle(listHandle,
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    for (auto& comp : cellComponents) {
        comp->Create();
    }

    initialized = true;
}

void TableComponent::Destroy() {
    for (auto& comp : cellComponents) {
        comp->Destroy();
    }
    cellComponents.clear();

    if (listHandle) {
        DestroyWindow(listHandle);
        listHandle = nullptr;
    }
    initialized = false;
}

void TableComponent::AddColumn(const std::wstring& title, int width) {
    columns.push_back(title);
    columnWidths.push_back(width);
    if (initialized && listHandle) {
        LVCOLUMNW col{};
        col.mask = LVCF_TEXT | LVCF_WIDTH;
        col.pszText = const_cast<LPWSTR>(title.c_str());
        col.cx = width;
        ListView_InsertColumn(listHandle, columnCount, &col);
    }
    ++columnCount;
}

void TableComponent::AddRow(const std::vector<std::shared_ptr<Component>>& row) {
    if (static_cast<int>(row.size()) != columnCount || !listHandle) return;

    // Insert placeholder item for correct number of columns
    LVITEMW item{};
    item.mask = LVIF_TEXT;
    item.iItem = ListView_GetItemCount(listHandle);
    item.iSubItem = 0;
    item.pszText = (LPWSTR) L"";
    ListView_InsertItem(listHandle, &item);

    // Store and create components
    for (auto& comp : row) {
        cellComponents.push_back(comp);
        if (initialized) {
            comp->SetParent(window);
            comp->Create();
        }
    }

    // Layout cells immediately
    OnParentResize(width, height);
}

void TableComponent::SetText(const std::wstring& text) {}

std::wstring TableComponent::GetText() const
{
    return L"";
}

void TableComponent::SetParent(Frame* window)
{
    Component::SetParent(window);
    for (auto& comp : cellComponents) {
        comp->SetParent(window);
    }
}

void TableComponent::OnParentResize(unsigned int w, unsigned int h) {
    SetSize(static_cast<int>(w), static_cast<int>(h));
    if (listHandle) {
        MoveWindow(listHandle, x, y, width, height, TRUE);
    }
    if (!initialized || columnCount == 0) return;

    int rowCount = ListView_GetItemCount(listHandle);
    int totalCells = rowCount * columnCount;
    for (int idx = 0; idx < totalCells; ++idx) {
        int row = idx / columnCount;
        int col = idx % columnCount;
        RECT rc{};
        // Get rectangle for each subitem
        if (ListView_GetSubItemRect(listHandle, row, col, LVIR_LABEL, &rc) == FALSE) continue;

        auto& comp = cellComponents[idx];
        comp->SetPosition(rc.left, rc.top);
        comp->SetSize(rc.right - rc.left, rc.bottom - rc.top);
        if (comp->GetHandle()) {
            ::SetParent(comp->GetHandle(), GetParent(listHandle));
            ::SetWindowPos(comp->GetHandle(), HWND_TOP,
                rc.left + x, rc.top + y, rc.right - rc.left, rc.bottom - rc.top,
                SWP_NOZORDER | SWP_SHOWWINDOW);
        }
    }
}
