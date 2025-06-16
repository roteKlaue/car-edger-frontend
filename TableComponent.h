#pragma once

#include "Component.h"
#include <CommCtrl.h>
#include <vector>
#include <memory>
#include <string>
#include "Button.h"

#pragma comment(lib, "comctl32.lib")

class TableComponent : public Component {
public:
    TableComponent() = default;
    ~TableComponent() override;

    void Create() override;
    void Destroy() override;
    void OnParentResize(unsigned int width, unsigned int height) override;

    void AddColumn(const std::wstring& title, int width = 100);
    void AddRow(const std::vector<std::shared_ptr<Component>>& row);

    virtual void SetText(const std::wstring& text) override;
    virtual std::wstring GetText() const override;

    void SetParent(Frame* window) override;

private:
    HWND                                        listHandle = nullptr;
    int                                         columnCount = 0;
    std::vector<std::wstring>                   columns;
    std::vector<int>                            columnWidths;
    std::vector<std::shared_ptr<Component>>     cellComponents;
};
