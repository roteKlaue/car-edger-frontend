#pragma once

#include <windows.h>
#include <string>

namespace Util {
	UINT GenerateId();
	HFONT CreatePointFont(int pointSize, const std::wstring& fontFamily = L"Segoe UI");
	std::string to_utf8(const std::wstring& wstr);
	std::wstring to_utf16(const std::string& str);
}
