#pragma once

#include "TableComponent.h"
#include <commctrl.h>
#include <vector>

class CustomOwnerDrawListView {
public:
    CustomOwnerDrawListView(HWND parent, int x, int y, int width, int height, int id);

    bool Create();

    HWND Handle() const;

    // Should be called in the parent window's WM_NOTIFY handler
    LRESULT OnNotify(LPNMHDR hdr) {
        if (hdr->idFrom != id_ || hdr->code != NM_CUSTOMDRAW)
            return 0;

        LPNMLVCUSTOMDRAW cd = reinterpret_cast<LPNMLVCUSTOMDRAW>(hdr);
        switch (cd->nmcd.dwDrawStage) {
        case CDDS_PREPAINT:
            return CDRF_NOTIFYITEMDRAW;

        case CDDS_ITEMPREPAINT:
            return CDRF_NOTIFYSUBITEMDRAW;

        case CDDS_SUBITEM | CDDS_ITEMPREPAINT: {
            int row = static_cast<int>(cd->nmcd.dwItemSpec);
            int col = cd->iSubItem;
            DrawCell(cd->nmcd.hdc, row, col, &cd->nmcd.rc);
            return CDRF_SKIPDEFAULT;
        }
        }
        return CDRF_DODEFAULT;
    }

    void AddColumn(const std::wstring& title, int width) {
        LVCOLUMNW col{};
        col.mask = LVCF_TEXT | LVCF_WIDTH;
        col.pszText = const_cast<LPWSTR>(title.c_str());
        col.cx = width;
        ListView_InsertColumn(hwnd_, columns_.size(), &col);
        columns_.push_back(title);
    }

    void AddRow(const std::vector<std::shared_ptr<Component>>& cells) {
        if (cells.size() != columns_.size()) return;
        int index = ListView_GetItemCount(hwnd_);
        LVITEMW item{};
        item.mask = LVIF_TEXT;
        item.iItem = index;
        item.iSubItem = 0;
        item.pszText = const_cast<LPWSTR>(L"");
        ListView_InsertItem(hwnd_, &item);
        rows_.push_back(cells);
    }

private:
    void DrawCell(HDC hdc, int row, int col, RECT* rc) {
        const auto& comp = rows_[row][col];
        std::wstring text = comp->GetText();

        // Use default drawing for text
        DrawTextW(hdc, text.c_str(), -1, rc,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    HWND parent_ = nullptr;
    HWND hwnd_ = nullptr;
    int x_, y_, width_, height_, id_;
    std::vector<std::wstring> columns_;
    std::vector<std::vector<std::shared_ptr<Component>>> rows_;
};

