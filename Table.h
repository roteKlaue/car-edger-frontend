#pragma once

#include "Component.h"
#include <vector>
#include <string>

/**
 * @brief A table component based on Win32 ListView (LVS_REPORT).
 *
 * Supports dynamic columns and rows, resizing, click callbacks, and offset-based layout.
 * Inherits from Component.
 */
class Table : public Component
{
public:
	/// Constructs a Table and initializes common controls.
	Table();
	/// Destroys the Table (virtual).
	virtual ~Table() = default;

	/// Creates the ListView window.
	void Create() override;
	/// Destroys the ListView window.
	void Destroy() override;

	/// Not applicable for Table; does nothing.
	void SetText(const std::wstring& text) override;
	/// Not applicable for Table; returns empty string.
	std::wstring GetText() const override;

	/**
	 * @brief Adds a column to the table.
	 *
	 * @param header Column header text.
	 * @param width Width of the column in pixels.
	 * @param index Optional column index (default: append to end).
	 */
	void AddColumn(const std::wstring& header, int width, int index = -1);
	
	/**
	 * @brief Adds a row to the table.
	 *
	 * @param rowData A vector of column values for the row.
	 */
	void AddRow(const std::vector<std::wstring>& rowData);
	
	/**
	 * @brief Sets a callback to be invoked when a row is clicked.
	 *
	 * @param callback Function taking the row index.
	 */
	void SetOnRowClick(const std::function<void(int)>& callback);
	
	/// Removes all rows from the table.
	void Clear();

	/**
	 * @brief Handles ListView notifications from the parent window.
	 *
	 * @param nmhdr Notification header.
	 */
	void HandleNotify(NMHDR* nmhdr) override;

	/**
	 * @brief Adjusts control size when parent is resized.
	 *
	 * @param parentWidth New parent width.
	 * @param parentHeight New parent height.
	 */
	void OnParentResize(unsigned int parentWidth, unsigned int parentHeight) override;
	
	/**
	 * @brief Resizes the table using offsets and parent dimensions.
	 *
	 * @param width Parent width.
	 * @param height Parent height.
	 */
	void Adjust(int width, int height);

	/**
	 * @brief Sets both right and bottom offsets for layout resizing.
	 *
	 * @param right Pixels from right edge.
	 * @param bottom Pixels from bottom edge.
	 */
	void SetOffsets(int right, int bottom);
	
	/**
	 * @brief Sets the right offset for layout resizing.
	 *
	 * @param offset Pixels from right edge.
	 */
	void SetRightOffset(int offset);
	
	/**
	 * @brief Sets the bottom offset for layout resizing.
	 *
	 * @param offset Pixels from bottom edge.
	 */
	void SetBottomOffset(int offset);
private:
	std::vector<std::wstring> columns;		///< Column headers.
	std::function<void(int)> onRowClick;	///< Row click callback.

	int rightOffset = 0;					///< Offset from the right edge.
	int bottomOffset = 0;					///< Offset from the bottom edge.
};
