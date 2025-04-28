#include "pch.h"
#include "Util.h"
#include <atomic>

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
