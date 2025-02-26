#include "ClassRegisterer.h"

ClassRegisterer* ClassRegisterer::GetInstance()
{
	return nullptr;
}

ATOM ClassRegisterer::registerClass(WCHAR classname[], size_t len, WNDCLASSEXW wcex)
{
    std::wstring className(classname, len);

    if (classCache.find(className) != classCache.end())
    {
        return 0;
    }

    auto atom = RegisterClassExW(&wcex);


    if (atom == 0)
    {
        return 0;
    }

    classCache[className] = wcex;
    return atom;
}

bool ClassRegisterer::classExists(const WCHAR classname[], size_t len)
{
    std::wstring className(classname, len);
    return (classCache.find(className) != classCache.end());
}

WNDCLASSEXW ClassRegisterer::getWindowClass(const WCHAR classname[], size_t len)
{
    std::wstring className(classname, len);

    // Check if the class exists in the cache
    auto it = classCache.find(className);
    if (it != classCache.end())
    {
        return it->second; // Return the cached class
    }

    // If not found, return an empty WNDCLASSEXW object (to indicate not found)
    return WNDCLASSEXW{};
}
