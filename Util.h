// Util.h : Utility functions for Win32 GUI applications.
// Author: Jan Koeck
// Created: 2025/04/26
//

#pragma once

#include <windows.h>
#include <string>
#include <optional>

namespace Util {
	/**
	 * @brief Generates a unique ID for controls or menus.
	 * @return A unique UINT identifier, incrementing on each call.
	 */
	UINT GenerateId();

	/**
	 * @brief Creates an HFONT with the specified point size and font family.
	 * @param pointSize Font height in points (e.g., 10 for 10pt).
	 * @param fontFamily The font face name (default: L"Segoe UI").
	 * @return A Windows HFONT handle.
	 */
	HFONT CreatePointFont(int pointSize, const std::wstring& fontFamily = L"Segoe UI");

	/**
	 * @brief Converts a UTF-16 wide string to a UTF-8 string.
	 * @param wstr The wide string to convert.
	 * @return A UTF-8 encoded std::string.
	 */
	std::string to_utf8(const std::wstring& wstr);

	/**
	 * @brief Converts a UTF-8 string to a UTF-16 wide string.
	 * @param str The UTF-8 string to convert.
	 * @return A UTF-16 encoded std::wstring.
	 */
	std::wstring to_utf16(const std::string& str);

	std::optional<double> WStringToNumber(const std::wstring& str);
}


