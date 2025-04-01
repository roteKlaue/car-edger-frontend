#include "pch.h"
#include "ClassRegisterer.h"

ClassRegisterer* ClassRegisterer::GetInstance()
{
    static ClassRegisterer instance;
    return &instance;
}

ATOM ClassRegisterer::registerClass(const std::wstring classname, WNDCLASSEXW wcex)
{
    if (classCache.find(classname) != classCache.end()) return 0;

    auto atom = RegisterClassExW(&wcex);
    if (atom == 0) return 0;

    classCache[classname] = wcex;
    return atom;
}

bool ClassRegisterer::classExists(const std::wstring classname)
{
    return (classCache.find(classname) != classCache.end());
}

WNDCLASSEXW ClassRegisterer::getWindowClass(const std::wstring classname)
{
    // Check if the class exists in the cache
    auto it = classCache.find(classname);
    if (it != classCache.end()) return it->second; // Return the cached class

    // If not found, return an empty WNDCLASSEXW object (to indicate not found)
    return WNDCLASSEXW{};
}

bool ClassRegisterer::unregisterClasses()
{
    for (auto& pair : classCache)
    {
        if (!UnregisterClassW(pair.first.c_str(), pair.second.hInstance)) return false;
    }
    classCache.clear();
    return true;
}
