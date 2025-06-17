#pragma once

#include "Component.h"
#include <vector>
#include <string>

class Table : public Component
{
public:
	Table();
	virtual ~Table() = default;

	void Create() override;
	void Destroy() override;

	void SetText(const std::wstring& text) override;
	std::wstring GetText() const override;

	void AddColumn(const std::wstring& header, int width, int index = -1);
	void AddRow(const std::vector<std::wstring>& rowData);
	void SetOnRowClick(const std::function<void(int)>& callback);
	void Clear();

	void HandleNotify(NMHDR* nmhdr) override;
	void OnParentResize(unsigned int parentWidth, unsigned int parentHeight) override;
	void Adjust(int width, int height);

	void SetOffsets(int right, int bottom);
	void SetRightOffset(int offset);
	void SetBottomOffset(int offset);
private:
	std::vector<std::wstring> columns;
	std::function<void(int)> onRowClick;

	int rightOffset = 0;
	int bottomOffset = 0;
};
