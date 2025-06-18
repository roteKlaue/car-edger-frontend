#include "pch.h"
#include "Util.h"
#include <atomic>
#include <sstream>

static std::atomic<UINT> s_nextMenuId = 1;

UINT Util::GenerateId() {
    return s_nextMenuId++;
}

HFONT Util::CreatePointFont(int pointSize, const std::wstring& fontFamily)
{
    HDC screenDC = GetDC(nullptr);
    int dpiY = GetDeviceCaps(screenDC, LOGPIXELSY);
    ReleaseDC(nullptr, screenDC);

    int logicalHeight = -MulDiv(pointSize, dpiY, 72);

    return CreateFontW(
        logicalHeight,    // height
        0,                // average character width (0 = match height)
        0,                // escapement
        0,                // orientation
        FW_NORMAL,        // weight
        FALSE,            // italic
        FALSE,            // underline
        FALSE,            // strikeout
        DEFAULT_CHARSET,  // character set
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        VARIABLE_PITCH | FF_DONTCARE,
        fontFamily.c_str()
    );
}


std::string Util::to_utf8(const std::wstring& wstr) {
    if (wstr.empty()) return {};

    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), result.data(), size, nullptr, nullptr);
    return result;
}

std::wstring Util::to_utf16(const std::string& str) {
    if (str.empty()) return {};

    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), size);
    return result;
}

std::optional<double> Util::WStringToNumber(const std::wstring& str) {
    std::wistringstream stream(str);
    double value;
    stream >> value;

    if (!stream.fail() && stream.eof())
        return value;

    return std::nullopt;
}