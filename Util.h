#pragma once

#include <windows.h>
#include <string>

namespace Util {
	UINT GenerateId();
	HFONT CreatePointFont(int pointSize, const std::wstring& fontFamily = L"Segoe UI");
}
